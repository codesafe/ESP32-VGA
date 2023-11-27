
//#include <esp32/spiram.h>
//#include <esp_himem.h>
#include "./Tools/Log.h"
#include "./VGA/ESP32S3VGA.h"
//#include <Ressources/Font6x8.h>
#include "./AppleII/Apple2Machine.h"


const int hsyncPin = 23;
const int vsyncPin = 15;

//pin configuration
/*
const int redPins[] = {32, 33, 22, 21, 27};
const int greenPins[] = {14, 12, 13, 2, 4};
const int bluePins[] = {16, 17, 5, 18}; // 19
VGA14Bit vga;
*/

const int redPins[] = {32, 27};
const int greenPins[] = {14, 4};
const int bluePins[] = {16, 18}; // 19
const PinConfig pins(4,5,6,7,8,  9,10,11,12,13,14,  15,16,17,18,21,  1,2);

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
	//unsigned char color[] = { vga.RGB(255, 0, 0),  vga.RGB(0, 255, 0), vga.RGB(0, 0, 255) };
	vga.clear(0);

	long long p = 17050;// *1.2f;
	machine.Run((int)p);

	//delay(1000);
	//vga.fillRect(0, 0, vga.xres, vga.yres, color[colorindex]);
	//colorindex = colorindex + 1 >= 3 ? 0 : colorindex+1;
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

