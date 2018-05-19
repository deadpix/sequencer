QT      += core gui  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET 	 = seq 
TEMPLATE = app 
SOURCES += main.cpp \ 
	   mainwindow.cpp \
	   bit.cpp \
	   led_matrix.cpp
HEADERS += mainwindow.h \
	   elapsedMillis.h \
	   led_matrix.h \
	   bit.h 
