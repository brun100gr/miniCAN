// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
#include <SPI.h>

typedef enum phase_vie {
  STANDBY = 0,
  NORMAL_MODE = 1,
  GOING_TO_STANDBY = 2,
  ACTIVATION = 3,
  COM_OFF = 4
} phase_vie_enum;

phase_vie_enum phase_vie_varaible = STANDBY;

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);   // init logging serial

    while (CAN_OK != CAN.begin(CAN_125KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop()
{
  stmp[4] = phase_vie_varaible;
  CAN.sendMsgBuf(0x36, 0, 8, stmp);
  delay(100); // send data per 100ms
}

