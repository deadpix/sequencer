#include "mainwindow.h" 
#include "../test7-refactor/src/bit.h"
#include "../test7-refactor/src/types.h"
#include "../test7-refactor/src/sequenception.h"
#include "../test7-refactor/src/gui.h"

//#include <stdint.h>
//#include <string.h>
#include <QCoreApplication>
#include <hw_debug.h>

const char* red   = "background-color: red";
const char* green = "background-color: green";
const char* blue  = "background-color: blue";
const char* yellow = "background-color: yellow";
const char* magenta = "background-color: magenta";
const char* cyan = "background-color: cyan";
const char* white = "background-color: white";
const char* gray = "background-color: gray";

static elapsedMillis ms;
static gui oled_gui;
static gui *gui_ctrl;
static sequenception sequenception;
static param* param_ptr;

static void tempo_change_handler(uint32_t ms){
	qDebug("tempo_change_handler ms=%d",ms);
}
static void midi_note_on(uint16_t note, uint8_t vel, uint8_t chan){
	qDebug("midi_note_on note=%d vel=%d chan=%d",note,vel,chan);
}

static void refresh_oled(char** line_arr){
	qDebug("OLED[0]: %s",line_arr[0]);
	qDebug("OLED[1]: %s",line_arr[1]);
	qDebug("OLED[2]: %s",line_arr[2]);
}

#define BTN_RELEASED	0
#define BTN_PUSHED	1
#define BTN_LONG_PUSHED	2
#define LONG_PRESS_TIME_MS	1000

static void init_menu_btn(prog* p){
	sequenception.menu_ctrl.set_next_prog(p);
}

void MainWindow::setup(){
	oled_gui.init_gui(refresh_oled);
	sequenception.fct_midi = midi_note_on;
	sequenception.fct_tempo_change = tempo_change_handler;
	sequenception.init(&oled_gui);
	sequenception.menu_ctrl.set_next_prog(sequenception.current_prog);
	param_ptr = NULL;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	// Create the button, make "this" the parent

	const int pos_x = 10, pos_y = 10, btn_size = 50;
	int x = pos_x;
	int y = pos_y;

	step_cnt = 0;

	for(int i=0;i<MATRIX_NR_BTNS;i++){
		x = pos_x + (btn_size + pos_x) * (i % MATRIX_ROW); 
		y = pos_y + (btn_size + pos_y) * (i / MATRIX_COL);

		matrix_btn[i] = new QPushButton(QString::number(i), this);
		matrix_btn[i]->setGeometry(QRect(QPoint(x, y),
					QSize(btn_size, btn_size)));
//		matrix_btn[i]->setStyleSheet(white);
		connect(matrix_btn[i], SIGNAL (released()), &mapper_release, SLOT (map()));
		connect(matrix_btn[i], SIGNAL (pressed()), &mapper_press, SLOT (map()));
		mapper_press.setMapping(matrix_btn[i], i);
		mapper_release.setMapping(matrix_btn[i], i);
	
		btn_status[i] = BTN_RELEASED;
	}
	connect(&mapper_press, SIGNAL(mapped(int)), this, SLOT(handleButtonPress(int)));
	connect(&mapper_release, SIGNAL(mapped(int)), this, SLOT(handleButtonRelease(int)));

//	for(int i=0;i<3;i++){
//		oled_label[i] = new QLabel(this);
//		oled_label[i]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//		oled_label[i]->setText("test ");
//	}


	ui_timer = new QTimer(this);
	connect(ui_timer, SIGNAL(timeout()), this, SLOT(handleTimerUI()));
	ui_timer->start(100);


	loop_timer = new QTimer(this);
	connect(loop_timer, SIGNAL(timeout()), this, SLOT(handleMainLoop()));
	loop_timer->start(10);

	menu_btn = new QPushButton("menu", this);
	menu_btn->setGeometry(QRect(QPoint(pos_x, 60 * 9), QSize(50, 50)));

	param_btn = new QPushButton("param", this);
	param_btn->setGeometry(QRect(QPoint(pos_x + 60, 60 * 9), QSize(50, 50)));

	connect(param_btn, SIGNAL (released()), this, SLOT (handleParamBtn()));
	connect(menu_btn, SIGNAL (released()), this, SLOT (handleMenuBtn()));

	menu_btn_status = BTN_RELEASED;
	param_btn_status = BTN_RELEASED;

	ms = 0;

	setup();
//	m_button = new QPushButton("My Button", this);
//	// set size and location of the button
//	m_button->setGeometry(QRect(QPoint(50, 10),
//	QSize(50, 50)));
//
//	// Connect button signal to appropriate slot
//	connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));


//	lm.set_led_x(LED_B_IDX, 1);
//	lm.set_led_x(LED_R_IDX, 2);
//	lm.set_led_x(LED_G_IDX, 4);
//	lm.set_led_x(LED_GB_IDX, 10);
//	lm.set_led_x(LED_RB_IDX, 25);
//	lm.set_led_x(LED_RG_IDX, 32);
//	lm.set_led_x(LED_GB_IDX, 47);
//	lm.set_led_x(LED_GBR_IDX, 63);
}

void MainWindow::handleParamBtn(){

	if(!param_btn_status){
		param_btn->setStyleSheet(red);
		param_btn_status = BTN_PUSHED;
		menu_btn->setEnabled(false);
	
		qDebug ("handle param button");
	
		param_ptr = sequenception.current_prog->get_param();
		if(param_ptr){
			sequenception.lm_ptr = param_ptr->get_led_matrix();
			sequenception.current_prog = param_ptr;
			param_ptr->param_on_enter();
		}

	} else {
		param_btn->setStyleSheet(white);
		param_btn_status = BTN_RELEASED;
		menu_btn->setEnabled(true);

		qDebug ("param menu button");
		
		if(param_ptr){
			sequenception.current_prog = param_ptr->get_prog();
			sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
			param_ptr->param_on_leave();
		}

	}
}

void MainWindow::handleMenuBtn(){

	if(!menu_btn_status){
		menu_btn->setStyleSheet(red);
		menu_btn_status = BTN_PUSHED;
		param_btn->setEnabled(false);
		qDebug ("handle menu button");
		sequenception.menu_ctrl.menu_enter();
		sequenception.lm_ptr = sequenception.menu_ctrl.get_menu_led_matrix();
		sequenception.current_prog = sequenception.prog_arr[sequenception.nr_prog];

	} else {
		menu_btn->setStyleSheet(white);
		menu_btn_status = BTN_RELEASED;
		param_btn->setEnabled(true);
		qDebug ("release menu button");
		sequenception.current_prog = sequenception.menu_ctrl.get_next_prog();
		sequenception.current_prog->display_title();
		sequenception.lm_ptr = sequenception.current_prog->get_led_matrix();
		sequenception.menu_ctrl.menu_leave();
	}
	
}

void MainWindow::handleButtonPress(int id)
{
	btn_status[id] = BTN_PUSHED;
	btn_ms[id] = 0;
	sequenception.current_prog->on_push(id);
}
void MainWindow::handleButtonRelease(int id)
{
	if(btn_status[id] == BTN_LONG_PUSHED){
		qDebug("long release %d",id);
		sequenception.current_prog->on_long_release(id);
	}
	else if(btn_status[id] == BTN_PUSHED){
		qDebug("release %d",id);
		sequenception.current_prog->on_release(id);
	} 
	else {
		qDebug("Fatal error in handleButtonRelease for button %d!!!!!",id);
	}
	btn_status[id] = BTN_RELEASED;
}

void MainWindow::check_matrix_btn(){
	for(int i=0;i<MATRIX_NR_BTNS;i++){
		if(btn_status[i] == BTN_PUSHED){
			if(btn_ms[i] > LONG_PRESS_TIME_MS){
				btn_status[i] = BTN_LONG_PUSHED;
				sequenception.current_prog->on_long_push(i);
			}
		}
	}
}

static void upd_btn_color_row(uint16_t* bmp_ret, uint16_t bmp, const char* color, int row_id, QPushButton *matrix_btn[MATRIX_NR_BTNS]){
	int bit = 0;
	for_eachset_bit(bit, &bmp, 8){
		if(!BIT::is_bit_set(*bmp_ret, bit)){
			matrix_btn[row_id * 8 + bit]->setStyleSheet(color);
			BIT::set_bits(bmp_ret, bit, 1);
		}
	}	
}
static void rst_btn_color_row(const char* color, int row_id, QPushButton *matrix_btn[MATRIX_NR_BTNS]){
	for(int i=0;i<8;i++){	
		matrix_btn[row_id * 8 + i]->setStyleSheet(color);
	}	
}


static void upd_btn_color(led_matrix* lm, QPushButton* matrix_btn[MATRIX_NR_BTNS]){
	led_t* leds = lm->get_led_arr();
	uint16_t tmp = 0x0;
	uint16_t set_led_bmp = 0x0;
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){

		rst_btn_color_row(gray, i, matrix_btn);

		set_led_bmp = 0x0;

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[1] & leds[i].bitmap[2];
		upd_btn_color_row(&set_led_bmp, tmp, white, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[1];
		upd_btn_color_row(&set_led_bmp, tmp, yellow, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[2];
		upd_btn_color_row(&set_led_bmp, tmp, magenta, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[1] & leds[i].bitmap[2];
		upd_btn_color_row(&set_led_bmp, tmp, cyan, i, matrix_btn);
	
		tmp = 0x0;
		tmp = leds[i].bitmap[0];
		upd_btn_color_row(&set_led_bmp, tmp, red, i, matrix_btn);
	
		tmp = 0x0;
		tmp = leds[i].bitmap[1];
		upd_btn_color_row(&set_led_bmp, tmp, green, i, matrix_btn);

		tmp = 0x0;
		tmp = leds[i].bitmap[2];	
		upd_btn_color_row(&set_led_bmp, tmp, blue, i, matrix_btn);
	

	}

}

void MainWindow::handleTimerUI(){
//	matrix_btn[step_cnt]->setStyleSheet(white);
  //    	step_cnt = (step_cnt + 1) % MATRIX_NR_BTNS;
//	matrix_btn[step_cnt]->setStyleSheet(red);
	
	upd_btn_color(sequenception.lm_ptr, matrix_btn);
	check_matrix_btn();
	uint32_t clk_res = sequenception.eval_mst_clk();


	if(sequenception.current_prog == sequenception.prog_arr[sequenception.nr_prog]){
		sequenception.menu_ctrl.menu_update();
	} 
	else {
		sequenception.current_prog->update_ui(clk_res, sequenception.mst_clk->clk_get_step_cnt());
	}
	sequenception.loop(clk_res);
}
void MainWindow::handleMainLoop(){
}
