#ifndef MEMORY_H
#define MEMORY_H

#include "Predef.h"
#include "Apple2Device.h"

class Memory
{
	public:
		// LC -> Language Card
		bool LCWritable;
		bool LCReadable;
		bool LCBank2Enable;			// bank 2 enabled
		bool LCPreWriteFlipflop;	// pre-write flip flop

		//BYTE ram[RAMSIZE];  // 48K of ram in $000-$BFFF
		//BYTE rom[ROMSIZE];  // 12K of rom in $D000-$FFFF		
		//BYTE lgc[LGCSIZE];  // Language Card 12K in $D000-$FFFF
		//BYTE bk2[BK2SIZE];  // bank 2 of Language Card 4K in $D000-$DFFF
		//BYTE sl6[SL6SIZE];  // P5A disk ][ PROM in slot 6
		BYTE *ram;
		BYTE *rom;
		BYTE *lgc;
		BYTE *bk2;
		BYTE *sl6;

		Apple2Device* device;

	public:
		Memory();
		~Memory();

		void Create();
		void Destroy();
		void Reset();

		BYTE ReadByte(int addr);
		void WriteByte(int addr, BYTE value);
		WORD ReadWord(int addr);
		void WriteWord(WORD value, int addr);

		WORD UpLoadProgram(BYTE *code, int codesize);
		void UpLoadProgram(int startPos, BYTE *code, int codesize);

		void UpLoadToRom(BYTE* code);
		void ResetRam();

		void Dump(FILE* fp);
		void LoadDump(FILE* fp);
};


#endif