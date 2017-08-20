#define gout0	17
#define gout1	16
#define gout2	15
#define gout3	14

void setup() {
	Serial.begin(9600);
	pinMode(gout0, OUTPUT);
	pinMode(gout1, OUTPUT);
	pinMode(gout2, OUTPUT);
	pinMode(gout3, OUTPUT);
	digitalWrite(gout0, LOW);
	digitalWrite(gout1, LOW);
	digitalWrite(gout2, LOW);
	digitalWrite(gout3, LOW);
}

void loop() {
	digitalWrite(gout0, HIGH);
	digitalWrite(gout1, HIGH);
	digitalWrite(gout2, HIGH);
	digitalWrite(gout3, HIGH);
	
	delay(1000);
}

