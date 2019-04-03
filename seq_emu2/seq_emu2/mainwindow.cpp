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

#include <hw_debug.h>

#define TRACE_PERF	0

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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QString str = "";
	bool fail = false;


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
	
	btnMenuStatus = BTN_RELEASED;
	btnParamStatus = BTN_RELEASED;
#if HW_ADAFRUIT_NEOTRELLIS == 1
	_hw_emulator = new hw_nt(btnMatrix);
#elif HW_SHIFT_REGISTER == 1
	_hw_emulator = new hw_sr();
#endif
	setup();

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
static void updBtnColorRow(uint16_t* bmp_ret, uint16_t bmp, const char* color, int row_id, QPushButton *matrix_btn[MATRIX_NR_BTN]){
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
	
	uint16_t tmp = 0x0;
	uint16_t set_led_bmp = 0x0;
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
	sequenception.do_isr();
	sequenception.loop(0);

//	uint32_t clk_res = sequenception.eval_mst_clk();
//
//	if(sequenception.current_prog == sequenception.prog_arr[sequenception.nr_prog]){
//		sequenception.menu_ctrl.menu_update();
//	} 
//	else {
//		sequenception.current_prog->update_ui(clk_res, sequenception.mst_clk->clk_get_step_cnt());
//	}
//	sequenception.loop(clk_res);

#if HW_SHIFT_REGISTER == 1
//	updBtnColor(sequenception.lm_ptr, btnMatrix);
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
