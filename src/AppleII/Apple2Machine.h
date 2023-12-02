#ifndef APPLE2_MACHINE_H
#define APPLE2_MACHINE_H

#include <stdio.h>
#include "AppleCpu.h"
#include "AppleMem.h"
#include "Apple2Device.h"

class VGA;

class Apple2Machine
{
public:
	CPU cpu;
	Memory mem;
	Apple2Device device;

private:
	bool Booting();
	bool UploadRom();

public:
	Apple2Machine();
	~Apple2Machine();

	void InitMachine();
	void Reset();
	void Run(long long cycle);
	void Render(VGA *vga, int frame);

	void LoadMachine(std::string path);
	void DumpMachine(std::string path);
};



#endif
