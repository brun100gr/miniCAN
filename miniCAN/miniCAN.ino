#define CALIBRATE 0
#define SENSOR_PIN  A1 // select the input pin for the potentiometer

int old_button = 0;
int pressed_button = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);      // open the serial port at 115200 bps:
}

#if (CALIBRATE == 1)
int samplesCounter = 0;
long samplesSum = 0;
bool stopLogging = false;

int round_(double x)
{
	double t;

	if (!isfinite(x))
		return (x);

	if (x >= 0.0) {
		t = floor(x);
		if (t - x <= -0.5)
				t += 1.0;
		return ((int)t);
	} else {
		t = floor(-x);
		if (t + x <= -0.5)
				t += 1.0;
		return ((int)-t);
	}
}

void calibrate(int buttonAnalogValue) {
	if (buttonAnalogValue < 1010)
	{
		if (samplesCounter < 100)
		{
			Serial.print(samplesCounter);
			Serial.print(" ");
			samplesSum += buttonAnalogValue;
			samplesCounter++;
		}
		else
		{
			if (!stopLogging)
			{
				Serial.print("\nTotal: ");
				Serial.print(samplesSum);
				Serial.print(" - Avg: (");
				Serial.print(round_((double)samplesSum/100.0 - 20));
				Serial.print(") ");
				Serial.print(samplesSum/100.0);
				Serial.print(" (");
				Serial.print(round_((double)samplesSum/100.0 + 20));
				Serial.println(")\n");
				stopLogging = true;
			}
		}
	}
	else
	{
		samplesCounter = 0;
		samplesSum = 0;
		stopLogging = false;
	}
	delay(50);
}
#endif

void loop() {
	// put your main code here, to run repeatedly:
	int button = 0;
	int buttonAnalogValue = analogRead(SENSOR_PIN);

#if (CALIBRATE == 1)
	calibrate(buttonAnalogValue);
	return;
#endif

	if (buttonAnalogValue < 1010)
	{	
		if (buttonAnalogValue > 1021) button = 0;
		else if (buttonAnalogValue > 542 && buttonAnalogValue < 582) button = 1;
		else if (buttonAnalogValue > 492 && buttonAnalogValue < 532) button = 2;
		else if (buttonAnalogValue > 431 && buttonAnalogValue < 471) button = 3;
		else if (buttonAnalogValue > 351 && buttonAnalogValue < 391) button = 4;
		else if (buttonAnalogValue > 246 && buttonAnalogValue < 286) button = 5;
		else button = 0;

		if (old_button != button) {
			old_button = button;
			pressed_button = button;
			Serial.println(pressed_button);
		}
	}
}
