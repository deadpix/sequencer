#ifndef __BIT_H__
#define __BIT_H__

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include <Arduino.h>
//#else
//	#include <WProgram.h>
//#endif
#include <stdint.h>

namespace BIT {
	int  is_bit_set(uint16_t reg, int bit_pos);
	int  find_next_bit (uint16_t *word, uint16_t limit, uint16_t from);
	void clear_bits(uint16_t *word, uint16_t from, uint16_t len);
	void set_bits(uint16_t *word, uint16_t from, uint16_t len);
	int  check_bit(uint16_t *word, int bit);
	uint8_t get_highest_bit_set(unsigned int);
}


#define for_eachset_bit(bit, word, limit) 					\
        for ((bit) = (BIT::find_next_bit((word), (limit), 0));			\
             ((bit) < (limit)) && (bit != -1);					\
             (bit) = (BIT::find_next_bit((word), (limit), (bit) + 1)))


#endif
