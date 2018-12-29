#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <elapsedMillis.h>
#include "../../sequencepter/src/led_matrix.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>
#include <QLabel>
#include <QtDebug>

#include "configuration.h"

namespace Ui {
class MainWindow;
}

#if HW_ADAFRUIT_NEOTRELLIS == 1
#include "hw_nt.h"
#elif HW_SHIFT_REGISTER == 1
#include "hw_sr.h"
#endif

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
//	QLabel*	     oled[OLED_LINE];

	uint8_t btnMatrixStatus[MATRIX_NR_BTN];
	elapsedMillis btnMatrixMs[MATRIX_NR_BTN];
	uint8_t btnMenuStatus;
	uint8_t btnParamStatus;
#if HW_ADAFRUIT_NEOTRELLIS == 1
	hw_nt* _hw_emulator;
#elif HW_SHIFT_REGISTER == 1
	hw_sr* _hw_emulator;
#endif

	void checkBtnMatrix();
	void setup();
	

};

#endif // MAINWINDOW_H
