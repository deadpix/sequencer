#include "mainwindow.h"
#include <QApplication>

#define ARCH_X86	1


int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.resize(600,600);
	mainWindow.setWindowTitle("test button");
	mainWindow.show();
	return app.exec();
}
