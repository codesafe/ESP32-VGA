
#include "Predef.h"
#include "AppleCPU.h"
#include "AppleMem.h"
#include "Apple2device.h"
#include "AppleFont.h"

/////////////////////////////////////////////////////////////////////////// 

const int offsetGR[24] = {                                                    // helper for TEXT and GR video generation
  0x000, 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380,                     // lines 0-7
  0x028, 0x0A8, 0x128, 0x1A8, 0x228, 0x2A8, 0x328, 0x3A8,                     // lines 8-15
  0x050, 0x0D0, 0x150, 0x1D0, 0x250, 0x2D0, 0x350, 0x3D0 };                    // lines 16-23


const int offsetHGR[192] = {                                                  // helper for HGR video generation
	0x0000, 0x0400, 0x0800, 0x0C00, 0x1000, 0x1400, 0x1800, 0x1C00,             // lines 0-7
	0x0080, 0x0480, 0x0880, 0x0C80, 0x1080, 0x1480, 0x1880, 0x1C80,             // lines 8-15
	0x0100, 0x0500, 0x0900, 0x0D00, 0x1100, 0x1500, 0x1900, 0x1D00,             // lines 16-23
	0x0180, 0x0580, 0x0980, 0x0D80, 0x1180, 0x1580, 0x1980, 0x1D80,
	0x0200, 0x0600, 0x0A00, 0x0E00, 0x1200, 0x1600, 0x1A00, 0x1E00,
	0x0280, 0x0680, 0x0A80, 0x0E80, 0x1280, 0x1680, 0x1A80, 0x1E80,
	0x0300, 0x0700, 0x0B00, 0x0F00, 0x1300, 0x1700, 0x1B00, 0x1F00,
	0x0380, 0x0780, 0x0B80, 0x0F80, 0x1380, 0x1780, 0x1B80, 0x1F80,
	0x0028, 0x0428, 0x0828, 0x0C28, 0x1028, 0x1428, 0x1828, 0x1C28,
	0x00A8, 0x04A8, 0x08A8, 0x0CA8, 0x10A8, 0x14A8, 0x18A8, 0x1CA8,
	0x0128, 0x0528, 0x0928, 0x0D28, 0x1128, 0x1528, 0x1928, 0x1D28,
	0x01A8, 0x05A8, 0x09A8, 0x0DA8, 0x11A8, 0x15A8, 0x19A8, 0x1DA8,
	0x0228, 0x0628, 0x0A28, 0x0E28, 0x1228, 0x1628, 0x1A28, 0x1E28,
	0x02A8, 0x06A8, 0x0AA8, 0x0EA8, 0x12A8, 0x16A8, 0x1AA8, 0x1EA8,
	0x0328, 0x0728, 0x0B28, 0x0F28, 0x1328, 0x1728, 0x1B28, 0x1F28,
	0x03A8, 0x07A8, 0x0BA8, 0x0FA8, 0x13A8, 0x17A8, 0x1BA8, 0x1FA8,
	0x0050, 0x0450, 0x0850, 0x0C50, 0x1050, 0x1450, 0x1850, 0x1C50,
	0x00D0, 0x04D0, 0x08D0, 0x0CD0, 0x10D0, 0x14D0, 0x18D0, 0x1CD0,
	0x0150, 0x0550, 0x0950, 0x0D50, 0x1150, 0x1550, 0x1950, 0x1D50,
	0x01D0, 0x05D0, 0x09D0, 0x0DD0, 0x11D0, 0x15D0, 0x19D0, 0x1DD0,
	0x0250, 0x0650, 0x0A50, 0x0E50, 0x1250, 0x1650, 0x1A50, 0x1E50,
	0x02D0, 0x06D0, 0x0AD0, 0x0ED0, 0x12D0, 0x16D0, 0x1AD0, 0x1ED0,             // lines 168-183
	0x0350, 0x0750, 0x0B50, 0x0F50, 0x1350, 0x1750, 0x1B50, 0x1F50,             // lines 176-183
	0x03D0, 0x07D0, 0x0BD0, 0x0FD0, 0x13D0, 0x17D0, 0x1BD0, 0x1FD0 };            // lines 184-191


const int color[16][3] = {                                                    // the 16 low res colors
	{ 0,   0,   0	  }, { 226, 57,  86  }, { 28,  116, 205 }, { 126, 110, 173 },
	{ 31,  129, 128 }, { 137, 130, 122 }, { 86,  168, 228 }, { 144, 178, 223 },
	{ 151, 88,  34	}, { 234, 108, 21  }, { 158, 151, 143 }, { 255, 206, 240 },
	{ 144, 192, 49	}, { 255, 253, 166 }, { 159, 210, 213 }, { 255, 255, 255 }
};

const int hcolor[16][3] = {                                                   // the high res colors (2 light levels)
	{ 0,   0,   0   }, { 144, 192, 49  }, { 126, 110, 173 }, { 255, 255, 255 },
	{ 0,   0,   0   }, { 234, 108, 21  }, { 86,  168, 228 }, { 255, 255, 255 },
	{ 0,   0,   0   }, { 63,  55,	 86  }, { 72,  96,  25	}, { 255, 255, 255 },
	{ 0,   0,   0   }, { 43,  84,	 114 }, { 117, 54,  10	}, { 255, 255, 255 }
};


Apple2Device::Apple2Device()
{
	DEBUG_PRINTLN("Construct Apple2Device");
	backbuffer = NULL;
	Reset();
}

Apple2Device::~Apple2Device()
{
	if (backbuffer != NULL)
		free(backbuffer);
}

void Apple2Device::Create(CPU* cpu)
{
	this->cpu = cpu;
	font.Create();
	zoomscale = 3;

	// 스크린 백버퍼
	backbuffer = (Color*)ps_malloc(SCREENSIZE_X * SCREENSIZE_Y * sizeof(Color));
	ClearScreen();
/*
	renderImage.data = backbuffer;
	renderImage.width = SCREENSIZE_X;
	renderImage.height = SCREENSIZE_Y;
	renderImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	renderImage.mipmaps = 1;
	renderTexture = LoadTextureFromImage(renderImage);
*/

	//////////////////////////////////////////////////////////////////////////
}

void Apple2Device::Reset()
{
	loaddumpmachine = false;
	dumpMachine = false;
	loadromfile = false;

	resetMachine = false;
	colorMonitor = true;
	keyboard = 0;

	pixelGR = { 0, 0, 7, 4 };

	// DISK ][
	updatedrive = 0;
	currentDrive = 0;
	// I/O register
	dLatch = 0;

	memset(phases, 0, sizeof(phases));
	memset(phasesB, 0, sizeof(phasesB));
	memset(phasesBB, 0, sizeof(phasesBB));
	memset(pIdx, 0, sizeof(pIdx));
	memset(pIdxB, 0, sizeof(pIdxB));
	memset(halfTrackPos, 0, sizeof(halfTrackPos));

	////////////////////////////////////////////////////////////////////////// VIDEO

	textMode = true;
	mixedMode = false;
	videoPage = 1;
	hires_Mode = false;

	//videoAddress = videoPage * 0x0400;
	memset(LoResCache, 0, sizeof(LoResCache));
	memset(HiResCache, 0, sizeof(HiResCache));
	memset(previousBit, 0, sizeof(previousBit));
	flashCycle = 0;
}


BYTE Apple2Device::SoftSwitch(Memory *mem, WORD address, BYTE value, bool WRT)
{
	switch (address) 
	{
		// KEYBOARD
		case 0xC000: 
			return(keyboard);
		// KBDSTROBE
		case 0xC010: 
			keyboard &= 0x7F; 
			return(keyboard);

		// TAPEOUT??
		case 0xC020:
			break;

		///////////////////////////////////////////////////////////////////////////////// Speaker

		case 0xC030: // SPEAKER
		//case 0xC033: 
			PlaySound(); 
			break;

		///////////////////////////////////////////////////////////////////////////////// Graphics

		case 0xC050: 
			textMode = false; 
			//printf("Text Mode Off\n");
			break;
		// Text
		case 0xC051: 
			textMode = true;  
			//printf("Text Mode On\n");
			break;

		// Mixed off
		case 0xC052: 
			mixedMode = false; 
			//printf("Mixed Mode Off\n");
			break;

		// Mixed on
		case 0xC053: 
			mixedMode = true;  
			//printf("Mixed Mode On\n");
			break;

		// Page 1
		case 0xC054: 
			videoPage = 1;
			//printf("Video Page 1\n");
			break;
		// Page 2
		case 0xC055: 
			videoPage = 2;
			//printf("Video Page 2\n");
			break;

		// HiRes off
		case 0xC056: 
			hires_Mode = false; 
			//printf("HIRES Mode Off\n");
			break;
		// HiRes on
		case 0xC057: 
			hires_Mode = true;  
			//printf("HIRES Mode On\n");
			break;

		/////////////////////////////////////////////////////////////////////////////////	Joy Paddle ?

/*
	https://apple2.org.za/gswv/a2zine/faqs/csa2pfaq.html

	These are actually the first two game Pushbutton inputs (PB0
	and PB1) which are borrowed by the Open Apple and Closed Apple
	keys. Bit 7 is set (=1) in these locations if the game switch or
	corresponding key is pressed.

	PB2 =      $C063 ;game Pushbutton 2 (read)
	This input has an option to be connected to the shift key on
	the keyboard. (See info on the 'shift key mod'.)

	PADDLE0 =  $C064 ;bit 7 = status of pdl-0 timer (read)
	PADDLE1 =  $C065 ;bit 7 = status of pdl-1 timer (read)
	PADDLE2 =  $C066 ;bit 7 = status of pdl-2 timer (read)
	PADDLE3 =  $C067 ;bit 7 = status of pdl-3 timer (read)
	PDLTRIG =  $C070 ;trigger paddles
	Read this to start paddle countdown, then time the period until
	$C064-$C067 bit 7 becomes set to determine the paddle position.
	This takes up to three milliseconds if the paddle is at its maximum
	extreme (reading of 255 via the standard firmware routine).

	SETIOUDIS= $C07E ;enable DHIRES & disable $C058-5F (W)
	CLRIOUDIS= $C07E ;disable DHIRES & enable $C058-5F (W)

*/
/*
		// Push Button 0
		case 0xC061: 
		{
			if (gamepad.pressbtn2)
				return 0x80;
			else
				return 0;
		}
		// Push Button 1
		case 0xC062: 
		{
			if (gamepad.pressbtn1)
				return 0x80;
			else
				return 0;
		}

// 		// Push Button 2
// 		case 0xC063: 
// 			return 0;

		// Paddle 0
		case 0xC064: 
		{
			BYTE v = readPaddle(0);
			return(v);
		}

		// Paddle 1
		case 0xC065: 
		{
			BYTE v = readPaddle(1);
			return(v);
		}

		// paddle timer RST
		case 0xC070: 
			resetPaddles(); 
			break;
*/
		/////////////////////////////////////////////////////////////////////////////////	DISK 2

		case 0xC0E0:
		case 0xC0E1:
		case 0xC0E2:
		case 0xC0E3:
		case 0xC0E4:
		case 0xC0E5:
		case 0xC0E6:
		case 0xC0E7: 
			stepMotor(address); 
			break; // MOVE DRIVE HEAD

		// MOTOR OFF
		case 0xCFFF:
		case 0xC0E8: 
			disk[currentDrive].motorOn = false; 
			//printf("--> DISK MOTOR OFF\n");
			break;

		// MOTOR ON
		case 0xC0E9: 
			disk[currentDrive].motorOn = true;  
			//printf("--> DISK MOTOR ON\n");
			break;

		// DRIVE 0
		case 0xC0EA: 
			setDrv(0); 
			break;
		// DRIVE 1
		case 0xC0EB: 
			setDrv(1); 
			break;

		// Shift Data Latch
		case 0xC0EC:                                                                
		{
			// writting
			if (disk[currentDrive].writeMode)
				disk[currentDrive].data[disk[currentDrive].track * 0x1A00 + disk[currentDrive].nibble] = dLatch;
			else
			{
				// reading
				dLatch = disk[currentDrive].data[disk[currentDrive].track * 0x1A00 + disk[currentDrive].nibble];
			}
			// turn floppy of 1 nibble
			disk[currentDrive].nibble = (disk[currentDrive].nibble + 1) % 0x1A00;
		}
		return(dLatch);

		// Load Data Latch
		case 0xC0ED: 
			dLatch = value; 
			break;

		// latch for READ
		case 0xC0EE:
			disk[currentDrive].writeMode = false;
			return(disk[currentDrive].readOnly ? 0x80 : 0);                                 // check protection

		// latch for WRITE
		case 0xC0EF: 
			disk[currentDrive].writeMode = true; 
			break;

		///////////////////////////////////////////////////////////////////////////////// LANGUAGE CARD

		case 0xC080: 
		case 0xC084: 
			mem->LCBank2Enable = 1; 
			mem->LCReadable = 1; 
			mem->LCWritable = 0;
			mem->LCPreWriteFlipflop = 0;    
			break;       // LC2RD

		case 0xC081:
		case 0xC085: 
			mem->LCBank2Enable = 1;
			mem->LCReadable = 0; 
			mem->LCWritable |= mem->LCPreWriteFlipflop; 
			mem->LCPreWriteFlipflop = !WRT; 
			break;       // LC2WR

		case 0xC082:
		case 0xC086: 
			mem->LCBank2Enable = 1;
			mem->LCReadable = 0;
			mem->LCWritable = 0;
			mem->LCPreWriteFlipflop = 0;    
			break;       // ROMONLY2

		case 0xC083:
		case 0xC087: 
			mem->LCBank2Enable = 1;
			mem->LCReadable = 1;
			mem->LCWritable |= mem->LCPreWriteFlipflop; 
			mem->LCPreWriteFlipflop = !WRT; 
			break;       // LC2RW

		case 0xC088:
		case 0xC08C: 
			mem->LCBank2Enable = 0;
			mem->LCReadable = 1; 
			mem->LCWritable = 0;
			mem->LCPreWriteFlipflop = 0;    
			break;       // LC1RD

		case 0xC089:
		case 0xC08D: 
			mem->LCBank2Enable = 0; 
			mem->LCReadable = 0; 
			mem->LCWritable |= mem->LCPreWriteFlipflop; 
			mem->LCPreWriteFlipflop = !WRT; 
			break;       // LC1WR

		case 0xC08A:
		case 0xC08E: 
			mem->LCBank2Enable = 0; 
			mem->LCReadable = 0; 
			mem->LCWritable = 0; 
			mem->LCPreWriteFlipflop = 0;    
			break;       // ROMONLY1

		case 0xC08B:
		case 0xC08F: 
			mem->LCBank2Enable = 0; mem->LCReadable = 1; 
			mem->LCWritable |= mem->LCPreWriteFlipflop; 
			mem->LCPreWriteFlipflop = !WRT; 
			break;       // LC1RW
	}

	return (cpu->tick % 256);
}

void Apple2Device::ClearScreen()
{
	for (int y = 0; y < SCREENSIZE_Y; y++)
		for (int x = 0; x < SCREENSIZE_X; x++)
		{
			Color color(30, 30, 30);
			backbuffer[y * SCREENSIZE_X + x] = color;
		}
}

void Apple2Device::DrawPoint(int x, int y, int r, int g, int b)
{
	Color color(0,0,0);
	if (colorMonitor)
	{
		color.r = r; color.g = g; color.b = b; color.a = 0xff;
	}
	else
	{
		float grayscale = (0.299f * r) + (0.587f * g) + (0.114f * b);
		color.r = 0; color.g = (BYTE)grayscale; color.b = 0; color.a = 0xff;
	}

	backbuffer[y * SCREENSIZE_X + x] = color;
}

void Apple2Device::DrawRect(_RECT rect, int r, int g, int b)
{
	for (int y = 0; y < (int)rect.height; y++)
		for (int x = 0; x < (int)rect.width; x++)
		{
			DrawPoint((int)rect.x + x, (int)rect.y + y, r, g, b);
		}
}

int Apple2Device::GetScreenMode()
{
	if (mixedMode == false)
	{
		if (textMode == false && hires_Mode)
			return HIRES_MODE;

		if (textMode == false && hires_Mode == false)
			return LORES_MODE;

		if (textMode == true && hires_Mode == false)
			return TEXT_MODE;
	}
	else
	{
		if (hires_Mode)
			return HIRES_MIX_MODE;
		else
			return LORES_MIX_MODE;
	}

	return TEXT_MODE;
}

/*
	TEXT 40x24 ( 7x8 Font )
	LORES : 40x24 (MIX 40x20)
	HIRES : 280×192 (MIX 280×160)
	MIX일경우에 하단은 TEXT( 4Line : 32 pixel )
*/
void Apple2Device::Render(Memory &mem, int frame)
{
	int screenmode = GetScreenMode();

	// video Page에 따라 Address가 달라짐
	// $400, $800, $2000, $4000
	if (screenmode == LORES_MODE || screenmode == HIRES_MODE || 
		screenmode == LORES_MIX_MODE || screenmode == HIRES_MIX_MODE)
	{
		// LoRes 저해상도
		if (hires_Mode == false)
		{
			videoAddress = videoPage * 0x0400;
			BYTE glyph;                                                            // 2 blocks in GR
			BYTE colorIdx = 0;                                                     // to index the color arrays

			// for each column
			for (int col = 0; col < 40; col++) 
			{
				pixelGR.x = col * 7;
				// Mixmode이면 하단 4라인은 Text용
				for (int line = 0; line < (mixedMode ? 20 : 24); line++) 
				{
					pixelGR.y = line * 8;                                                 // first block

					glyph = mem.ReadByte(videoAddress + offsetGR[line] + col);                         // read video memory

					if (LoResCache[line][col] != glyph || !flashCycle) 
					{
						LoResCache[line][col] = glyph;

						// first nibble(4bit) 1/2 Byte
						colorIdx = glyph & 0x0F;
						DrawRect(pixelGR, color[colorIdx][0], color[colorIdx][1], color[colorIdx][2]);

						pixelGR.y += 4;                                                       // second block
						colorIdx = (glyph & 0xF0) >> 4;                                       // second nibble
						DrawRect(pixelGR, color[colorIdx][0], color[colorIdx][1], color[colorIdx][2]);
					}
				}
			}
		}
		else
		{
			// highRes 고해상도
			WORD word;
			BYTE bits[16], bit, pbit, colorSet, even;
			// PAGE is 1 or 2
			videoAddress = videoPage * 0x2000;
			BYTE colorIdx = 0;

			// Mixmode이면 하단 4라인은 Text용
			for (int line = 0; line < (mixedMode ? 160 : 192); line++)
			{
				// for every 7 horizontal dots
				for (int col = 0; col < 40; col += 2) 
				{
					int x = col * 7;
					even = 0;

					word = (WORD)(mem.ReadByte((videoAddress + offsetHGR[line] + col + 1))) << 8;    // store the two next bytes into 'word'
					word += mem.ReadByte(videoAddress + offsetHGR[line] + col);              // in reverse order

					// check if this group of 7 dots need a redraw
					if (HiResCache[line][col] != word || !flashCycle) 
					{

						for (bit = 0; bit < 16; bit++)                                        // store all bits 'word' into 'bits'
							bits[bit] = (word >> bit) & 1;

						colorSet = bits[7] * 4;                                             // select the right color set
						pbit = previousBit[line][col];                                      // the bit value of the left dot
						bit = 0;                                                            // starting at 1st bit of 1st byte

						while (bit < 15) 
						{                                                  // until we reach bit7 of 2nd byte
							if (bit == 7) 
							{                                                   // moving into the second byte
								colorSet = bits[15] * 4;                                        // update the color set
								bit++;                                                          // skip bit 7
							}
							colorIdx = even + colorSet + (bits[bit] << 1) + (pbit);

							DrawPoint(x++, line, hcolor[colorIdx][0], hcolor[colorIdx][1], hcolor[colorIdx][2]);
							pbit = bits[bit++];                                               // proceed to the next pixel
							even = even ? 0 : 8;                                              // one pixel every two is darker
						}

						HiResCache[line][col] = word;                                       // update the video cache
						if ((col < 37) && (previousBit[line][col + 2] != pbit)) {           // check color franging effect on the dot after
							previousBit[line][col + 2] = pbit;                                // set pbit and clear the
							HiResCache[line][col + 2] = -1;                                   // video cache for next dot
						}
					}                                                                     // if (HiResCache[line][col] ...
				}
			}

		}
	}

	// TEXT는 TEXT Only 그리고 Mixed에 모두 출력되어야 함
	if (screenmode == TEXT_MODE || screenmode == LORES_MIX_MODE || screenmode == HIRES_MIX_MODE)
	{
// 		if(screenmode == TEXT_MODE)
// 			ClearScreen();

		videoAddress = videoPage * 0x0400;

		// Text or Mixed
		// Font 크기 7X8 / 40x20 글자
		int linelimit = textMode ? 0 : 20;

		for (int col = 0; col < SCREENTEXT_X; col++)
		{
			for (int line = linelimit; line < SCREENTEXT_Y; line++)
			{
				// read video memory
				BYTE glyph = mem.ReadByte(videoAddress + offsetGR[line] + col);

				int fontattr = 0;
				if (glyph > 0x7F)
					fontattr = FONT_NORMAL;
				else if (glyph < 0x40)
					fontattr = FONT_INVERSE;
				else
					fontattr = FONT_FLASH;

				glyph &= 0x7F; // unset bit 7
				if (glyph > 0x5F) glyph &= 0x3F; // shifts to match
				if (glyph < 0x20) glyph |= 0x40; // the ASCII codes

				if (fontattr == FONT_NORMAL || (fontattr == FONT_FLASH && frame < 15))
				{
					font.RenderFont(backbuffer, glyph, col * FONT_X, line * FONT_Y, false);
				}
				else
				{
					font.RenderFont(backbuffer, glyph, col * FONT_X, line * FONT_Y, true);
				}
			}
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	// Render Backbuffer
	UnloadTexture(renderTexture);
	renderTexture = LoadTextureFromImage(renderImage);

	Vector2 pos;
 	pos.x = 300;
 	pos.y = 10;
	DrawTextureEx(renderTexture, pos, 0, zoomscale, WHITE);

	const int gap = 8;
	Rectangle rec;
	rec.x = pos.x - gap;
	rec.y = pos.y - gap;
	rec.width = (float)(SCREENSIZE_X * zoomscale + (gap * 2));
	rec.height = (float)(SCREENSIZE_Y * zoomscale + (gap * 2));
	DrawRectangleLinesEx(rec, 2, GRAY);
*/
	if (++flashCycle == 30)
		flashCycle = 0;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Apple Disk II 
bool Apple2Device::InsertFloppy(const char* filename, int drv)
{
	int readlen = filesystem.ReadFile(filename, disk[drv].data, DISKSIZE);
	if ( readlen != DISKSIZE)
		return false;

	sprintf(disk[drv].filename, "%s", filename);

	// 일단 쓰기 불가 모드로 진행
	disk[drv].readOnly = false;	// 읽기만 가능
	return true;
}

void Apple2Device::stepMotor(WORD address)
{
	address &= 7;
	int phase = address >> 1;

	phasesBB[currentDrive][pIdxB[currentDrive]] = phasesB[currentDrive][pIdxB[currentDrive]];
	phasesB[currentDrive][pIdx[currentDrive]] = phases[currentDrive][pIdx[currentDrive]];
	pIdxB[currentDrive] = pIdx[currentDrive];
	pIdx[currentDrive] = phase;

	if (!(address & 1)) 
	{                                                         // head not moving (PHASE x OFF)
		phases[currentDrive][phase] = false;
		return;
	}

	if ((phasesBB[currentDrive][(phase + 1) & 3]) && (--halfTrackPos[currentDrive] < 0))      // head is moving in
		halfTrackPos[currentDrive] = 0;

	if ((phasesBB[currentDrive][(phase - 1) & 3]) && (++halfTrackPos[currentDrive] > 140))    // head is moving out
		halfTrackPos[currentDrive] = 140;

	phases[currentDrive][phase] = true;                                                 // update track#
	disk[currentDrive].track = (halfTrackPos[currentDrive] + 1) / 2;
}

void Apple2Device::setDrv(int drv)
{
	disk[drv].motorOn = disk[!drv].motorOn || disk[drv].motorOn;                  // if any of the motors were ON
	disk[!drv].motorOn = false;                                                   // motor of the other drive is set to OFF
	currentDrive = drv;                                                                 // set the current drive
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////// Input

void Apple2Device::UpdateInput()
{
	UpdateKeyBoard();
	UpdateGamepad();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

// 플로피 디스크 업데이트
bool Apple2Device::UpdateFloppyDisk()
{
	// Floppy motor가 off이거나 updatedrive이 0이되면 끝
	if (disk[currentDrive].motorOn == false || updatedrive++ == 0)
		return false;
	else
		return true;
}

void Apple2Device::InsetFloppy()
{
	//memset(&disk[0], 0, sizeof(FloppyDrive));
	//memset(&disk[1], 0, sizeof(FloppyDrive));
	disk[0].Reset();
	disk[1].Reset();

//	InsertFloppy("rom/DOS3.3.nib", 0);
	InsertFloppy("/LodeRunner.nib", 0);
//	InsertFloppy("rom/Pacman.nib", 0);
//	InsertFloppy("rom/karateka.nib", 0);
//	InsertFloppy("rom/Ultima4-1.nib", 0);
//	InsertFloppy("rom/Ultima5-1.nib", 0);
//	InsertFloppy("rom/Captain Goodnight-A.nib", 0);
//	InsertFloppy("rom/Where in the World is Carmen Sandiego-A.nib", 0);
}

bool Apple2Device::GetDiskMotorState()
{
	return disk[currentDrive].motorOn;
}

int tock = 0;
void Apple2Device::PlaySound()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////// 키보드

void Apple2Device::UpdateKeyBoard()
{
}


// game pad update
void Apple2Device::UpdateGamepad()
{
}

std::string Apple2Device::GetDiskName(int i)
{
	return disk[i].filename;
}


///////////////////////////////////////////////////////////////////////////////////////

void Apple2Device::Dump(FILE* fp)
{

}

void Apple2Device::LoadDump(FILE* fp)
{

}

// Surface backbuffer
Color * Apple2Device::getBackBuffer()
{
	return backbuffer;
}
