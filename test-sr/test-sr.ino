//LED TEST w/ 74HC595
//by Amanda Ghassaei 2012
//http://www.instructables.com/id/Multiplexing-with-Arduino-and-the-74HC595/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

//this code will light up each led in the 4x4 matrix one at a time


//pin connections- the #define tag will replace all instances of "latchPin" in your code with A1 (and so on)
#define latchPin 22
#define clockPin 23
#define dataPin  21

#define NR_COL	8
#define NR_ROW	4

//looping variables
byte i;
byte j;

//storage variable
byte dataToSend;

IntervalTimer ui_timer;

volatile char check_ui;

byte red[8];
int row_counter;
int freq_cnt;
int freq_len;
void led_next_row(){
//	uint32_t data = 0xFF000000 | (1 << (16+row_counter));
//	uint32_t data = 0x000000ff | (1 << (24+row_counter));
//	uint32_t data = 0xffFF0000 | (1 << row_counter);
//	uint32_t data = 0xff00FF00 | (1 << row_counter);
	uint32_t data = 0x00FFFF00 | (1 << row_counter);
//	data |= ~red[row_counter] << 8;
//	data |= ~red[row_counter] << 16;
	data |= ~red[row_counter] << 24;
//	data |= ~red[row_counter] << 0;
//	uint32_t data = 0xF0F0 | (1 << (8+row_counter));
//	digitalWrite(latchPin, LOW);
//	shiftOut(dataPin, clockPin, LSBFIRST, data);
//	shiftOut(dataPin, clockPin, LSBFIRST, data >> 8);
//	digitalWrite(latchPin, HIGH);

//	Serial.println(data);

//	freq_cnt = (freq_cnt + 1) % freq_len;
//	if(freq_cnt == (freq_len-1)){

	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, MSBFIRST, data);
	shiftOut(dataPin, clockPin, MSBFIRST, data >> 8);
	shiftOut(dataPin, clockPin, MSBFIRST, data >> 16);
	shiftOut(dataPin, clockPin, MSBFIRST, data >> 24);
	digitalWrite(latchPin, HIGH);
	row_counter = (row_counter + 1) % 8;
//	}
}

void upd_ui(){
//	if(check_ui){
	led_next_row();
//	Serial.println("IRQ");		
//		check_ui = false;	
//	}	
}




void setup() {
  //set pins as output
	  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
	check_ui = false;
	row_counter = 0;
	ui_timer.begin(upd_ui, 2000);

	freq_len = 2;
	freq_cnt = 0;
}

void set_test1(){
//	uint32_t x = 61936; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
//	uint32_t x = 496; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
	uint32_t x = 0x1F00;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, x);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 8);
	digitalWrite(latchPin, HIGH);
	delay(500);
}

void set_test2(){
//	uint32_t x = 61936; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
//	uint32_t x = 496; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
	uint32_t x = 0xF100;
//	uint32_t x = 272;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, x);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 8);
	digitalWrite(latchPin, HIGH);
	delay(50);
}
void set_test3(){
//	uint32_t x = 61936; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
//	uint32_t x = 496; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
	uint32_t x = 0xffff0000; // gr: off 24
//	uint32_t x = 0xf00f0000; // gr: off 24
//	uint32_t x = 0x00;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, x);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 8);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 16);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 24);
	digitalWrite(latchPin, HIGH);
	delay(500);
}

void set_test4(){
//	uint32_t x = 61936; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
//	uint32_t x = 496; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
	byte x = 0x7;
//	uint32_t x = 0x00;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, x);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 8);
	digitalWrite(latchPin, HIGH);
//	delay(50);
}



void set_led_x(int x){
	uint32_t dataToSend = (1 << ((x/NR_ROW)+4)) | (0xf & ~(1 << (x%NR_ROW)));
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, (byte)(0));
	shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
	digitalWrite(latchPin, HIGH);
}

void set_led_x_(int x){
	byte dataToSend = (1 << ((x/NR_ROW)+4)) | (0xf & (1 << (x%NR_ROW)));
	Serial.println(dataToSend);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
	digitalWrite(latchPin, HIGH);
}

void _set_led_x_(int x){
	byte dataToSend = ((1 << ((x/NR_ROW)+4)) | (0xf & (1 << (x%NR_ROW)))) << 8;
//	byte dataToSend = (1 << ((x/NR_ROW))) | (0xf & (1 << (x%NR_ROW) +4));
	Serial.println(dataToSend);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
	digitalWrite(latchPin, HIGH);
}

void set_led(int x){
	uint16_t dataToSend = (1 << ((x/NR_ROW)+4)) | (0xf & ~(1 << (x%NR_ROW)));
	Serial.print(x);
	Serial.print(" => ");
	Serial.println(dataToSend);
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, (byte)dataToSend);
	shiftOut(dataPin, clockPin, LSBFIRST, (byte)(dataToSend >> 8));
	digitalWrite(latchPin, HIGH);
}

void test2(){
	for(int i=0;i<16;i++){
		set_led_x_(i);
      		delay(800);//wait
		all_leds();
	}
}

void test3(){
	for(int i=0;i<32;i++){
		set_led(i);
      		delay(100);//wait
		
	}
}

void all_leds(){
//	byte dataToSend = (0xf << ((x/NR_ROW)+4)) | (0xf & ~(0xf << (x%NR_ROW)));
//	byte dataToSend = (0xf << (4));
	byte dataToSend = 0xFF;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
	digitalWrite(latchPin, HIGH);
      //		delay(400);//wait

}

void test1(){
  
  for (i=0;i<4;i++){
    
    for (j=0;j<4;j++){
      
      //bit manipulation (more info at http://arduino.cc/en/Reference/Bitshift ,  http://arduino.cc/en/Reference/BitwiseXorNot , and http://arduino.cc/en/Reference/BitwiseAnd)
      dataToSend = (1 << (i+4)) | (15 & ~(1 << j));//preprare byte (series of 8 bits) to send to 74HC595
      //for example when i =2, j = 3,
      //dataToSend = (1 << 6) | (15 & ~(1 << 3));
      //dataToSend = 01000000 | (15 & ~(00001000));
      //dataToSend = 01000000 | (15 & 11110111);
      //dataToSend = 01000000 | (15 & 11110111);
      //dataToSend = 01000000 | 00000111;
      //dataToSend = 01000111;
      //the first four bits of dataToSend go to the four rows (anodes) of the LED matrix- only one is set high and the rest are set to ground
      //the last four bits of dataToSend go to the four columns (cathodes) of the LED matrix- only one is set to ground and the rest are high
      //this means that going through i = 0 to 3 and j = 0 to three with light up each led once
      
      // setlatch pin low so the LEDs don't change while sending in bits
      digitalWrite(latchPin, LOW);
      // shift out the bits of dataToSend to the 74HC595
      shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
      //set latch pin high- this sends data to outputs so the LEDs will light up
      digitalWrite(latchPin, HIGH);
      
      delay(500);//wait
    }
  }
 
}


int x;
void loop() {
//	test2(); 
//	all_leds();
//	set_test1();
//	set_test3();
//	set_test4();
//	if(x==16)
//		x=0;	
//	set_led_x(x);
//	x++;
//	all_leds();
	Serial.println("ouaich");
	for(int i=0;i<64;i++){
		red[i/8] = (1<<(i%8));
		delay(100);
	}
//	delay(100);
//	freq_len = (freq_len + 1) % 5;
	freq_len++;
	if(freq_len == 5) freq_len = 1;
}

