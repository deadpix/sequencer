/*
 *      Copyright (c) 2017 Vincent "deadpix" Dupre
 *
 *      Author: Vincent Dupre (vdupre@gmail.com)
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../sequencepter/src/bit.h"
#include "../../sequencepter/src/types.h"
#include "../../sequencepter/src/sequenception.h"
#include "../../sequencepter/src/gui.h"
#include "../../sequencepter/src/perf.h"

#include <iostream>
#include <sstream> // for ostringstream
#include <string>

#include <hw_debug.h>

#define TRACE_PERF              0

const char* red     = "background-color: red";
const char* green   = "background-color: green";
const char* blue    = "background-color: blue";
const char* yellow  = "background-color: yellow";
const char* magenta = "background-color: magenta";
const char* cyan    = "background-color: cyan";
const char* white   = "background-color: white";
const char* gray    = "background-color: gray";


const char* color_arr[] = {
		"background-color: gray", 	// no color: 0
		"background-color: red",	// red: 1
		"background-color: green",	// green: 2
		"background-color: yellow",	// yellow: 3
		"background-color: blue",	// blue: 4
		"background-color: magenta",	// magenta: 5
		"background-color: cyan",	// cyan: 6
		"background-color: white",	// white: 7
};

static elapsedMillis ms;
static gui oled_gui;
static sequenception sequenception;
static param* param_ptr;
static QLabel* oled[OLED_LINE];
#if TRACE_PERF == 1
static perf p;
#endif
static void tempo_change_handler(uint32_t ms){
	qDebug("tempo_change_handler ms=%d",ms);
}
static void midi_note_on(uint16_t note, uint8_t vel, uint8_t chan){
	qDebug("midi_note_on note=%d vel=%d chan=%d",note,vel,chan);
}

static void refresh_oled(char** line_arr){
//	qDebug("OLED[0]: %s",line_arr[0]);
//	qDebug("OLED[1]: %s",line_arr[1]);
//	qDebug("OLED[2]: %s",line_arr[2]);
	oled[0]->setText(line_arr[0]);
	oled[1]->setText(line_arr[1]);
	oled[2]->setText(line_arr[2]);

}

static void printTree(node * n, QPlainTextEdit * out, QString * str){
    const QString space = "           ";
    //QTextCursor::Right
    // movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, col_space)
    if(!n->_children || n->_children->size() == 0){
        // node is a leaf
        if(!n->_step){
            dbg::printf("ERROR: leaf does not contain step...\n");
            return;
        }
        QString tmp;
        tmp.sprintf("-(%2u/%2u,%2u)"
                    ,n->_step->_clk_def.numerator
                    ,n->_step->_clk_def.denominator
                    ,n->_mtx_id);
        if(str->isNull()){
            for(int i=0;i<n->_node_depth;i++){
                str->append(space);
            }
        }
        str->append(tmp);
        out->appendPlainText(*str);
        str->clear();
    } else {
        QString tmp;
        tmp.sprintf("-(XX/XX,%2u)"
                    ,n->_mtx_id);
        if(str->isNull()){
            for(int i=0;i<n->_node_depth;i++){
                str->append(space);
            }
        }
        str->append(tmp);

        for(int i=0; i<n->_children->size();i++){
            printTree(n->_children->get(i), out, str);
        }
    }

}


#define BTN_RELEASED	0
#define BTN_PUSHED	1
#define BTN_LONG_PUSHED	2
#define LONG_PRESS_TIME_MS	1000

//static void init_menu_btn(prog* p){
//	sequenception.menu_ctrl.set_next_prog(p);
//}

void MainWindow::setup(){
    dbg::printf("start setup\n");
//	led_matrix::set_hw(_hw_emulator);
	oled_gui.init_gui(refresh_oled);
	sequenception.fct_midi = midi_note_on;
	sequenception.fct_tempo_change = tempo_change_handler;
	sequenception.init(&oled_gui);
	sequenception.menu_ctrl.set_next_prog(sequenception.current_prog);
//	_hw_emulator->set_lm(sequenception.current_prog->get_led_matrix());
	_hw_emulator->switch_matrix(sequenception.current_prog->get_led_matrix(), sequenception.current_prog->get_led_matrix());
	param_ptr = NULL;
	dbg::printf("end setup\n");
}

void MainWindow::loadFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QByteArray line = file.readLine();
    if(!file.atEnd()) {
        qDebug("multi line file in %s????\n",__FUNCTION__);
    }
    qDebug("line size %d\n",line.size());

    if(line.size() > HW_EEPROM_BYTE_SIZE){
        qDebug("not enough memory...\n");
        return;
    }
    // get header
    struct serialized_tree_t st = {.nr_byte = HW_EEPROM_BYTE_SIZE };
    st.header.magic = line.at(SERIALIZATION_HEADER_MAGIC_BYTE_OFFSET);
    st.header.nr_nodes = static_cast<uint16_t>(
                (line.at(SERIALIZATION_HEADER_NR_NODES_BYTE_OFFSET) << 8)
              | (line.at(SERIALIZATION_HEADER_NR_NODES_BYTE_OFFSET + 1)));
    st.header.nr_steps = static_cast<uint16_t>(
                (line.at(SERIALIZATION_HEADER_NR_STEPS_BYTE_OFFSET) << 8)
              | (line.at(SERIALIZATION_HEADER_NR_STEPS_BYTE_OFFSET + 1)));
    st.header.tree_byte_sz = static_cast<uint16_t>(
                (line.at(SERIALIZATION_HEADER_TREE_SZ_BYTE_OFFSET) << 8)
              | (line.at(SERIALIZATION_HEADER_TREE_SZ_BYTE_OFFSET + 1)));

    dbg::printf("magic=%u nr_nodes=%u nr_steps=%u tree_byte_sz=%u header size=%u\n",
                st.header.magic,st.header.nr_nodes,st.header.nr_steps,
                st.header.tree_byte_sz,sizeof(st.header));

    uint8_t * buffer = new uint8_t[line.size()-SERIALIZATION_HEADER_BYTE_SIZE];
    for(int i=0;i<(line.size()-SERIALIZATION_HEADER_BYTE_SIZE);i++){
        buffer[i] = line.at(SERIALIZATION_HEADER_BYTE_SIZE+i);
        dbg::printf("buf[%d] = %u ",i,buffer[i]);
    }
    st.buf = buffer;

    flgProcess = false;

    int ret = sequenception.midi_seq.deserialize_current_track(&st);
    if(ret == SERIALIZATION_OK){
        // delete tree
//      sequenception.midi_seq.get_current_track()->get_root_node()->delete_tree();
    }

    flgProcess = true;


    file.close();
}
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        qDebug("open file\n");
        loadFile(fileName);
    }
}
void MainWindow::saveFile()
{
    struct serialized_tree_t st = {.nr_byte = HW_EEPROM_BYTE_SIZE };
//    st.nr_byte = HW_EEPROM_BYTE_SIZE;
    st.buf = new uint8_t[HW_EEPROM_BYTE_SIZE];
    uint16_t s_data_sz;
    sequenception.midi_seq.serialize_current_track(&st, &s_data_sz);
    QString fileName = QFileDialog::getSaveFileName(this,
           tr("Save sequencer track"), "", "");
    dbg::printf("serialized data size=%d\n",s_data_sz);
    if (fileName.isEmpty()){
        return;
    }
    else {

        QFile file1(fileName);

        if (!file1.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file1.errorString());
            return;
        }
        dbg::printf("file %s will be saved!\n",fileName.toUtf8().constData());

        QDataStream out1(&file1);

        out1 << st.header.magic;
        out1 << st.header.nr_nodes;
        out1 << st.header.nr_steps;
        out1 << st.header.tree_byte_sz;

//        dbg::printf("test");
//        dbg::printf("%u %u %u %u\n",st.header.magic,st.header.nr_nodes,
//                    st.header.nr_steps,st.header.tree_byte_sz);

        for(int i=0;i<s_data_sz;i++){
            out1 << st.buf[i];
            dbg::printf("%u ",st.buf[i]);
        }
        file1.close();

    }
    delete st.buf;

}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Load sequence"), this);
    saveAct = new QAction(tr("&Save sequence"), this);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QString str = "";
	bool fail = false;

    menu = this->findChild<QMenuBar *>(QString("menuBar"));
    file = this->findChild<QMenu *>(QString("menuSequenception"));

    logs = this->findChild<QPlainTextEdit *>(QString("logWindow"));
    logs->setReadOnly(true);
    QTextDocument *doc = logs->document();
    QFont font = doc->defaultFont();
    font.setFamily("Courier New");
    doc->setDefaultFont(font);

    logs->appendPlainText("**** sequencepter simulator logs ****\n");

//    file->addMenu("&Load sequence");
//    file->addMenu("&Save sequence");
    createActions();

    file->addAction(openAct);
    file->addAction(saveAct);
    menu->addMenu(file);

	for(int i=0;i<MATRIX_NR_BTN;i++){
		str = QString("pushButton_%1").arg(i);
		btnMatrixStatus[i] = BTN_RELEASED;
		btnMatrix[i] = this->findChild<QPushButton *>(str);
		if(!btnMatrix[i]){
			qDebug("unable to find btn %s...",str.toUtf8().constData());
			fail = true;
		} 
		else {
			connect(btnMatrix[i], SIGNAL (released()), &signalMatrixMapperRelease, SLOT (map()));
			connect(btnMatrix[i], SIGNAL (pressed()), &signalMatrixMapperPress, SLOT (map()));
			signalMatrixMapperPress.setMapping(btnMatrix[i], i);
			signalMatrixMapperRelease.setMapping(btnMatrix[i], i);			
		}
	}

	if(!fail){
		connect(&signalMatrixMapperPress, SIGNAL(mapped(int)), this, SLOT(handleBtnPress(int)));
		connect(&signalMatrixMapperRelease, SIGNAL(mapped(int)), this, SLOT(handleBtnRelease(int)));
	}
	
	uiTimer = new QTimer(this);
	connect(uiTimer, SIGNAL(timeout()), this, SLOT(handleTimerUI()));
	uiTimer->start(2);

	for(int i=0;i<OLED_LINE;i++){
		str = QString("oled_%1").arg(i);
		oled[i] = this->findChild<QLabel *>(str);
		
		if(!oled[i]){
			qDebug("unable to find label %s...",str.toUtf8().constData());
		}
	}

	btnMenu = this->findChild<QPushButton *>("menu");
	if(!btnMenu){
		qDebug("unable to find menu button %s...",str.toUtf8().constData());
		fail = true;
	}
	else {
		connect(btnMenu, SIGNAL (released()), this, SLOT (handleMenuBtn()));
	}
		
	btnParam = this->findChild<QPushButton *>("param");
	if(!btnParam){
		qDebug("unable to find parameter button %s...",str.toUtf8().constData());
		fail = true;
	}
	else {
		connect(btnParam, SIGNAL (released()), this, SLOT (handleParamBtn()));
	}
	
    btnStart = this->findChild<QPushButton *>("start");
    btnStop  = this->findChild<QPushButton *>("stop");
    btnPause = this->findChild<QPushButton *>("pause");
    btnReset = this->findChild<QPushButton *>("reset");

    if(!btnStart || !btnStop || !btnPause || !btnReset){
        qDebug("unable to find transport buttons...");
        fail = true;
    }
    else {
        connect(btnStart, SIGNAL (released()), this, SLOT (handleStartBtn()));
        connect(btnStop,  SIGNAL (released()), this, SLOT (handleStopBtn()));
        connect(btnPause, SIGNAL (released()), this, SLOT (handlePauseBtn()));
        connect(btnReset, SIGNAL (released()), this, SLOT (handleResetBtn()));
    }

    btnRefresh = this->findChild<QPushButton *>("refresh");
    if(!btnRefresh){
        qDebug("unable to find refresh button...");
    } else {
        connect(btnRefresh, SIGNAL (released()), this, SLOT (handleRefreshBtn()));
    }



	btnMenuStatus = BTN_RELEASED;
	btnParamStatus = BTN_RELEASED;
#if HW_ADAFRUIT_NEOTRELLIS == 1
	_hw_emulator = new hw_nt(btnMatrix);
#elif HW_SHIFT_REGISTER == 1
	_hw_emulator = new hw_sr();
#endif
	setup();

    QString tmp;
    printTree(sequenception.midi_seq.get_current_track()->get_root_node(),logs, &tmp);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleBtnPress(int id){
	btnMatrixStatus[id] = BTN_PUSHED;
	btnMatrixMs[id] = 0;	
	sequenception.current_prog->on_push(id);
}
void MainWindow::handleBtnRelease(int id){
	if(btnMatrixStatus[id] == BTN_LONG_PUSHED){
		qDebug("long release %d",id);
		sequenception.current_prog->on_long_release(id);
	}
	else if(btnMatrixStatus[id] == BTN_PUSHED){
		qDebug("release %d",id);
		sequenception.current_prog->on_release(id);
	} 
	else {
		qDebug("Fatal error in handleButtonRelease for button %d!!!!!",id);
	}
	btnMatrixStatus[id] = BTN_RELEASED;
}
void MainWindow::checkBtnMatrix(){
	for(int i=0;i<MATRIX_NR_BTN;i++){
		if(btnMatrixStatus[i] == BTN_PUSHED){
			if(btnMatrixMs[i] > LONG_PRESS_TIME_MS){
				btnMatrixStatus[i] = BTN_LONG_PUSHED;
				sequenception.current_prog->on_long_push(i);
			}
		}
	}
}
static void updBtnColorRow(uint8_t* bmp_ret, uint8_t bmp, const char* color, int row_id, QPushButton *matrix_btn[MATRIX_NR_BTN]){
	int bit = 0;
	for_eachset_bit(bit, &bmp, 8){
		if(!BIT::is_bit_set(*bmp_ret, bit)){
			matrix_btn[row_id * 8 + bit]->setStyleSheet(color);
			BIT::set_bits(bmp_ret, bit, 1);
		}
	}	
}
static void rstBtnColorRow(const char* color, int row_id, QPushButton *matrix_btn[MATRIX_NR_BTN]){
	for(int i=0;i<8;i++){	
		matrix_btn[row_id * 8 + i]->setStyleSheet(color);
	}	
}

#if HW_SHIFT_REGISTER == 1
static void updBtnColor(hw_sr* hw, QPushButton* matrix_btn[MATRIX_NR_BTN]){
	led_t* leds = hw->get_led_arr();
	
    uint8_t tmp = 0x0;
    uint8_t set_led_bmp = 0x0;
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){

		rstBtnColorRow(gray, i, matrix_btn);

		set_led_bmp = 0x0;

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[1] & leds[i].bitmap[2];
		updBtnColorRow(&set_led_bmp, tmp, white, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[1];
		updBtnColorRow(&set_led_bmp, tmp, yellow, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[2];
		updBtnColorRow(&set_led_bmp, tmp, magenta, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[1] & leds[i].bitmap[2];
		updBtnColorRow(&set_led_bmp, tmp, cyan, i, matrix_btn);
	
		tmp = 0x0;
		tmp = leds[i].bitmap[0];
		updBtnColorRow(&set_led_bmp, tmp, red, i, matrix_btn);
	
		tmp = 0x0;
		tmp = leds[i].bitmap[1];
		updBtnColorRow(&set_led_bmp, tmp, green, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[2];	
		updBtnColorRow(&set_led_bmp, tmp, blue, i, matrix_btn);
	}
}
#endif


void MainWindow::handleTimerUI(){
//	qDebug("timer expired");
#if TRACE_PERF == 1
	p.start_ms_counter();
#endif
    if(flgProcess){
        sequenception.do_isr();
        sequenception.loop(0);

#if HW_SHIFT_REGISTER == 1
//      updBtnColor(sequenception.lm_ptr, btnMatrix);
        updBtnColor(_hw_emulator, btnMatrix);
#endif
        checkBtnMatrix();
#if TRACE_PERF == 1
        p.stop_ms_counter();
        if(p.get_perf_cnt() >= 1000){
            p.print_perf();
            p.reset_ms_counter();
        }
#endif
    }
}
void MainWindow::handleParamBtn(){
	led_matrix* prev = sequenception.current_prog->get_led_matrix();
	if(!btnParamStatus){
		btnParam->setStyleSheet(red);
		btnParamStatus = BTN_PUSHED;
		btnMenu->setEnabled(false);
	
		qDebug ("handle param button");
	
		param_ptr = sequenception.current_prog->get_param();
		if(param_ptr){
			sequenception.lm_ptr = param_ptr->get_led_matrix();
			sequenception.current_prog = param_ptr;
			_hw_emulator->switch_matrix(sequenception.lm_ptr, prev);
			param_ptr->param_on_enter();
		}

	} else {
		btnParam->setStyleSheet(white);
		btnParamStatus = BTN_RELEASED;
		btnMenu->setEnabled(true);

		qDebug ("param menu button");
		
		if(param_ptr){
			sequenception.current_prog = param_ptr->get_prog();
			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
			_hw_emulator->switch_matrix(sequenception.lm_ptr, prev);
			param_ptr->param_on_leave();
		}

	}
}

void MainWindow::handleMenuBtn(){
	led_matrix* prev = sequenception.current_prog->get_led_matrix();
	if(!btnMenuStatus){
		btnMenu->setStyleSheet(red);
		btnMenuStatus = BTN_PUSHED;
		btnParam->setEnabled(false);
		qDebug ("handle menu button");

		sequenception.lm_ptr = sequenception.menu_ctrl.get_menu_led_matrix();
		sequenception.current_prog = sequenception.prog_arr[sequenception.nr_prog];
		_hw_emulator->switch_matrix(sequenception.lm_ptr, prev);
		sequenception.menu_ctrl.menu_enter();

	} else {
		btnMenu->setStyleSheet(white);
		btnMenuStatus = BTN_RELEASED;
		btnParam->setEnabled(true);
		qDebug ("release menu button");

		sequenception.current_prog = sequenception.menu_ctrl.get_next_prog();
		sequenception.current_prog->display_title();
		sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
		_hw_emulator->switch_matrix(sequenception.lm_ptr, prev);
		sequenception.menu_ctrl.menu_leave();
	}

}
void MainWindow::handleStartBtn(){
    qDebug("push start button");
    sequenception.midi_seq.start_all_tracks();
}
void MainWindow::handleStopBtn(){
    qDebug("push stop button");
    sequenception.midi_seq.stop_all_tracks();
}
void MainWindow::handlePauseBtn(){
    qDebug("push pause button");
    sequenception.midi_seq.pause_all_tracks();
}
void MainWindow::handleResetBtn(){
    qDebug("push reset button");
    sequenception.midi_seq.reset_all();
}
void MainWindow::handleRefreshBtn(){
    qDebug("refresh tree");
    QString tmp;
    printTree(sequenception.midi_seq.get_current_track()->get_root_node(),logs, &tmp);
}
