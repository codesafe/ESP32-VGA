#include <Arduino.h>
#include <SPIFFS.h>

#include "./Tools/Log.h"
#include "./VGA/ESP32S3VGA.h"
#include "./AppleII/Apple2Machine.h"

#include "FS.h"
//#include "SPIFFS.h"

//pin configuration
#define PIN_NULL	0
#define PIN_R0	4
#define PIN_R1	5
#define PIN_R2	6

#define PIN_G0	10
#define PIN_G1	11
#define PIN_G2	12

#define PIN_B0	13
#define PIN_B1	14

#define HSYNC	1
#define VSYNC	2

#define FORMAT_SPIFFS_IF_FAILED true

const PinConfig pins(
PIN_R0, PIN_R1, PIN_R2, PIN_NULL, PIN_NULL,
PIN_G0, PIN_G1, PIN_G2, PIN_NULL, PIN_NULL, PIN_NULL,
PIN_B0, PIN_B1, PIN_NULL, PIN_NULL, PIN_NULL,
HSYNC, VSYNC);

VGA *vga;
Apple2Machine *machine;

void readFile(fs::FS &fs, const char * path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
//    while(file.available()){
//        Serial.write(file.read());
//    }

	unsigned char *buf = (unsigned char *)ps_malloc(232960);
	int s = file.readBytes((char*)buf, 232960);
	Serial.printf("Reading : %d\r\n", s);
	free(buf);
    file.close();
}

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

	//listDir(SPIFFS, "/", 0);
	//readFile(SPIFFS, "/loderunner.nib");

	machine = new Apple2Machine();
	vga = new VGA();

	DEBUG_PRINTLN("MODE320x200");
	Mode mode = Mode::MODE_320x200x70;
	if(!vga->init(pins, mode, 8, 2)) while(1) delay(1);

	DEBUG_PRINTLN("INIT Machine");
	machine->InitMachine();
}

int colorindex = 0;
unsigned long heapCheckMillis = 0;
unsigned long memlast = 0;




void loop()
{
	vga->clear(0);

	long long p = 17050;// *1.2f;
	machine->Run((int)p);

	//delay(1000);
	vga->show();

	if(millis() - heapCheckMillis > 15000)
	{
		unsigned int memcurr = ESP.getFreeHeap();
		//Serial.printf("FREEHeap: %d; DIFF %d\n", memcurr, memcurr - memlast);
		memlast = memcurr;
		heapCheckMillis = millis();

		Serial.printf("Heap : %d / %d\n", ESP.getFreeHeap(), ESP.getHeapSize());
		Serial.printf("PSRam : %d / %d\n", ESP.getFreePsram(), ESP.getPsramSize());
	}

}

