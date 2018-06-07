#-------------------------------------------------
#
# Project created by QtCreator 2018-06-03T22:36:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seq_emu2
TEMPLATE = app
INCLUDEPATH += ./include
DEFINES += IB_USE_STD_STRING

SOURCES += main.cpp \
        mainwindow.cpp \
	../../test7-refactor/src/bit.cpp \
	../../test7-refactor/src/led_matrix.cpp \
	../../test7-refactor/src/fct_clbk.cpp \
	../../test7-refactor/src/led_toogle.cpp \
	../../test7-refactor/src/seq_param.cpp \
	../../test7-refactor/src/tempo.cpp \
	../../test7-refactor/src/gate.cpp \
	../../test7-refactor/src/menu.cpp \
	../../test7-refactor/src/test_proj_one.cpp \
	../../test7-refactor/src/btn_state.cpp \
	../../test7-refactor/src/gui.cpp \
	../../test7-refactor/src/param.cpp \
	../../test7-refactor/src/sequencer.cpp \
	../../test7-refactor/src/sequenception.cpp \
	../../test7-refactor/src/test_proj_two.cpp \
	../../test7-refactor/src/clk.cpp \
	../../test7-refactor/src/prog.cpp \
	../../test7-refactor/src/step.cpp \
	../../test7-refactor/src/track.cpp \
	../../test7-refactor/src/sequencer/fct_loop_setting.cpp \
	../../test7-refactor/src/sequencer/fct_step.cpp 

HEADERS  += mainwindow.h \
	../../test7-refactor/src/led_matrix.h \
	../../test7-refactor/src/bit.h \
	../../test7-refactor/src/types.h \
	../../test7-refactor/src/fct_clbk.h \
	../../test7-refactor/src/led_toogle.h \
	../../test7-refactor/src/seq_param.h \
	../../test7-refactor/src/tempo.h \
	../../test7-refactor/src/gate.h \
	../../test7-refactor/src/menu.h \
	../../test7-refactor/src/test_proj_one.h \
	../../test7-refactor/src/btn_state.h \
	../../test7-refactor/src/gui.h \
	../../test7-refactor/src/param.h \
	../../test7-refactor/src/sequencer.h \
	../../test7-refactor/src/sequenception.h \
	../../test7-refactor/src/test_proj_two.h \
	../../test7-refactor/src/clk.h \
	../../test7-refactor/src/prog.h \
	../../test7-refactor/src/step.h \
	../../test7-refactor/src/track.h \
	../../test7-refactor/src/sequencer/fct_loop_setting.h \
	../../test7-refactor/src/sequencer/fct_step.h 
FORMS    += mainwindow.ui
