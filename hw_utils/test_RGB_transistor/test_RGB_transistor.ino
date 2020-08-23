const int red_light_pin = 23;
const int green_light_pin = 22;
const int blue_light_pin = 21;

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
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  RGB_color(0, 0 ,0);
}

int pwm_val = 0;
void loop() 
{
  RGB_color(pwm_val,pwm_val,pwm_val);
  pwm_val += 5;
  if(pwm_val > 255)
    pwm_val = 0;

  delay(100);
}
