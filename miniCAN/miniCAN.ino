int old_button = 0;
int button;
int pressed_button;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // open the serial port at 115200 bps:
  pinMode(A5, INPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonAnalogValue = analogRead(A5);
  if (buttonAnalogValue > 1021) button = 0;
  else if (buttonAnalogValue > 511 && buttonAnalogValue < 514) button = 1;
  else if (buttonAnalogValue > 680 && buttonAnalogValue < 684) button = 2;
  else if (buttonAnalogValue > 766 && buttonAnalogValue < 770) button = 3;
  else if (buttonAnalogValue > 817 && buttonAnalogValue < 822) button = 4;
  else if (buttonAnalogValue > 851 && buttonAnalogValue < 856) button = 5;
  else if (buttonAnalogValue > 875 && buttonAnalogValue < 880) button = 6;
  else button = 0;

  if (old_button == button) {
    old_button = button;
    pressed_button = 0;
  }else{
    old_button = button;
    pressed_button = button;
  }
  Serial.println(pressed_button);
}

