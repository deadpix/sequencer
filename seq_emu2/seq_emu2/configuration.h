#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define MATRIX_NR_COL	8
#define MATRIX_NR_ROW	8
#define MATRIX_NR_BTN	(MATRIX_NR_COL * MATRIX_NR_ROW)
#define OLED_LINE	3

extern const char* color_arr[];

#define HW_SHIFT_REGISTER       1
#define HW_ADAFRUIT_NEOTRELLIS  0

#define HW_EEPROM_BYTE_SIZE     2048

#endif
