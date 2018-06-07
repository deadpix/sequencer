#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <elapsedMillis.h>
#include "../../test7-refactor/src/led_matrix.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>
#include <QLabel>
#include <QtDebug>

#define MATRIX_NR_COL	8
#define MATRIX_NR_ROW	8
#define MATRIX_NR_BTN	(MATRIX_NR_COL * MATRIX_NR_ROW)
#define OLED_LINE	3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void handleBtnPress(int);
	void handleBtnRelease(int);
	void handleTimerUI();
	void handleParamBtn();
	void handleMenuBtn();

private:
	Ui::MainWindow *ui;

	QSignalMapper signalMatrixMapperPress;
	QSignalMapper signalMatrixMapperRelease;
	
	QTimer *uiTimer;	
	
	QPushButton* btnMatrix[MATRIX_NR_BTN];
	QPushButton* btnMenu;
	QPushButton* btnParam;
	QLabel*	     oled[OLED_LINE];

	uint8_t btnMatrixStatus[MATRIX_NR_BTN];
	elapsedMillis btnMatrixMs[MATRIX_NR_BTN];
	uint8_t btnMenuStatus;
	uint8_t btnParamStatus;
	void checkBtnMatrix();
	void setup();

};

#endif // MAINWINDOW_H
