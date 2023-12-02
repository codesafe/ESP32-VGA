#include <Arduino.h>
#include <SPIFFS.h>
#include "./Tools/Log.h"
#include "./VGA/ESP32S3VGA.h"
#include "./AppleII/Apple2Machine.h"
#include "FS.h"

//#define COLORBIT_8    true

//pin configuration
#define PIN_NULL	0

#define PIN_R0	4
#define PIN_R1	5
#define PIN_R2	6
#define PIN_R3	7
#define PIN_R4	15

#define PIN_G0	16
#define PIN_G1	17
#define PIN_G2	18
#define PIN_G3	8
#define PIN_G4	3

#define PIN_B0	10
#define PIN_B1	11
#define PIN_B2	12
#define PIN_B3	13
#define PIN_B4	14

#define HSYNC	21//48
#define VSYNC	47


const PinConfig pins(
PIN_R0, PIN_R1, PIN_R2, PIN_R3, PIN_R4,
PIN_G0, PIN_G1, PIN_NULL, PIN_G2, PIN_G3, PIN_G4,
PIN_B0, PIN_B1, PIN_B2, PIN_B3, PIN_B4,
HSYNC, VSYNC);

const PinConfig pins_8(
PIN_NULL, PIN_NULL, PIN_R0, PIN_R2, PIN_R4, // 3
PIN_NULL, PIN_NULL, PIN_NULL, PIN_G0, PIN_G2, PIN_G4, // 3
PIN_NULL, PIN_NULL, PIN_NULL, PIN_B0, PIN_B3,
HSYNC, VSYNC);


VGA *vga;
Apple2Machine *machine;

#if 1
void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
#endif

//initial setup
void setup()
{
	Serial.begin(115200);
	if(psramInit())
		Serial.println("\nPSRAM is correctly initialized");
	else
		Serial.println("PSRAM not available");

    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
	{
        Serial.println("SPIFFS Mount Failed");
       return;
    }

	listDir(SPIFFS, "/", 0);

	machine = new Apple2Machine();
	vga = new VGA();

	DEBUG_PRINTLN("===> MODE320x200");
    Mode mode = Mode::MODE_320x200x70;
#ifdef COLORBIT_8
    if(!vga->init(pins_8, mode, 8, 2)) while(1) delay(1);
#else    
	if(!vga->init(pins, mode, 16, 1)) while(1) delay(1);
#endif

    vga->start();
	DEBUG_PRINTLN("===> INIT Machine");
	machine->InitMachine();
}

unsigned long heapCheckMillis = 0;

unsigned long memlast = 0;
int frame = 0;
int fpscount = 0;
unsigned long fpsMillis = 0;

void loop()
{
	//vga->clear(vga->rgb(0,0,255));
	long long p = 17050 * 4;
	machine->Run(p);
    machine->Render(vga, frame);
	vga->show();

    if (frame++ > TARGET_FRAME) 
        frame = 0;

	if(millis() - heapCheckMillis > 150000)
	{
		unsigned int memcurr = ESP.getFreeHeap();
		//Serial.printf("FREEHeap: %d; DIFF %d\n", memcurr, memcurr - memlast);
		memlast = memcurr;
		heapCheckMillis = millis();

		Serial.printf("Heap : %d / %d\n", ESP.getFreeHeap(), ESP.getHeapSize());
		Serial.printf("PSRam : %d / %d\n", ESP.getFreePsram(), ESP.getPsramSize());
	}

    fpscount++;
    if( millis() - fpsMillis > 1000)
    {
        fpsMillis = millis();
        Serial.printf("FPS : %d\n", fpscount);
        fpscount = 0;
    }

}

