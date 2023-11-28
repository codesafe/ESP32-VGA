#include "Predef.h"
#include "rombios.h"
#include "Apple2Machine.h"
#include "./Tools/Log.h"
#include "./VGA/ESP32S3VGA.h"

Apple2Machine::Apple2Machine()
{
	DEBUG_PRINTLN("Construct Apple2Machine");
}

Apple2Machine::~Apple2Machine()
{

}

void Apple2Machine::InitMachine()
{
	mem.Create();
	device.InsetFloppy();
	// unset the Power-UP byte
	mem.WriteByte(0x3F4, 0);
	cpu.Reset(mem);
	mem.WriteByte(0x4D, 0xAA);   // Just crashes if this memory location equals zero
	mem.WriteByte(0xD0, 0xAA);   // won't work if this memory location equals zero

	device.Create(&cpu);
	mem.device = &device;

	Booting();
	//UploadRom();	
}

// 롬을 내장
bool Apple2Machine::Booting()
{
	DEBUG_PRINTLN("====> BOOTING ...");
	DEBUG_PRINTLN("Load Apple II Rom");
	memcpy(mem.rom, appleIIrom, ROMSIZE);
	DEBUG_PRINTLN("Load Disk II");
	memcpy(mem.sl6, diskII, SL6SIZE);
	cpu.Reset(mem);
	return true;
}

// 롬을 파일에서 로딩
bool Apple2Machine::UploadRom()
{
	bool ret = false;

	// load the Apple II+ ROM
	BYTE* rom =(BYTE*)ps_malloc(ROMSIZE);
	FILE* fp = fopen("/apple2.rom", "rb"); 
	if (fp)
	{
#if 0
		fread(rom, ROMSIZE, 1, fp);
		mem.UpLoadProgram(ROMSTART, rom, ROMSIZE);
#else
		fread(mem.rom, ROMSIZE, 1, fp);
#endif
		fclose(fp);
		ret = true;
	}
	free(rom);

	// load Apple II+ / Disk II
	BYTE* disk2 = (BYTE*)ps_malloc(SL6SIZE);
	FILE* disk2fp = fopen("rom/diskII.rom", "rb");
	if (disk2fp)
	{
#if 0
		fread(disk2, SL6SIZE, 1, disk2fp);
		mem.UpLoadProgram(SL6START, disk2, SL6SIZE);
#else
		fread(mem.sl6, SL6SIZE, 1, disk2fp);
#endif
		fclose(disk2fp);
		ret = true;
	}
	free(disk2);

	cpu.Reset(mem);
	return ret;
}

void Apple2Machine::Reset()
{
	mem.Reset();
	cpu.Reboot(mem);
	device.Reset();

	// unset the Power-UP byte
	mem.WriteByte(0x3F4, 0);
	// dirty hack, fix soon... if I understand why
	mem.WriteByte(0x4D, 0xAA);   // Joust crashes if this memory location equals zero
	mem.WriteByte(0xD0, 0xAA);   // Planetoids won't work if this memory location equals zero

	Booting();
}

void Apple2Machine::Run(int cycle)
{
	if (device.resetMachine)
	{
		Reset();
		return;
	}

	device.UpdateInput();
	cpu.Run(mem, cycle);
	while (1)
	{
		if( device.UpdateFloppyDisk() == false ) 
			break;
		cpu.Run(mem, 10000);
	}
}

void Apple2Machine::Render(VGA *vga, int frame)
{
	device.Render(mem, frame);

	Color * backbuffer = device.getBackBuffer();
	for(int y=0; y<SCREENSIZE_Y; y++)
		for(int x=0; x<SCREENSIZE_X; x++)
		{
			Color color = backbuffer[y*SCREENSIZE_X+x];
			vga->dot(x,y,color.GetColor());
		}
}


// DUMP파일을 로드하여 재개
void Apple2Machine::LoadMachine(std::string path)
{
}

// 현재의 모든 상태를 저장
void Apple2Machine::DumpMachine(std::string path)
{
}

