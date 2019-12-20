#include "bit.h"

//#define WORD_BITS			16
//#define WORD_BITS_LOG2		4
#define WORD_BITS			8
#define WORD_BITS_LOG2		3
#define bitwise_getword(x)	((x) >> WORD_BITS_LOG2)	/* Divide by 8 */
#define	bitwise_getbit(x)	(1UL << ((x) % WORD_BITS))

int BIT::is_bit_set(uint8_t reg, int bit_pos){
	return reg & (1U << bit_pos);
}

int BIT::find_next_bit (uint8_t *word, uint16_t limit, uint16_t from){
	int success = 0;
	int i;

	for(i = from; i < limit; i++) {
		if ((word[bitwise_getword(i)] & bitwise_getbit(i))) {
			success = 1;
			break;
		}
	}
	if (success)
		return i;
	else
		return -1;
}

void BIT::clear_bits(uint8_t *word, uint16_t from, uint16_t len){
	int i;
	for (i = from; i < from + len; i++) {
		word[bitwise_getword(i)] &= ~(bitwise_getbit(i));
	} 
}

void BIT::set_bits(uint8_t *word, uint16_t from, uint16_t len){
	int i;
	for(i = from; i < from + len; i++) {
		word[bitwise_getword(i)] |= bitwise_getbit(i);
	}
}

int BIT::check_bit(uint8_t *word, int bit){
	if (word[bitwise_getword(bit)] & bitwise_getbit(bit)) {
		return 0;
	} else {
		return -1;
	}
}

uint8_t BIT::get_highest_bit_set(unsigned int v){
	uint8_t ret = 0;
	while (v >>= 1){
    		ret++;
	}
	return ret;
}
