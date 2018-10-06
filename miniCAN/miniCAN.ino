#include <mcp_can.h>
#include <SPI.h>

#define CALIBRATE		0
#define SENSOR_PIN		A1	// select the input pin for the potentiometer
#define INTERRUPT_PIN	3	// select the input pin for interrupt

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
#define SPI_CS_PIN		10

MCP_CAN CAN(SPI_CS_PIN);	// Set CS pin

typedef enum phase_vie {
  STANDBY = 0,
  NORMAL_MODE = 1,
  GOING_TO_STANDBY = 2,
  ACTIVATION = 3,
  COM_OFF = 4,
  MAX_PHASE_VIE_VALUE
} phase_vie_enum;

phase_vie_enum phaseVieVaraible = MAX_PHASE_VIE_VALUE;

int old_button = MAX_PHASE_VIE_VALUE;
int pressed_button = MAX_PHASE_VIE_VALUE;
volatile bool buttonPressed = false;
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);      // open the serial port at 115200 bps:

	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), my_interrupt_handler, FALLING);
	while (CAN_OK != CAN.begin(CAN_125KBPS))		// init can bus : baudrate = 500k
	{
		Serial.println("CAN BUS Shield init fail");
		Serial.println(" Init CAN BUS Shield again");
		delay(100);
	}
	Serial.println("CAN BUS Shield init ok!");
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
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

int buttonDetection()
{
	int button = MAX_PHASE_VIE_VALUE;
	int buttonAnalogValue = 0;
	int analogValue = 0;
	for (int i = 0; i < 10; i++)
	{
		analogValue = analogRead(SENSOR_PIN);
		if (analogValue > 600)
			return MAX_PHASE_VIE_VALUE;
		buttonAnalogValue += analogValue;
//		Serial.println(analogValue);
		delay(5);
	}
	buttonAnalogValue /= 10;
//	Serial.print("Avg: "); Serial.println(buttonAnalogValue);

	buttonPressed = false;
#if (CALIBRATE == 1)
	calibrate(buttonAnalogValue);
	return;
#endif

//	Serial.print("Button analog value: "); Serial.println(buttonAnalogValue);

	if (buttonAnalogValue < 1010)
	{	
		if (buttonAnalogValue > 600) button = MAX_PHASE_VIE_VALUE;
		else if (buttonAnalogValue > 116 && buttonAnalogValue < 156) button = STANDBY;
		else if (buttonAnalogValue > 216 && buttonAnalogValue < 256) button = NORMAL_MODE;
		else if (buttonAnalogValue > 297 && buttonAnalogValue < 337) button = GOING_TO_STANDBY;
		else if (buttonAnalogValue > 361 && buttonAnalogValue < 401) button = ACTIVATION;
		else if (buttonAnalogValue > 416 && buttonAnalogValue < 456) button = COM_OFF;
		else button = MAX_PHASE_VIE_VALUE;

		if (old_button != button) {
			old_button = button;
			pressed_button = button;
			Serial.println(pressed_button);
		}

		return button;
	}
}

void loop() {
	// put your main code here, to run repeatedly:
	if (buttonPressed)
		phaseVieVaraible = buttonDetection();

	if (phaseVieVaraible < 5)
		stmp[4] = phaseVieVaraible;
	
	if ((phaseVieVaraible < 5) && (phaseVieVaraible > 0))
		CAN.sendMsgBuf(0x36, 0, 8, stmp);
	delay(100); // send data every 100ms
	for(int i=0; i<5; i++)
	{
		digitalWrite(5+i, LOW);
	}
	digitalWrite(5+phaseVieVaraible, HIGH);
}

void my_interrupt_handler()
{
	static unsigned long last_interrupt_time = 0;
	unsigned long interrupt_time = millis();
	// If interrupts come faster than 200ms, assume it's a bounce and ignore
	if (interrupt_time - last_interrupt_time > 200) 
	{
		Serial.println("Button pressed.");
		buttonPressed = true;	// use this flag in mainloop to trigger analo reading.
	}
	last_interrupt_time = interrupt_time;
}
