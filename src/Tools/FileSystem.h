#pragma once

#include <Arduino.h>
#include <SPIFFS.h>
#include "./Tools/Log.h"

#define FORMAT_SPIFFS_IF_FAILED true

class FileSystem
{
public :
     FileSystem()
     {
        DEBUG_PRINTLN("Construct FileSystem");

        if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
            Serial.println("SPIFFS Mount Failed");
     }

    ~FileSystem()
    {
        SPIFFS.end();
    }

    int ReadFile(const char *path, unsigned char *buffer, int len)
    {
        File file = SPIFFS.open(path);
        if(!file)
        {
            Serial.println("- failed to open file for reading");
            return -1;
        }

        //Serial.println("- read from file:");
        int readlen = file.read(buffer, len);
        file.close();

        return readlen;
    }

};