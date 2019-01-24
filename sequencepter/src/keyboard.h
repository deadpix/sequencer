#ifndef __SRC_KEYBOARD_H__
#define __SRC_KEYBOARD_H__

#define KEYBOARD_NR_INTERVALS	12 // 12 semi-tones

struct scale {
	uint8_t nr_notes;
	uint8_t intervals[KEYBOARD_NR_INTERVALS];
};

// btn id of the key
const uint8_t keys_offset {0,9,2,10,3,4,12,5,13,6,14,7};

const struct scale scales[] = {
	{ 12, {0,1,2,3,4,5,6,7,8,9,10,11} }, // semi-tones
	{ 7, {0, 2, 4, 5, 7, 9, 10} },
};

#endif
