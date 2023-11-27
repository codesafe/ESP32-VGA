
#include "./Tools/Log.h"
#include "./VGA/ESP32S3VGA.h"
#include "./AppleII/Apple2Machine.h"

//pin configuration
#define PIN_NULL	0
#define PIN_R0	1
#define PIN_R1	2
#define PIN_R2	3

#define PIN_G0	4
#define PIN_G1	5
#define PIN_G2	6

#define PIN_B0	7
#define PIN_B1	8

#define HSYNC	23
#define VSYNC	15

const PinConfig pins(
PIN_R0, PIN_R1, PIN_R2, PIN_NULL, PIN_NULL,
PIN_G0, PIN_G1, PIN_G2, PIN_NULL, PIN_NULL, PIN_NULL,
PIN_B0, PIN_B1, PIN_NULL, PIN_NULL, PIN_NULL,
HSYNC, VSYNC);

VGA vga;
Apple2Machine machine;

//initial setup
void setup()
{
	Serial.begin(115200);
	if(psramInit())
		Serial.println("\nPSRAM is correctly initialized");
	else
		Serial.println("PSRAM not available");

	DEBUG_PRINTLN("MODE320x200");
	Mode mode = Mode::MODE_320x200x70;
	if(!vga.init(pins, mode, 8, 2)) while(1) delay(1);

	DEBUG_PRINTLN("INIT Machine");
	machine.InitMachine();
}

int colorindex = 0;
unsigned long heapCheckMillis = 0;
unsigned long memlast = 0;

void loop()
{
	vga.clear(0);

	long long p = 17050;// *1.2f;
	machine.Run((int)p);

	//delay(1000);
	vga.show();

	if(millis() - heapCheckMillis > 1000)
	{
		unsigned int memcurr = ESP.getFreeHeap();
		Serial.printf("FREEHeap: %d; DIFF %d\n", memcurr, memcurr - memlast);
		memlast = memcurr;
		heapCheckMillis = millis();

		Serial.printf("Total heap: %d\n", ESP.getHeapSize());
		Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
		Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
		Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

	}

}

