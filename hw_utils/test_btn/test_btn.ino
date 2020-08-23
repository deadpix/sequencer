const int buttonPin = 17;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

 if (buttonState == HIGH) {
  		Serial.println("pushed");
	}
else {
	Serial.println("released");
}
	delay(20);
}
