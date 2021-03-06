EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "Sequencepter"
Date "2020-05-03"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L sequenception_hw-rescue:Teensy3.2-teensy U2
U 1 1 5EAEC0DC
P 5200 3150
F 0 "U2" H 5200 4787 60  0000 C CNN
F 1 "Teensy3.2-teensy" H 5200 4681 60  0000 C CNN
F 2 "teensy:Teensy_32_test" H 5200 2400 60  0001 C CNN
F 3 "" H 5200 2400 60  0000 C CNN
	1    5200 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 5EAF0797
P 3000 3300
F 0 "R4" V 2793 3300 50  0000 C CNN
F 1 "47" V 2884 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2930 3300 50  0001 C CNN
F 3 "~" H 3000 3300 50  0001 C CNN
	1    3000 3300
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5EAF1362
P 2250 2150
F 0 "R2" H 2320 2196 50  0000 L CNN
F 1 "47" H 2320 2105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2180 2150 50  0001 C CNN
F 3 "~" H 2250 2150 50  0001 C CNN
	1    2250 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector:DIN-5_180degree J6
U 1 1 5EAF32B6
P 2550 3400
F 0 "J6" H 2550 3125 50  0000 C CNN
F 1 "MIDI_OUT" H 2550 3034 50  0000 C CNN
F 2 "Connectors:SDS-50J" H 2550 3400 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 2550 3400 50  0001 C CNN
	1    2550 3400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2250 3300 2250 2300
$Comp
L power:GND #PWR010
U 1 1 5EAF8D7F
P 2550 3000
F 0 "#PWR010" H 2550 2750 50  0001 C CNN
F 1 "GND" H 2555 2827 50  0000 C CNN
F 2 "" H 2550 3000 50  0001 C CNN
F 3 "" H 2550 3000 50  0001 C CNN
	1    2550 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2550 3100 2550 3000
$Comp
L Isolator:6N138 U1
U 1 1 5EAF9FA3
P 2350 1100
F 0 "U1" H 2350 1567 50  0000 C CNN
F 1 "6N138" H 2350 1476 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 2640 800 50  0001 C CNN
F 3 "http://www.onsemi.com/pub/Collateral/HCPL2731-D.pdf" H 2640 800 50  0001 C CNN
	1    2350 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector:DIN-5_180degree J2
U 1 1 5EAFC4FD
P 1000 1050
F 0 "J2" H 1000 775 50  0000 C CNN
F 1 "MIDI_IN" H 1000 684 50  0000 C CNN
F 2 "Connectors:SDS-50J" H 1000 1050 50  0001 C CNN
F 3 "http://www.mouser.com/ds/2/18/40_c091_abd_e-75918.pdf" H 1000 1050 50  0001 C CNN
	1    1000 1050
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5EB02064
P 1450 950
F 0 "R1" V 1243 950 50  0000 C CNN
F 1 "220" V 1334 950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 1380 950 50  0001 C CNN
F 3 "~" H 1450 950 50  0001 C CNN
	1    1450 950 
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4148 D1
U 1 1 5EB03CB9
P 1600 1100
F 0 "D1" V 1554 1180 50  0000 L CNN
F 1 "1N4148" V 1645 1180 50  0000 L CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 1600 925 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 1600 1100 50  0001 C CNN
	1    1600 1100
	0    1    1    0   
$EndComp
Wire Wire Line
	700  950  550  950 
Wire Wire Line
	550  950  550  1550
Wire Wire Line
	550  1550 1600 1550
Wire Wire Line
	1600 1550 1600 1250
Wire Wire Line
	1600 950  2050 950 
Wire Wire Line
	2050 950  2050 1000
Connection ~ 1600 950 
Wire Wire Line
	1600 1550 2050 1550
Wire Wire Line
	2050 1550 2050 1200
Connection ~ 1600 1550
$Comp
L power:GND #PWR020
U 1 1 5EB08DC7
P 4250 1200
F 0 "#PWR020" H 4250 950 50  0001 C CNN
F 1 "GND" H 4250 1050 50  0000 C CNN
F 2 "" H 4250 1200 50  0001 C CNN
F 3 "" H 4250 1200 50  0001 C CNN
	1    4250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5EB091EE
P 4200 1800
F 0 "#PWR016" H 4200 1550 50  0001 C CNN
F 1 "GND" H 4205 1627 50  0000 C CNN
F 2 "" H 4200 1800 50  0001 C CNN
F 3 "" H 4200 1800 50  0001 C CNN
	1    4200 1800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5EB09822
P 2650 1300
F 0 "#PWR013" H 2650 1050 50  0001 C CNN
F 1 "GND" H 2655 1127 50  0000 C CNN
F 2 "" H 2650 1300 50  0001 C CNN
F 3 "" H 2650 1300 50  0001 C CNN
	1    2650 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1200 3000 1200
Wire Wire Line
	3000 1200 3000 1450
Wire Wire Line
	3000 1900 4200 1900
$Comp
L power:+5V #PWR019
U 1 1 5EB0B0B0
P 4250 850
F 0 "#PWR019" H 4250 700 50  0001 C CNN
F 1 "+5V" H 4265 1023 50  0000 C CNN
F 2 "" H 4250 850 50  0001 C CNN
F 3 "" H 4250 850 50  0001 C CNN
	1    4250 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 850  4250 900 
$Comp
L Device:CP1 C1
U 1 1 5EB0EA89
P 4250 1050
F 0 "C1" H 4365 1096 50  0000 L CNN
F 1 "0.1uF" H 4365 1005 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.00mm" H 4250 1050 50  0001 C CNN
F 3 "~" H 4250 1050 50  0001 C CNN
	1    4250 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5EB11557
P 3350 1450
F 0 "R5" V 3557 1450 50  0000 C CNN
F 1 "470" V 3466 1450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3280 1450 50  0001 C CNN
F 3 "~" H 3350 1450 50  0001 C CNN
	1    3350 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 1450 3000 1450
Connection ~ 3000 1450
Wire Wire Line
	3000 1450 3000 1900
NoConn ~ 2650 1000
NoConn ~ 1300 1050
NoConn ~ 700  1050
NoConn ~ 2250 3400
NoConn ~ 2850 3400
NoConn ~ 1000 750 
Text Label 6200 2900 0    50   ~ 0
GREEN
Text Label 6200 2800 0    50   ~ 0
BLUE
$Comp
L Device:LED_CRGB D2
U 1 1 5EB14BC1
P 7650 1150
F 0 "D2" H 7650 683 50  0000 C CNN
F 1 "LED_CRGB" H 7650 774 50  0000 C CNN
F 2 "teensy:LED_D5.0mm-4_RGB_common-cathode" H 7650 1100 50  0001 C CNN
F 3 "~" H 7650 1100 50  0001 C CNN
	1    7650 1150
	-1   0    0    1   
$EndComp
Text Label 6900 950  0    50   ~ 0
BLUE
Text Label 6900 1150 0    50   ~ 0
GREEN
Text Label 6900 1350 0    50   ~ 0
RED
$Comp
L Device:R R8
U 1 1 5EB1BDCC
P 7300 1350
F 0 "R8" V 7200 1350 50  0000 C CNN
F 1 "47" V 7300 1350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7230 1350 50  0001 C CNN
F 3 "~" H 7300 1350 50  0001 C CNN
	1    7300 1350
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5EB1C1D1
P 7300 950
F 0 "R6" V 7200 950 50  0000 C CNN
F 1 "47" V 7300 950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7230 950 50  0001 C CNN
F 3 "~" H 7300 950 50  0001 C CNN
	1    7300 950 
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5EB1C8B0
P 7300 1150
F 0 "R7" V 7200 1150 50  0000 C CNN
F 1 "47" V 7300 1150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7230 1150 50  0001 C CNN
F 3 "~" H 7300 1150 50  0001 C CNN
	1    7300 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 950  6900 950 
Wire Wire Line
	7150 1150 6900 1150
Wire Wire Line
	7150 1350 6900 1350
$Comp
L Device:R R11
U 1 1 5EB22FF9
P 9200 1250
F 0 "R11" V 9100 1250 50  0000 C CNN
F 1 "R" V 9200 1250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9130 1250 50  0001 C CNN
F 3 "~" H 9200 1250 50  0001 C CNN
	1    9200 1250
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 5EB23D36
P 9200 1550
F 0 "R12" V 9100 1550 50  0000 C CNN
F 1 "R" V 9200 1550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 9130 1550 50  0001 C CNN
F 3 "~" H 9200 1550 50  0001 C CNN
	1    9200 1550
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 5EB2472B
P 9750 1250
F 0 "J7" H 9722 1182 50  0000 R CNN
F 1 "SOFTPOT_CONN" H 9722 1273 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9750 1250 50  0001 C CNN
F 3 "~" H 9750 1250 50  0001 C CNN
	1    9750 1250
	-1   0    0    1   
$EndComp
Wire Wire Line
	9550 1250 9400 1250
$Comp
L power:GND #PWR036
U 1 1 5EB27EDD
P 9050 850
F 0 "#PWR036" H 9050 600 50  0001 C CNN
F 1 "GND" H 9055 677 50  0000 C CNN
F 2 "" H 9050 850 50  0001 C CNN
F 3 "" H 9050 850 50  0001 C CNN
	1    9050 850 
	-1   0    0    1   
$EndComp
Wire Wire Line
	9050 1250 9050 850 
Wire Wire Line
	9400 1250 9400 1350
Wire Wire Line
	9400 1350 8600 1350
Connection ~ 9400 1250
Wire Wire Line
	9400 1250 9350 1250
Wire Wire Line
	9550 1350 9550 1550
Wire Wire Line
	9550 1550 9350 1550
Wire Wire Line
	9050 1550 8800 1550
$Comp
L power:GND #PWR035
U 1 1 5EB2BDA7
P 8800 1550
F 0 "#PWR035" H 8800 1300 50  0001 C CNN
F 1 "GND" V 8805 1422 50  0000 R CNN
F 2 "" H 8800 1550 50  0001 C CNN
F 3 "" H 8800 1550 50  0001 C CNN
	1    8800 1550
	0    1    1    0   
$EndComp
Text Label 6200 3800 0    50   ~ 0
SP
Text Label 8700 1350 0    50   ~ 0
SP
Text Label 3750 4200 0    50   ~ 0
I2C_DATA
Text Label 3750 4300 0    50   ~ 0
I2C_CLK
$Comp
L Interface_Expansion:MCP23017_SP U3
U 1 1 5EB33E34
P 9200 3250
F 0 "U3" H 9200 4531 50  0000 C CNN
F 1 "MCP23017_SP" H 9200 4440 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 9400 2250 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf" H 9400 2150 50  0001 L CNN
	1    9200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 3850 8500 3950
Wire Wire Line
	8500 3950 8500 4050
Connection ~ 8500 3950
Wire Wire Line
	8500 4050 8500 4200
Connection ~ 8500 4050
$Comp
L power:GND #PWR033
U 1 1 5EB38E0E
P 8500 4250
F 0 "#PWR033" H 8500 4000 50  0001 C CNN
F 1 "GND" H 8505 4077 50  0000 C CNN
F 2 "" H 8500 4250 50  0001 C CNN
F 3 "" H 8500 4250 50  0001 C CNN
	1    8500 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 4350 8700 4350
Wire Wire Line
	8700 4350 8700 4200
Wire Wire Line
	8700 4200 8500 4200
Connection ~ 8500 4200
Wire Wire Line
	8500 4200 8500 4250
NoConn ~ 8500 3050
NoConn ~ 8500 3150
$Comp
L Device:R R9
U 1 1 5EB3F47A
P 8200 2300
F 0 "R9" H 8100 2300 50  0000 C CNN
F 1 "4.7K" H 8350 2300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8130 2300 50  0001 C CNN
F 3 "~" H 8200 2300 50  0001 C CNN
	1    8200 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R10
U 1 1 5EB3C1E3
P 8200 2700
F 0 "R10" H 8100 2700 50  0000 C CNN
F 1 "4.7K" H 8350 2700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8130 2700 50  0001 C CNN
F 3 "~" H 8200 2700 50  0001 C CNN
	1    8200 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	8500 2450 8200 2450
Wire Wire Line
	8500 2550 8200 2550
Wire Wire Line
	8200 2850 7350 2850
Connection ~ 7350 2850
Wire Wire Line
	7350 2850 7350 3350
Wire Wire Line
	8200 2150 7350 2150
Wire Wire Line
	7350 2150 7350 2850
Wire Wire Line
	8200 2450 7600 2450
Connection ~ 8200 2450
Wire Wire Line
	8200 2550 7600 2550
Connection ~ 8200 2550
Text Label 7600 2450 0    50   ~ 0
I2C_DATA
Text Label 7600 2550 0    50   ~ 0
I2C_CLK
Wire Wire Line
	8500 3350 7350 3350
$Comp
L Connector:Conn_01x09_Female J3
U 1 1 5EB4CC63
P 1050 4700
F 0 "J3" H 942 4075 50  0000 C CNN
F 1 "Conn_01x09_Female" H 942 4166 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 1050 4700 50  0001 C CNN
F 3 "~" H 1050 4700 50  0001 C CNN
	1    1050 4700
	-1   0    0    1   
$EndComp
Wire Wire Line
	1250 4300 1900 4300
Text Label 1550 4500 0    50   ~ 0
SPI_CLK
Wire Wire Line
	1250 4500 1900 4500
Wire Wire Line
	1250 4600 1900 4600
Wire Wire Line
	1250 4700 1900 4700
Wire Wire Line
	1250 4900 1900 4900
Wire Wire Line
	1250 5000 2350 5000
Text Label 1550 4400 0    50   ~ 0
LCD_BL
$Comp
L Device:R R3
U 1 1 5EB6322E
P 2450 4400
F 0 "R3" V 2350 4400 50  0000 C CNN
F 1 "100" V 2450 4400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2380 4400 50  0001 C CNN
F 3 "~" H 2450 4400 50  0001 C CNN
	1    2450 4400
	0    1    1    0   
$EndComp
Text Label 1550 4300 0    50   ~ 0
SPI_DIN
Text Label 1550 4600 0    50   ~ 0
SPI_DOUT
Text Label 1550 4700 0    50   ~ 0
DC_RS
Text Label 1550 4900 0    50   ~ 0
SPI_CS
$Comp
L power:GND #PWR09
U 1 1 5EB65606
P 2350 5000
F 0 "#PWR09" H 2350 4750 50  0001 C CNN
F 1 "GND" V 2355 4872 50  0000 R CNN
F 2 "" H 2350 5000 50  0001 C CNN
F 3 "" H 2350 5000 50  0001 C CNN
	1    2350 5000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5EB69150
P 2600 4400
F 0 "#PWR011" H 2600 4150 50  0001 C CNN
F 1 "GND" H 2605 4227 50  0000 C CNN
F 2 "" H 2600 4400 50  0001 C CNN
F 3 "" H 2600 4400 50  0001 C CNN
	1    2600 4400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1250 5100 1900 5100
Text Label 3750 3100 0    50   ~ 0
SPI_DIN
Text Label 3750 3000 0    50   ~ 0
SPI_DOUT
Text Label 3750 2900 0    50   ~ 0
SPI_CS
Text Label 3750 3700 0    50   ~ 0
SPI_CLK
$Comp
L power:+3.3V #PWR028
U 1 1 5EB83483
P 7350 3350
F 0 "#PWR028" H 7350 3200 50  0001 C CNN
F 1 "+3.3V" H 7365 3523 50  0000 C CNN
F 2 "" H 7350 3350 50  0001 C CNN
F 3 "" H 7350 3350 50  0001 C CNN
	1    7350 3350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5EB91E03
P 4200 3400
F 0 "#PWR018" H 4200 3150 50  0001 C CNN
F 1 "GND" V 4205 3272 50  0000 R CNN
F 2 "" H 4200 3400 50  0001 C CNN
F 3 "" H 4200 3400 50  0001 C CNN
	1    4200 3400
	0    1    1    0   
$EndComp
Connection ~ 7350 3350
$Comp
L power:+3.3V #PWR037
U 1 1 5EBAB838
P 9200 2150
F 0 "#PWR037" H 9200 2000 50  0001 C CNN
F 1 "+3.3V" V 9215 2278 50  0000 L CNN
F 2 "" H 9200 2150 50  0001 C CNN
F 3 "" H 9200 2150 50  0001 C CNN
	1    9200 2150
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR04
U 1 1 5EBB15AC
P 1900 4800
F 0 "#PWR04" H 1900 4650 50  0001 C CNN
F 1 "+3.3V" V 1915 4928 50  0000 L CNN
F 2 "" H 1900 4800 50  0001 C CNN
F 3 "" H 1900 4800 50  0001 C CNN
	1    1900 4800
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR08
U 1 1 5EBB17C3
P 2150 4400
F 0 "#PWR08" H 2150 4250 50  0001 C CNN
F 1 "+3.3V" H 2165 4573 50  0000 C CNN
F 2 "" H 2150 4400 50  0001 C CNN
F 3 "" H 2150 4400 50  0001 C CNN
	1    2150 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 4400 2150 4400
Connection ~ 2150 4400
Wire Wire Line
	2150 4400 2300 4400
Wire Wire Line
	1250 4800 1900 4800
Wire Wire Line
	9550 1150 8600 1150
$Comp
L power:+3.3V #PWR034
U 1 1 5EBCFD7E
P 8600 1150
F 0 "#PWR034" H 8600 1000 50  0001 C CNN
F 1 "+3.3V" V 8615 1278 50  0000 L CNN
F 2 "" H 8600 1150 50  0001 C CNN
F 3 "" H 8600 1150 50  0001 C CNN
	1    8600 1150
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR012
U 1 1 5EBDF9F2
P 2650 900
F 0 "#PWR012" H 2650 750 50  0001 C CNN
F 1 "+5V" V 2665 1028 50  0000 L CNN
F 2 "" H 2650 900 50  0001 C CNN
F 3 "" H 2650 900 50  0001 C CNN
	1    2650 900 
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2000 4200 2000
Text Notes 8050 600  0    50   ~ 0
Soft pot and RGB LED
Text Notes 8150 4600 0    50   ~ 0
Transport / menu / parameters / shift buttons
Text Notes 1850 4050 0    50   ~ 0
LCD
$Comp
L power:+3.3V #PWR014
U 1 1 5EBF1429
P 3150 3300
F 0 "#PWR014" H 3150 3150 50  0001 C CNN
F 1 "+3.3V" V 3165 3428 50  0000 L CNN
F 2 "" H 3150 3300 50  0001 C CNN
F 3 "" H 3150 3300 50  0001 C CNN
	1    3150 3300
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR015
U 1 1 5EBF1B26
P 3500 1450
F 0 "#PWR015" H 3500 1300 50  0001 C CNN
F 1 "+3.3V" V 3515 1578 50  0000 L CNN
F 2 "" H 3500 1450 50  0001 C CNN
F 3 "" H 3500 1450 50  0001 C CNN
	1    3500 1450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5EBF258D
P 7850 1150
F 0 "#PWR029" H 7850 900 50  0001 C CNN
F 1 "GND" V 7855 1022 50  0000 R CNN
F 2 "" H 7850 1150 50  0001 C CNN
F 3 "" H 7850 1150 50  0001 C CNN
	1    7850 1150
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR05
U 1 1 5EBB10E3
P 1900 5100
F 0 "#PWR05" H 1900 4950 50  0001 C CNN
F 1 "+3.3V" V 1915 5228 50  0000 L CNN
F 2 "" H 1900 5100 50  0001 C CNN
F 3 "" H 1900 5100 50  0001 C CNN
	1    1900 5100
	0    1    1    0   
$EndComp
$Sheet
S 4350 5500 1700 1000
U 5EB7F55F
F0 "Button matrix" 50
F1 "button_matrix.sch" 50
F2 "matrix_row_0" I L 4350 5600 50 
F3 "matrix_row_1" I L 4350 5700 50 
F4 "matrix_row_2" I L 4350 5800 50 
F5 "matrix_row_3" I L 4350 5900 50 
F6 "matrix_row_4" I L 4350 6000 50 
F7 "matrix_row_5" I L 4350 6100 50 
F8 "matrix_row_6" I L 4350 6200 50 
F9 "matrix_row_7" I L 4350 6300 50 
F10 "matrix_col_0" I R 6050 5600 50 
F11 "matrix_col_1" I R 6050 5700 50 
F12 "matrix_col_2" I R 6050 5800 50 
F13 "matrix_col_3" I R 6050 5900 50 
F14 "matrix_col_4" I R 6050 6000 50 
F15 "matrix_col_5" I R 6050 6100 50 
F16 "matrix_col_6" I R 6050 6200 50 
F17 "matrix_col_7" I R 6050 6300 50 
$EndSheet
Text Label 6200 2100 0    50   ~ 0
MAT_ROW_0
Text Label 6200 2500 0    50   ~ 0
MAT_ROW_4
Text Label 6200 2400 0    50   ~ 0
MAT_ROW_5
Text Label 6200 2300 0    50   ~ 0
MAT_ROW_6
Text Label 6200 4400 0    50   ~ 0
MAT_COL_0
Text Label 6200 4500 0    50   ~ 0
MAT_COL_1
Text Label 3750 4500 0    50   ~ 0
MAT_COL_2
Text Label 3750 4400 0    50   ~ 0
MAT_COL_3
Text Label 3750 4100 0    50   ~ 0
MAT_COL_4
Text Label 3750 4000 0    50   ~ 0
MAT_COL_5
Text Label 3750 3900 0    50   ~ 0
MAT_COL_6
Text Label 3750 3800 0    50   ~ 0
MAT_COL_7
Wire Wire Line
	3750 4500 4200 4500
Wire Wire Line
	3750 4400 4200 4400
Wire Wire Line
	3750 4300 4200 4300
Wire Wire Line
	3750 4200 4200 4200
Wire Wire Line
	3750 4100 4200 4100
Wire Wire Line
	3750 4000 4200 4000
Wire Wire Line
	3750 3900 4200 3900
Wire Wire Line
	3750 3800 4200 3800
Wire Wire Line
	3750 3700 4200 3700
Wire Wire Line
	3750 3100 4200 3100
Wire Wire Line
	3750 3000 4200 3000
Wire Wire Line
	3750 2900 4200 2900
Wire Wire Line
	6050 5600 6450 5600
Wire Wire Line
	6050 5700 6450 5700
Wire Wire Line
	6050 5800 6450 5800
Wire Wire Line
	6050 5900 6450 5900
Wire Wire Line
	6050 6000 6450 6000
Wire Wire Line
	6050 6100 6450 6100
Wire Wire Line
	6050 6200 6450 6200
Wire Wire Line
	6050 6300 6450 6300
Wire Wire Line
	3900 5600 4350 5600
Wire Wire Line
	3900 5700 4350 5700
Wire Wire Line
	3900 5800 4350 5800
Wire Wire Line
	3900 5900 4350 5900
Wire Wire Line
	3900 6000 4350 6000
Wire Wire Line
	3900 6100 4350 6100
Wire Wire Line
	3900 6200 4350 6200
Wire Wire Line
	3900 6300 4350 6300
Text Label 3900 5700 0    50   ~ 0
MAT_ROW_1
Text Label 3900 5800 0    50   ~ 0
MAT_ROW_2
Text Label 3900 5900 0    50   ~ 0
MAT_ROW_3
Text Label 3900 6000 0    50   ~ 0
MAT_ROW_4
Text Label 3900 6100 0    50   ~ 0
MAT_ROW_5
Text Label 3900 6200 0    50   ~ 0
MAT_ROW_6
Text Label 6050 5600 0    50   ~ 0
MAT_COL_0
Text Label 6050 5700 0    50   ~ 0
MAT_COL_1
Text Label 6050 5800 0    50   ~ 0
MAT_COL_2
Text Label 6050 5900 0    50   ~ 0
MAT_COL_3
Text Label 6050 6000 0    50   ~ 0
MAT_COL_4
Text Label 6050 6100 0    50   ~ 0
MAT_COL_5
Text Label 6050 6200 0    50   ~ 0
MAT_COL_6
Text Label 6050 6300 0    50   ~ 0
MAT_COL_7
NoConn ~ 4200 3200
NoConn ~ 4200 3500
$Comp
L Switch:SW_Push SW1
U 1 1 5EB7485C
P 10100 2450
F 0 "SW1" H 10100 2735 50  0000 C CNN
F 1 "SW_Push" H 10100 2644 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 2650 50  0001 C CNN
F 3 "~" H 10100 2650 50  0001 C CNN
	1    10100 2450
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5EB75892
P 10100 2550
F 0 "SW2" H 10100 2835 50  0000 C CNN
F 1 "SW_Push" H 10100 2744 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 2750 50  0001 C CNN
F 3 "~" H 10100 2750 50  0001 C CNN
	1    10100 2550
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 5EB75C1D
P 10100 2650
F 0 "SW3" H 10100 2935 50  0000 C CNN
F 1 "SW_Push" H 10100 2844 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 2850 50  0001 C CNN
F 3 "~" H 10100 2850 50  0001 C CNN
	1    10100 2650
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 5EB75F57
P 10100 2750
F 0 "SW4" H 10100 3035 50  0000 C CNN
F 1 "SW_Push" H 10100 2944 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 2950 50  0001 C CNN
F 3 "~" H 10100 2950 50  0001 C CNN
	1    10100 2750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW7
U 1 1 5EB774CF
P 10100 3150
F 0 "SW7" H 10100 3250 50  0000 C CNN
F 1 "SW_Push" H 10100 3100 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 3350 50  0001 C CNN
F 3 "~" H 10100 3350 50  0001 C CNN
	1    10100 3150
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW6
U 1 1 5EB7785A
P 10100 3050
F 0 "SW6" H 10100 3150 50  0000 C CNN
F 1 "SW_Push" H 10100 3000 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 3250 50  0001 C CNN
F 3 "~" H 10100 3250 50  0001 C CNN
	1    10100 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 2450 10300 2550
Connection ~ 10300 2550
Wire Wire Line
	10300 2550 10300 2650
Connection ~ 10300 2650
Wire Wire Line
	10300 2650 10300 2750
Connection ~ 10300 2750
$Comp
L power:GND #PWR038
U 1 1 5EB8592B
P 10300 4200
F 0 "#PWR038" H 10300 3950 50  0001 C CNN
F 1 "GND" H 10305 4027 50  0000 C CNN
F 2 "" H 10300 4200 50  0001 C CNN
F 3 "" H 10300 4200 50  0001 C CNN
	1    10300 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 5EB8645D
P 7950 4450
F 0 "#PWR032" H 7950 4200 50  0001 C CNN
F 1 "GND" H 7955 4277 50  0000 C CNN
F 2 "" H 7950 4450 50  0001 C CNN
F 3 "" H 7950 4450 50  0001 C CNN
	1    7950 4450
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR031
U 1 1 5EB867D6
P 7950 4150
F 0 "#PWR031" H 7950 4000 50  0001 C CNN
F 1 "+3.3V" V 7965 4278 50  0000 L CNN
F 2 "" H 7950 4150 50  0001 C CNN
F 3 "" H 7950 4150 50  0001 C CNN
	1    7950 4150
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5EB87536
P 7950 4300
F 0 "C3" H 8065 4346 50  0000 L CNN
F 1 "0.1uF" H 8065 4255 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 7988 4150 50  0001 C CNN
F 3 "~" H 7950 4300 50  0001 C CNN
	1    7950 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2100 4200 2100
Wire Wire Line
	4200 2200 3750 2200
Wire Wire Line
	4200 2300 3750 2300
Wire Wire Line
	4200 2400 3750 2400
Wire Wire Line
	4200 2500 3750 2500
Wire Wire Line
	4200 2600 3750 2600
Wire Wire Line
	4200 2700 3750 2700
Wire Wire Line
	4200 2800 3750 2800
Text Label 3750 2100 0    50   ~ 0
PIN2
Text Label 3750 2200 0    50   ~ 0
PIN3
Text Label 3750 2300 0    50   ~ 0
PIN4
Text Label 3750 2400 0    50   ~ 0
PIN5
Text Label 3750 2500 0    50   ~ 0
PIN6
Text Label 3750 2600 0    50   ~ 0
PIN7
Text Label 3750 2700 0    50   ~ 0
PIN8
Text Label 3750 2800 0    50   ~ 0
DC_RS
Text Label 1050 5800 0    50   ~ 0
PIN3
Text Label 1050 5900 0    50   ~ 0
PIN4
Text Label 1050 6000 0    50   ~ 0
PIN5
Text Label 1050 6100 0    50   ~ 0
PIN6
Text Label 1050 6200 0    50   ~ 0
PIN7
Text Label 1050 6300 0    50   ~ 0
PIN8
Wire Wire Line
	4200 3600 3750 3600
Text Label 3750 3600 0    50   ~ 0
PIN10
Text Label 6200 2600 0    50   ~ 0
PIN11
Text Label 6200 2700 0    50   ~ 0
PIN12
Text Notes 1250 5400 0    50   ~ 0
Teensy unused pins
$Comp
L Connector_Generic:Conn_02x08_Odd_Even J4
U 1 1 5EC943A1
P 1700 7350
F 0 "J4" H 1750 6725 50  0000 C CNN
F 1 "Conn_02x08_Odd_Even" H 1750 6816 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x08_P2.54mm_Vertical" H 1700 7350 50  0001 C CNN
F 3 "~" H 1700 7350 50  0001 C CNN
	1    1700 7350
	-1   0    0    1   
$EndComp
Wire Wire Line
	1900 7550 1900 7450
Wire Wire Line
	1900 7450 1900 7350
Connection ~ 1900 7450
Wire Wire Line
	1400 7350 1400 7450
Wire Wire Line
	1400 7450 1400 7550
Connection ~ 1400 7450
$Comp
L power:GND #PWR07
U 1 1 5ECEDD98
P 1900 7450
F 0 "#PWR07" H 1900 7200 50  0001 C CNN
F 1 "GND" V 1905 7322 50  0000 R CNN
F 2 "" H 1900 7450 50  0001 C CNN
F 3 "" H 1900 7450 50  0001 C CNN
	1    1900 7450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5ECEE3F0
P 1400 7450
F 0 "#PWR03" H 1400 7200 50  0001 C CNN
F 1 "GND" V 1405 7322 50  0000 R CNN
F 2 "" H 1400 7450 50  0001 C CNN
F 3 "" H 1400 7450 50  0001 C CNN
	1    1400 7450
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 5ECEF6BF
P 1900 7150
F 0 "#PWR06" H 1900 7000 50  0001 C CNN
F 1 "+5V" V 1915 7278 50  0000 L CNN
F 2 "" H 1900 7150 50  0001 C CNN
F 3 "" H 1900 7150 50  0001 C CNN
	1    1900 7150
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 5ECEFD35
P 1400 7150
F 0 "#PWR02" H 1400 7000 50  0001 C CNN
F 1 "+5V" V 1415 7278 50  0000 L CNN
F 2 "" H 1400 7150 50  0001 C CNN
F 3 "" H 1400 7150 50  0001 C CNN
	1    1400 7150
	0    -1   -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5ECFB931
P 7200 5300
F 0 "#FLG01" H 7200 5375 50  0001 C CNN
F 1 "PWR_FLAG" H 7200 5473 50  0000 C CNN
F 2 "" H 7200 5300 50  0001 C CNN
F 3 "~" H 7200 5300 50  0001 C CNN
	1    7200 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR027
U 1 1 5ECFC698
P 7200 5300
F 0 "#PWR027" H 7200 5150 50  0001 C CNN
F 1 "+5V" H 7215 5473 50  0000 C CNN
F 2 "" H 7200 5300 50  0001 C CNN
F 3 "" H 7200 5300 50  0001 C CNN
	1    7200 5300
	1    0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5ECFF2E9
P 8000 5300
F 0 "#FLG02" H 8000 5375 50  0001 C CNN
F 1 "PWR_FLAG" H 8000 5473 50  0000 C CNN
F 2 "" H 8000 5300 50  0001 C CNN
F 3 "~" H 8000 5300 50  0001 C CNN
	1    8000 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5ECFFE80
P 8000 5300
F 0 "#PWR030" H 8000 5050 50  0001 C CNN
F 1 "GND" H 8005 5127 50  0000 C CNN
F 2 "" H 8000 5300 50  0001 C CNN
F 3 "" H 8000 5300 50  0001 C CNN
	1    8000 5300
	1    0    0    -1  
$EndComp
NoConn ~ 1400 6950
NoConn ~ 1400 7050
NoConn ~ 1900 6950
NoConn ~ 1900 7050
NoConn ~ 1900 7250
NoConn ~ 1400 7250
NoConn ~ 1400 7650
NoConn ~ 1900 7650
Text Label 1050 5700 0    50   ~ 0
PIN2
Text Label 6200 3400 0    50   ~ 0
RED
Text Label 6200 3200 0    50   ~ 0
MAT_ROW_1
Text Label 6200 3100 0    50   ~ 0
MAT_ROW_2
Text Label 6200 3000 0    50   ~ 0
MAT_ROW_3
Text Label 6200 3700 0    50   ~ 0
MAT_ROW_7
$Comp
L power:GND #PWR024
U 1 1 5EC51171
P 6200 3300
F 0 "#PWR024" H 6200 3050 50  0001 C CNN
F 1 "GND" V 6205 3172 50  0000 R CNN
F 2 "" H 6200 3300 50  0001 C CNN
F 3 "" H 6200 3300 50  0001 C CNN
	1    6200 3300
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR026
U 1 1 5EC524A6
P 6200 4300
F 0 "#PWR026" H 6200 4150 50  0001 C CNN
F 1 "+3.3V" V 6215 4428 50  0000 L CNN
F 2 "" H 6200 4300 50  0001 C CNN
F 3 "" H 6200 4300 50  0001 C CNN
	1    6200 4300
	0    1    -1   0   
$EndComp
$Comp
L power:+3.3V #PWR023
U 1 1 5EC5335C
P 6200 2200
F 0 "#PWR023" H 6200 2050 50  0001 C CNN
F 1 "+3.3V" V 6215 2328 50  0000 L CNN
F 2 "" H 6200 2200 50  0001 C CNN
F 3 "" H 6200 2200 50  0001 C CNN
	1    6200 2200
	0    1    -1   0   
$EndComp
$Comp
L power:+3.3V #PWR017
U 1 1 5EC5ABCD
P 4200 3300
F 0 "#PWR017" H 4200 3150 50  0001 C CNN
F 1 "+3.3V" V 4215 3428 50  0000 L CNN
F 2 "" H 4200 3300 50  0001 C CNN
F 3 "" H 4200 3300 50  0001 C CNN
	1    4200 3300
	0    -1   1    0   
$EndComp
$Comp
L power:+5V #PWR025
U 1 1 5EC6070C
P 6200 4100
F 0 "#PWR025" H 6200 3950 50  0001 C CNN
F 1 "+5V" V 6215 4273 50  0000 C CNN
F 2 "" H 6200 4100 50  0001 C CNN
F 3 "" H 6200 4100 50  0001 C CNN
	1    6200 4100
	0    1    -1   0   
$EndComp
NoConn ~ 6200 4000
$Comp
L power:GND #PWR022
U 1 1 5EC33D1E
P 5350 4750
F 0 "#PWR022" H 5350 4500 50  0001 C CNN
F 1 "GND" H 5355 4577 50  0000 C CNN
F 2 "" H 5350 4750 50  0001 C CNN
F 3 "" H 5350 4750 50  0001 C CNN
	1    5350 4750
	0    -1   1    0   
$EndComp
$Comp
L Device:C C2
U 1 1 5EC33D32
P 5200 4750
F 0 "C2" H 5315 4796 50  0000 L CNN
F 1 "0.1uF" H 5315 4705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5238 4600 50  0001 C CNN
F 3 "~" H 5200 4750 50  0001 C CNN
	1    5200 4750
	0    -1   1    0   
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 5EC4842A
P 5050 4750
F 0 "#PWR021" H 5050 4600 50  0001 C CNN
F 1 "+5V" V 5065 4923 50  0000 C CNN
F 2 "" H 5050 4750 50  0001 C CNN
F 3 "" H 5050 4750 50  0001 C CNN
	1    5050 4750
	0    -1   1    0   
$EndComp
Text Label 3900 6300 0    50   ~ 0
MAT_ROW_7
Text Label 3900 5600 0    50   ~ 0
MAT_ROW_0
$Comp
L Connector:Conn_01x08_Male J8
U 1 1 5ECCF270
P 10350 5350
F 0 "J8" H 10322 5232 50  0000 R CNN
F 1 "Conn_01x08_Male" H 10322 5323 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 10350 5350 50  0001 C CNN
F 3 "~" H 10350 5350 50  0001 C CNN
	1    10350 5350
	-1   0    0    1   
$EndComp
Wire Wire Line
	10150 4950 9750 4950
Wire Wire Line
	10150 5050 9750 5050
Wire Wire Line
	10150 5150 9750 5150
Wire Wire Line
	10150 5250 9750 5250
Wire Wire Line
	10150 5350 9750 5350
Wire Wire Line
	10150 5450 9750 5450
Wire Wire Line
	10150 5550 9750 5550
Wire Wire Line
	10150 5650 9750 5650
Wire Wire Line
	9900 3750 10150 3750
Wire Wire Line
	9900 3850 10150 3850
Wire Wire Line
	9900 3950 10150 3950
Wire Wire Line
	9900 4050 10150 4050
Text Label 10150 3750 2    50   ~ 0
GPA4
Text Label 10150 3850 2    50   ~ 0
GPA5
Text Label 9750 5350 0    50   ~ 0
GPA4
Text Label 9750 5450 0    50   ~ 0
GPA5
Text Label 10150 3950 2    50   ~ 0
GPA6
Text Label 10150 4050 2    50   ~ 0
GPA7
Text Label 9750 5550 0    50   ~ 0
GPA6
Text Label 9750 5650 0    50   ~ 0
GPA7
Wire Wire Line
	9900 3350 10200 3350
Wire Wire Line
	9900 3450 10200 3450
Wire Wire Line
	9900 3550 10200 3550
Wire Wire Line
	9900 3650 10200 3650
Text Label 10200 3350 2    50   ~ 0
GPA0
Text Label 10200 3450 2    50   ~ 0
GPA1
Text Label 10200 3550 2    50   ~ 0
GPA2
Text Label 10200 3650 2    50   ~ 0
GPA3
Text Label 9750 4950 0    50   ~ 0
GPA0
Text Label 9750 5050 0    50   ~ 0
GPA1
Text Label 9750 5150 0    50   ~ 0
GPA2
Text Label 9750 5250 0    50   ~ 0
GPA3
$Comp
L Connector:Conn_01x08_Male J1
U 1 1 5ED7F2D6
P 850 6000
F 0 "J1" H 958 6481 50  0000 C CNN
F 1 "Conn_01x08_Male" H 958 6390 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 850 6000 50  0001 C CNN
F 3 "~" H 850 6000 50  0001 C CNN
	1    850  6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5ED80839
P 1050 6400
F 0 "#PWR01" H 1050 6150 50  0001 C CNN
F 1 "GND" H 1055 6227 50  0000 C CNN
F 2 "" H 1050 6400 50  0001 C CNN
F 3 "" H 1050 6400 50  0001 C CNN
	1    1050 6400
	0    -1   1    0   
$EndComp
Wire Wire Line
	2100 6350 2600 6350
Wire Wire Line
	2100 6250 2600 6250
Wire Wire Line
	2100 6150 2600 6150
$Comp
L Connector:Conn_01x03_Male J5
U 1 1 5ED69E91
P 1900 6250
F 0 "J5" H 2008 6531 50  0000 C CNN
F 1 "Conn_01x03_Male" H 2008 6440 50  0000 C CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x03_P2.00mm_Vertical" H 1900 6250 50  0001 C CNN
F 3 "~" H 1900 6250 50  0001 C CNN
	1    1900 6250
	1    0    0    -1  
$EndComp
Text Notes 1500 6650 0    50   ~ 0
power in
Text Label 2600 6250 2    50   ~ 0
PIN12
Text Label 2600 6150 2    50   ~ 0
PIN11
Text Label 2600 6350 2    50   ~ 0
PIN10
NoConn ~ 6200 4200
NoConn ~ 6200 3900
NoConn ~ -2400 2700
NoConn ~ 6550 3950
NoConn ~ 5200 1050
$Comp
L power:GND #PWR040
U 1 1 5EDF9A56
P 5150 1200
F 0 "#PWR040" H 5150 950 50  0001 C CNN
F 1 "GND" H 5155 1027 50  0000 C CNN
F 2 "" H 5150 1200 50  0001 C CNN
F 3 "" H 5150 1200 50  0001 C CNN
	1    5150 1200
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR039
U 1 1 5EDF9A5C
P 5150 900
F 0 "#PWR039" H 5150 750 50  0001 C CNN
F 1 "+3.3V" V 5165 1028 50  0000 L CNN
F 2 "" H 5150 900 50  0001 C CNN
F 3 "" H 5150 900 50  0001 C CNN
	1    5150 900 
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5EDF9A62
P 5150 1050
F 0 "C4" H 5265 1096 50  0000 L CNN
F 1 "0.1uF" H 5265 1005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5188 900 50  0001 C CNN
F 3 "~" H 5150 1050 50  0001 C CNN
	1    5150 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5EDFEFEA
P 5500 1200
F 0 "#PWR042" H 5500 950 50  0001 C CNN
F 1 "GND" H 5505 1027 50  0000 C CNN
F 2 "" H 5500 1200 50  0001 C CNN
F 3 "" H 5500 1200 50  0001 C CNN
	1    5500 1200
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR041
U 1 1 5EDFEFF0
P 5500 900
F 0 "#PWR041" H 5500 750 50  0001 C CNN
F 1 "+3.3V" V 5515 1028 50  0000 L CNN
F 2 "" H 5500 900 50  0001 C CNN
F 3 "" H 5500 900 50  0001 C CNN
	1    5500 900 
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR044
U 1 1 5EE042F5
P 5850 1200
F 0 "#PWR044" H 5850 950 50  0001 C CNN
F 1 "GND" H 5855 1027 50  0000 C CNN
F 2 "" H 5850 1200 50  0001 C CNN
F 3 "" H 5850 1200 50  0001 C CNN
	1    5850 1200
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR043
U 1 1 5EE042FB
P 5850 900
F 0 "#PWR043" H 5850 750 50  0001 C CNN
F 1 "+3.3V" V 5865 1028 50  0000 L CNN
F 2 "" H 5850 900 50  0001 C CNN
F 3 "" H 5850 900 50  0001 C CNN
	1    5850 900 
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C6
U 1 1 5EE04301
P 5850 1050
F 0 "C6" H 5965 1096 50  0000 L CNN
F 1 "0.1uF" H 5965 1005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5888 900 50  0001 C CNN
F 3 "~" H 5850 1050 50  0001 C CNN
	1    5850 1050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5EDFEFF6
P 5500 1050
F 0 "C5" H 5615 1096 50  0000 L CNN
F 1 "0.1uF" H 5615 1005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5538 900 50  0001 C CNN
F 3 "~" H 5500 1050 50  0001 C CNN
	1    5500 1050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW5
U 1 1 5EB76E86
P 10100 2950
F 0 "SW5" H 10100 3050 50  0000 C CNN
F 1 "SW_Push" H 10100 2900 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 3150 50  0001 C CNN
F 3 "~" H 10100 3150 50  0001 C CNN
	1    10100 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 2750 10300 2850
$Comp
L Switch:SW_Push SW8
U 1 1 5EB77ABC
P 10100 2850
F 0 "SW8" H 10100 2950 50  0000 C CNN
F 1 "SW_Push" H 10100 2800 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 10100 3050 50  0001 C CNN
F 3 "~" H 10100 3050 50  0001 C CNN
	1    10100 2850
	1    0    0    -1  
$EndComp
Connection ~ 10300 2850
Wire Wire Line
	10300 2850 10300 2950
Connection ~ 10300 2950
Wire Wire Line
	10300 2950 10300 3050
Connection ~ 10300 3050
Wire Wire Line
	10300 3050 10300 3150
Connection ~ 10300 3150
Wire Wire Line
	10300 3150 10300 4200
$Comp
L Mechanical:MountingHole H4
U 1 1 5EEFDB9A
P 3250 7250
F 0 "H4" H 3150 7296 50  0000 R CNN
F 1 "MountingHole" H 3150 7205 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 7250 50  0001 C CNN
F 3 "~" H 3250 7250 50  0001 C CNN
	1    3250 7250
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 5EF0405F
P 3250 7450
F 0 "H5" H 3150 7496 50  0000 R CNN
F 1 "MountingHole" H 3150 7405 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 7450 50  0001 C CNN
F 3 "~" H 3250 7450 50  0001 C CNN
	1    3250 7450
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 5EF049D3
P 3250 7650
F 0 "H6" H 3150 7696 50  0000 R CNN
F 1 "MountingHole" H 3150 7605 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 7650 50  0001 C CNN
F 3 "~" H 3250 7650 50  0001 C CNN
	1    3250 7650
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5EF04B93
P 3250 7050
F 0 "H3" H 3150 7096 50  0000 R CNN
F 1 "MountingHole" H 3150 7005 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 7050 50  0001 C CNN
F 3 "~" H 3250 7050 50  0001 C CNN
	1    3250 7050
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5EF04F62
P 3250 6850
F 0 "H2" H 3150 6896 50  0000 R CNN
F 1 "MountingHole" H 3150 6805 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 6850 50  0001 C CNN
F 3 "~" H 3250 6850 50  0001 C CNN
	1    3250 6850
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 5EF057BD
P 3250 6650
F 0 "H1" H 3150 6696 50  0000 R CNN
F 1 "MountingHole" H 3150 6605 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 3250 6650 50  0001 C CNN
F 3 "~" H 3250 6650 50  0001 C CNN
	1    3250 6650
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H7
U 1 1 5EF05928
P 4200 6950
F 0 "H7" H 4100 6996 50  0000 R CNN
F 1 "MountingHole" H 4100 6905 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4200 6950 50  0001 C CNN
F 3 "~" H 4200 6950 50  0001 C CNN
	1    4200 6950
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H8
U 1 1 5EF05C3C
P 4200 7150
F 0 "H8" H 4100 7196 50  0000 R CNN
F 1 "MountingHole" H 4100 7105 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4200 7150 50  0001 C CNN
F 3 "~" H 4200 7150 50  0001 C CNN
	1    4200 7150
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H9
U 1 1 5EF05FC7
P 4200 7350
F 0 "H9" H 4100 7396 50  0000 R CNN
F 1 "MountingHole" H 4100 7305 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4200 7350 50  0001 C CNN
F 3 "~" H 4200 7350 50  0001 C CNN
	1    4200 7350
	-1   0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H10
U 1 1 5EF06550
P 4200 7550
F 0 "H10" H 4100 7596 50  0000 R CNN
F 1 "MountingHole" H 4100 7505 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4200 7550 50  0001 C CNN
F 3 "~" H 4200 7550 50  0001 C CNN
	1    4200 7550
	-1   0    0    -1  
$EndComp
$EndSCHEMATC
