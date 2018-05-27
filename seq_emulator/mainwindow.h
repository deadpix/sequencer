#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <elapsedMillis.h>
#include "../test7-refactor/src/led_matrix.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>
#include <QMutex>

#define MATRIX_ROW	8
#define MATRIX_COL	8
#define MATRIX_NR_BTNS	(MATRIX_ROW * MATRIX_COL)

namespace Ui {
	class MainWindow;
}

struct _btn_status {
        uint8_t push;
        uint8_t long_push;
};



class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		explicit MainWindow(QWidget *parent = 0);
		
	private slots:
		void handleButtonPress(int);
		void handleButtonRelease(int);
		void handleTimerUI();
		void handleParamBtn();
		void handleMenuBtn();
		void handleMainLoop();
	private:
		QMutex mutex;
		QPushButton *menu_btn, *param_btn;
		QPushButton *matrix_btn[MATRIX_NR_BTNS];
		int step_cnt;
		led_matrix lm;
		uint8_t btn_status[MATRIX_NR_BTNS];
		elapsedMillis btn_ms[MATRIX_NR_BTNS];
		uint8_t menu_btn_status;
		uint8_t param_btn_status;
		void check_matrix_btn();
		void setup();
	
	QSignalMapper mapper_press;
	QSignalMapper mapper_release;
	QTimer *ui_timer;	
	QTimer *loop_timer;	
};
 

#endif // MAINWINDOW_H
