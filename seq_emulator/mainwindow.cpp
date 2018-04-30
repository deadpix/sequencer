#include "mainwindow.h" 

#include <QCoreApplication>
const char* red   = "background-color: red";
const char* green = "background-color: green";
const char* blue  = "background-color: blue";
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

	menu_btn = new QPushButton("menu", this);
	menu_btn->setGeometry(QRect(QPoint(pos_x, 60 * 9), QSize(50, 50)));

	param_btn = new QPushButton("param", this);
	param_btn->setGeometry(QRect(QPoint(pos_x + 60, 60 * 9), QSize(50, 50)));

	ms = 0;

//	m_button = new QPushButton("My Button", this);
//	// set size and location of the button
//	m_button->setGeometry(QRect(QPoint(50, 10),
//	QSize(50, 50)));
//
//	// Connect button signal to appropriate slot
//	connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
}
 
void MainWindow::handleButton(int id)
{
	unsigned long long tmp = ms;
	qDebug ("click on btn %d: ms=%d\r\n",id,tmp);
	ms = 0;
}
void MainWindow::handleTimerUI(){
	matrix_btn[step_cnt]->setStyleSheet(white);
      	step_cnt = (step_cnt + 1) % MATRIX_NR_BTNS;
	matrix_btn[step_cnt]->setStyleSheet(red);
}
