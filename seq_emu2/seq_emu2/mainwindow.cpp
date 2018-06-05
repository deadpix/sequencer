#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QString str = "";
	bool fail = false;

	for(int i=0;i<MATRIX_NR_BTN;i++){
		str = QString("pushButton_%1").arg(i);
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
	uiTimer->start(100);

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
	


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleBtnPress(int btn_id){
	qDebug("btn%d pressed", btn_id);
}
void MainWindow::handleBtnRelease(int btn_id){
	qDebug("btn%d released", btn_id);
}
void MainWindow::handleTimerUI(){
//	qDebug("timer expired");
}
void MainWindow::handleParamBtn(){
	qDebug("handle parameter button");
}
void MainWindow::handleMenuBtn(){
	qDebug("handle menu button");
}
