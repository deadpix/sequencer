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

void setup() {
  //set pins as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void set_test(){
	uint32_t x = 61936; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
//	uint32_t x = 496; // blue led!!! 272: 0x V-V-V-V gr-gr-gr-gr B-B-B-B - - - -
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, x);
	shiftOut(dataPin, clockPin, LSBFIRST, x >> 8);
	digitalWrite(latchPin, HIGH);
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
	byte dataToSend = 0xff;
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, dataToSend);
	digitalWrite(latchPin, HIGH);
      		delay(400);//wait

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


void loop() {
//	test2(); 
//	all_leds();
	set_test();
	delay(100);
}

