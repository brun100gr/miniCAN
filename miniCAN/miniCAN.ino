int old_button = 0;
int pressed_button = 0;
//int sensorPin = A1;    // select the input pin for the potentiometer
#define SENSOR_PIN  A1 // select the input pin for the potentiometer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // open the serial port at 115200 bps:
}

void loop() {
  // put your main code here, to run repeatedly:
  int button = 0;
  int buttonAnalogValue = analogRead(SENSOR_PIN);
  // Serial.print("Analog value: "); Serial.println(buttonAnalogValue); // debug log
  if (buttonAnalogValue > 1021) button = 0;
  else if (buttonAnalogValue > 838 && buttonAnalogValue < 868) button = 1;
  else if (buttonAnalogValue > 803 && buttonAnalogValue < 833) button = 2;
  else if (buttonAnalogValue > 752 && buttonAnalogValue < 782) button = 3;
  else if (buttonAnalogValue > 666 && buttonAnalogValue < 696) button = 4;
  else if (buttonAnalogValue > 494 && buttonAnalogValue < 524) button = 5;
  else button = 0;

  if (old_button != button) {
    old_button = button;
    pressed_button = button;
    Serial.println(pressed_button);
  }
}

