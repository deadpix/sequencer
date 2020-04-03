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
	hw_nt.cpp \
	hw_sr.cpp \
	../../sequencepter/src/bit.cpp \
	../../sequencepter/src/fct_clbk.cpp \
	../../sequencepter/src/perf.cpp \
	../../sequencepter/src/led_toogle.cpp \
	../../sequencepter/src/seq_param.cpp \
	../../sequencepter/src/tempo.cpp \
	../../sequencepter/src/gate.cpp \
	../../sequencepter/src/menu.cpp \
	../../sequencepter/src/midi_controller.cpp \
	../../sequencepter/src/midi_controller_param.cpp \
	../../sequencepter/src/keyboard.cpp \
	../../sequencepter/src/gui.cpp \
	../../sequencepter/src/param.cpp \
	../../sequencepter/src/sequencer.cpp \
	../../sequencepter/src/sequenception.cpp \
	../../sequencepter/src/test_proj_two.cpp \
	../../sequencepter/src/clk.cpp \
	../../sequencepter/src/prog.cpp \
	../../sequencepter/src/step.cpp \
	../../sequencepter/src/track.cpp \
	../../sequencepter/src/node.cpp \
        ../../sequencepter/src/led_matrix.cpp \
	../../sequencepter/src/sequencer/fct_loop_setting.cpp \
        ../../sequencepter/src/sequencer/fct_normalization.cpp \
        ../../sequencepter/src/sequencer/fct_step.cpp

HEADERS  += mainwindow.h \
	../../sequencepter/src/blink.h \
	configuration.h \
	../../sequencepter/src/hw.h \ 
	../../sequencepter/src/perf.h \
	../../sequencepter/src/led_matrix.h \
	../../sequencepter/src/bit.h \
	../../sequencepter/src/types.h \
	../../sequencepter/src/fct_clbk.h \
	../../sequencepter/src/led_toogle.h \
	../../sequencepter/src/seq_param.h \
	../../sequencepter/src/tempo.h \
	../../sequencepter/src/gate.h \
	../../sequencepter/src/menu.h \
	../../sequencepter/src/midi_controller.h \
	../../sequencepter/src/midi_controller_param.h \
	../../sequencepter/src/gui.h \
	../../sequencepter/src/param.h \
	../../sequencepter/src/sequencer.h \
	../../sequencepter/src/sequenception.h \
	../../sequencepter/src/test_proj_two.h \
	../../sequencepter/src/clk.h \
	../../sequencepter/src/prog.h \
	../../sequencepter/src/step.h \
	../../sequencepter/src/track.h \
        ../../sequencepter/src/blink.h \
        ../../sequencepter/src/node.h \
	../../sequencepter/src/led_matrix.h \
	../../sequencepter/src/sequencer/fct_loop_setting.h \
	../../sequencepter/src/sequencer/fct_step.h \
        ../../sequencepter/src/sequencer/fct_normalization.h \
        hw_nt.h \
	hw_sr.h 
FORMS    += mainwindow.ui
