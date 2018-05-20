#ifndef MAINWINDOW_H
#define MAINWINDOW_H
 
#include <elapsedMillis.h>
#include "../test7-refactor/src/led_matrix.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSignalMapper>
#include <QTimer>

#define MATRIX_ROW	8
#define MATRIX_COL	8
#define MATRIX_NR_BTNS	(MATRIX_ROW * MATRIX_COL)

namespace Ui {
	class MainWindow;
}
 
class MainWindow : public QMainWindow {
	Q_OBJECT
	public:
		explicit MainWindow(QWidget *parent = 0);
		
	private slots:
		void handleButton(int);
		void handleTimerUI();
		void handleParamBtn();
		void handleMenuBtn();
		void handleMainLoop();
	private:
		QPushButton *menu_btn, *param_btn;
		QPushButton *matrix_btn[MATRIX_NR_BTNS];
		int step_cnt;
		led_matrix lm;
	
	QSignalMapper mapper;
	QTimer *ui_timer;	
	QTimer *loop_timer;	
};
 

#endif // MAINWINDOW_H
