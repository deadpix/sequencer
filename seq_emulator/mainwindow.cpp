#include "mainwindow.h" 
#include "../test7-refactor/src/bit.h"
#include "../test7-refactor/src/types.h"
#include <stdint.h>
#include <QCoreApplication>

const char* red   = "background-color: red";
const char* green = "background-color: green";
const char* blue  = "background-color: blue";
const char* yellow = "background-color: yellow";
const char* magenta = "background-color: magenta";
const char* cyan = "background-color: cyan";
const char* white = "background-color: white";

static elapsedMillis ms;

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
		connect(matrix_btn[i], SIGNAL (released()), &mapper, SLOT (map()));
		mapper.setMapping(matrix_btn[i], i);
	}
	connect(&mapper, SIGNAL(mapped(int)), this, SLOT(handleButton(int)));

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

	connect(menu_btn, SIGNAL (released()), this, SLOT (handleParamBtn()));
	connect(param_btn, SIGNAL (released()), this, SLOT (handleMenuBtn()));

	ms = 0;

//	m_button = new QPushButton("My Button", this);
//	// set size and location of the button
//	m_button->setGeometry(QRect(QPoint(50, 10),
//	QSize(50, 50)));
//
//	// Connect button signal to appropriate slot
//	connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));


	lm.set_led_x(LED_B_IDX, 1);
	lm.set_led_x(LED_R_IDX, 2);
	lm.set_led_x(LED_G_IDX, 4);
	lm.set_led_x(LED_GB_IDX, 10);
	lm.set_led_x(LED_RB_IDX, 25);
	lm.set_led_x(LED_RG_IDX, 32);
	lm.set_led_x(LED_GB_IDX, 47);
	lm.set_led_x(LED_GBR_IDX, 63);
}

void MainWindow::handleParamBtn(){
	qDebug ("handle param button");
}

void MainWindow::handleMenuBtn(){
	qDebug ("handle menu button");
}

void MainWindow::handleButton(int id)
{
	unsigned long long tmp = ms;
	qDebug ("click on btn %d: ms=%d",id,tmp);
	ms = 0;
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

static void upd_btn_color(led_matrix* lm, QPushButton* matrix_btn[MATRIX_NR_BTNS]){
	led_t* leds = lm->get_led_arr();
	uint16_t tmp = 0x0;
	uint16_t set_led_bmp = 0x0;
	for(int i=0;i<LED_MATRIX_NR_GROUND;i++){
		set_led_bmp = 0x0;

		tmp = 0x0;
		tmp = leds[i].bitmap[0] & leds[i].bitmap[1] & leds[i].bitmap[2];
		upd_btn_color_row(&set_led_bmp, tmp, white, i, matrix_btn);
		qDebug("col%d set_led_bmp=%x",i,set_led_bmp);

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
	
	upd_btn_color(&lm, matrix_btn);
}
void MainWindow::handleMainLoop(){
}
