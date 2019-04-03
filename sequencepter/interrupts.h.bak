#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#define DISABLE_IRQ()				\
	unsigned char tmp_reg = disable_irq();	\

#define ENABLE_IRQ()				\
	enable_irq(tmp_reg);			\

static inline unsigned char disable_irq(){
	unsigned char reg = SREG;
	cli();
	return reg;
}

static inline void enable_irq(unsigned char reg){
	SREG = reg;
}

#endif
