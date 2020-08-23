const int SOFT_POT_PIN = 17; // Pin connected to softpot wiper

const int GRAPH_LENGTH = 40; // Length of line graph

const int red_light_pin = 23;
const int green_light_pin = 22;
const int blue_light_pin = 21;

// 1.5kohm seems ok when softpot is not touched
const uint16_t touch_threshold = 1;
static uint16_t prev_value;

static void RGB_color(int red_light_value, int green_light_value, int blue_light_value){
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}


// need calibration?  - resistor 10K on middle pin
//                    - ~1.5K on pin connecting to the ground
uint16_t min_val = 136;
uint16_t softpot_range = 1023 - min_val;
uint16_t half_range = softpot_range / 2;

void setup() 
{
  Serial.begin(9600);
  pinMode(SOFT_POT_PIN, INPUT);
  prev_value = 0;
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  RGB_color(0, 0 ,0);
}

void loop() 
{
  // Read in the soft pot's ADC value
  int softPotADC = analogRead(SOFT_POT_PIN);

  if(softPotADC >= min_val){

  // did we touched softpot membrane?
//  if( (softPotADC - touch_threshold) > 0 ){

    uint16_t sf_val = softPotADC - min_val;
    if(sf_val < half_range){
      int blue_val = map(half_range - sf_val, 0, half_range, 0, 255);
      int green_val = map(sf_val, 0, half_range, 0, 255);
      RGB_color(0, green_val, blue_val);
      Serial.print("blue ");
      Serial.print(blue_val);
      Serial.print(" green ");
      Serial.println(green_val);

    } else {
      sf_val -= half_range;
      int green_val = map(half_range - sf_val, 0, half_range, 0, 255);
      int red_val = map(sf_val, 0, half_range, 0, 255);
      RGB_color(red_val, green_val, 0);
      Serial.print("red ");
      Serial.print(red_val);
      Serial.print(" green ");
      Serial.println(green_val);

    }


    // Map the 0-1023 value to 0-40
      int softPotPosition = map(softPotADC, min_val, 1023, 0, GRAPH_LENGTH);
      // Print a line graph:
      Serial.print("<"); // Starting end
      for (int i=0; i<GRAPH_LENGTH; i++)
      {
        if (i == softPotPosition) Serial.print("|");
        else Serial.print("-");
      }
      Serial.println("> (" + String(softPotADC) /*+ " - " + String(digitalRead(SOFT_POT_PIN))*/ + ")");
    }


//  }


  delay(50);
}
