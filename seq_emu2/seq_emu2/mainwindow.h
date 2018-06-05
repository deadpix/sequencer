#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QPushButton>
#include <QLabel>
#include <QtDebug>
#include <QTimer>

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
};

#endif // MAINWINDOW_H
