﻿
/*
	MOS 6502 CPU Emulator
*/

#include "AppleMem.h"

Memory::Memory()
{
	device = NULL;
}

Memory::~Memory()
{

}

void Memory::Create()
{
	ram = new BYTE[RAMSIZE];  // 48K of ram in $000-$BFFF
	rom = new BYTE[ROMSIZE];  // 12K of rom in $D000-$FFFF
	lgc = new BYTE[LGCSIZE];
	bk2 = new BYTE[BK2SIZE];
	sl6 = new BYTE[SL6SIZE];

	Reset();
}

void Memory::Destroy()
{
	delete [] ram;
	delete [] rom;
	delete [] lgc;
	delete [] bk2;
	delete [] sl6;
}

void Memory::Reset()
{
	LCWritable = true;
	LCReadable = false;
	LCBank2Enable = true;
	LCPreWriteFlipflop = false;

	memset(ram, 0, RAMSIZE);
	memset(rom, 0, ROMSIZE);
	memset(lgc, 0, LGCSIZE);
	memset(bk2, 0, BK2SIZE);
	memset(sl6, 0, SL6SIZE);
}


BYTE Memory::ReadByte(int address)
{
#if 0
	BYTE v = memory[address];
	if (address == 0xCFFF || ((address & 0xFF00) == 0xC000))
		v = device->SoftSwitch(address, 0, false);
	return v;
#else
	if (address < RAMSIZE)
		return ram[address];                                                        // RAM

	if (address >= ROMSTART) {
		if (!LCReadable)
			return rom[address - ROMSTART];                                           // ROM

		if (LCBank2Enable && (address < 0xE000))
			return bk2[address - BK2START];                                           // BK2

		return lgc[address - LGCSTART];                                             // LC
	}

	if ((address & 0xFF00) == SL6START)
		return sl6[address - SL6START];  // disk][

	if ((address & 0xF000) == 0xC000)
		return (device->SoftSwitch(this, address, 0, false));

	return 0;
#endif
}

void Memory::WriteByte(int address, BYTE value)
{
#if 0

	memory[address] = value;
	if (address == 0xCFFF || ((address & 0xFF00) == 0xC000))
		device->SoftSwitch(address, value, true);
#else
	if (address < RAMSIZE) {
		ram[address] = value;                                                       // RAM
		return;
	}

	if (LCWritable && (address >= ROMSTART)) {
		if (LCBank2Enable && (address < 0xE000)) {
			bk2[address - BK2START] = value;                                          // BK2
			return;
		}
		lgc[address - LGCSTART] = value;                                            // LC
		return;
	}

	if ((address & 0xF000) == 0xC000)
	{
		device->SoftSwitch(this, address, value, true);
		return;
	}
#endif
}

WORD Memory::ReadWord(int addr)
{
	BYTE m0 = ReadByte(addr);
	BYTE m1 = ReadByte(addr + 1);
	WORD w = (m1 << 8) | m0;
	return w;
}

void Memory::WriteWord(WORD value, int addr)
{
	WriteByte(addr, value >> 8);
	WriteByte(addr + 1, value & 0xFF);
}


void Memory::UpLoadToRom(BYTE* code)
{
	memcpy(rom, code, ROMSIZE);
}

void Memory::ResetRam()
{
	memset(ram, 0, RAMSIZE);
}

void Memory::Dump(FILE* fp)
{

}

void Memory::LoadDump(FILE* fp)
{

}