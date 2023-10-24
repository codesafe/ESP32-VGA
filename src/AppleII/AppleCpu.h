#ifndef CPU_H
#define CPU_H

/*
	MOS 6502 CPU Emulator
*/

#include "Predef.h"
#include <map>
#include <string>

class Memory;


#define REGISTER_A		0
#define REGISTER_X		1
#define REGISTER_Y		2

/*
	https://www.cs.jhu.edu/~phi/csf/slides/lecture-6502-stack.pdf
	6502 Stack은 0x01FF, 0x01FE, 0x01FD --> 0x0100로 Push 된다
	Stack은 총 256 Byte
*/
#define STACK_ADDRESS	0x0100
#define STACK_POS		0xFD
#define PC_START		0xFFFC


/*
BYTE C : 1;	//0: Carry Flag
BYTE Z : 1;	//1: Zero Flag
BYTE I : 1; //2: Interrupt disable
BYTE D : 1; //3: Decimal mode
BYTE B : 1; //4: Break
//BYTE Unused : 1; //5: Unused
BYTE V : 1; //6: Overflow
BYTE N : 1; //7: Negative
*/
// Flag Define : PS
#define FLAG_CARRY					0b00000001
#define FLAG_ZERO					0b00000010
#define FLAG_INTERRUPT_DISABLE		0b00000100
#define FLAG_DECIMAL_MODE			0b00001000
#define FLAG_BREAK					0b00010000
#define FLAG_UNUSED					0b00100000
#define FLAG_OVERFLOW				0b01000000
#define FLAG_NEGATIVE				0b10000000

// Instruction
/*
	구현된것들은 () 처리
	(AND) (EOR) (ORA) (BIT)
	(JMP) (JSR) (RTS)
	(NOP)
	(LDA) (LDX) (LDY)
	(TXS) (TSX) (PHA) (PHP) (PLA) (PLP) 
	(STA) (STX) (STY)
	(ADC) (SBC) (CMP) (CPX) (CPY)
	(DEC) (DEX) (DEY) (INC) (INX) (INY)
	(TAX) (TAY) (TXA) (TYA)
	(ASL) (LSR) (ROL) (ROR)
	(BRK) (RTI)
	(BCC) (BCS) (BEQ) (BMI) (BNE) (BPL) (BVC) (BVS)
	(CLC) (CLD) (CLI) (CLV)
	(SEC) (SED) (SEI)  
*/

// LDA (LoaD Accumulator)
#define LDA_IM		0xA9
#define LDA_ZP		0xA5
#define LDA_ZPX		0xB5
#define LDA_ABS		0xAD
#define LDA_ABSX	0xBD
#define LDA_ABSY	0xB9
#define LDA_INDX	0xA1
#define LDA_INDY	0xB1

// LDX (LoaD X register)
#define LDX_IM		0xA2
#define LDX_ZP		0xA6
#define LDX_ZPY		0xB6
#define LDX_ABS		0xAE
#define LDX_ABSY	0xBE

// LDY (LoaD Y register)
#define LDY_IM		0xA0
#define LDY_ZP		0xA4
#define LDY_ZPX		0xB4
#define LDY_ABS		0xAC
#define LDY_ABSX	0xBC

// No Operation
#define NOP			0xEA

// JSR (Jump to Subroutine)
#define JSR			0x20
#define JMP_ABS		0x4C
#define JMP_IND		0x6C
// Return from Subroutine
#define RTS			0x60


// STA - Store Accumulator
#define STA_ZP		0x85
#define STA_ZPX		0x95
#define STA_ABS		0x8D
#define STA_ABSX	0x9D
#define STA_ABSY	0x99
#define STA_INDX	0x81
#define STA_INDY	0x91

// STX - Store X Register
#define STX_ZP		0x86
#define STX_ZPY		0x96
#define STX_ABS		0x8E

// STY - Store Y Register
#define STY_ZP		0x84
#define STY_ZPX		0x94
#define STY_ABS		0x8C


// Stack Operation
#define TSX			0xBA		// Transfer Stack Pointer to X
#define TXS			0x9A		// Transfer X to Stack Pointer
#define PHA			0X48		// Push Accumulator
#define PLA			0x68		// Pull Accumulator
#define PLP			0x28		// Pull Processor Status
#define PHP			0x08		// Pushes a copy of the status flags on to the stack.

// Logical Operation
#define AND_IM		0x29
#define AND_ZP		0x25
#define AND_ZPX		0x35
#define AND_ABS		0x2D
#define AND_ABSX	0x3D
#define AND_ABSY	0x39
#define AND_INDX	0x21
#define AND_INDY	0x31

#define ORA_IM		0x09
#define ORA_ZP		0x05
#define ORA_ZPX		0x15
#define ORA_ABS		0x0D
#define ORA_ABSX	0x1D
#define ORA_ABSY	0x19
#define ORA_INDX	0x01
#define ORA_INDY	0x11

#define EOR_IM		0x49
#define EOR_ZP		0x45
#define EOR_ZPX		0x55
#define EOR_ABS		0x4D
#define EOR_ABSX	0x5D
#define EOR_ABSY	0x59
#define EOR_INDX	0x41
#define EOR_INDY	0x51

// Bit Test
#define BIT_ZP		0x24
#define BIT_ABS		0x2C


// Register Transfer
#define TAX			0xAA
#define TAY			0xA8
#define TXA			0x8A
#define TYA			0x98

// increase / decrease
#define INX			0xE8
#define INY			0xC8
#define DEX			0xCA
#define DEY			0x88
#define INC_ZP		0xE6
#define INC_ZPX		0xF6
#define INC_ABS		0xEE
#define INC_ABSX	0xFE
#define DEC_ZP		0xC6
#define DEC_ZPX		0xD6
#define DEC_ABS		0xCE
#define DEC_ABSX	0xDE

// Arithmetic
#define ADC_IM		0x69
#define ADC_ZP		0x65
#define ADC_ZPX		0x75
#define ADC_ABS		0x6D
#define ADC_ABSX	0x7D
#define ADC_ABSY	0x79
#define ADC_INDX	0x61
#define ADC_INDY	0x71

#define SBC_IM		0xE9
#define SBC_ZP		0xE5
#define SBC_ZPX		0xF5
#define SBC_ABS		0xED
#define SBC_ABSX	0xFD
#define SBC_ABSY	0xF9
#define SBC_INDX	0xE1
#define SBC_INDY	0xF1

#define CMP_IM		0xC9
#define CMP_ZP		0xC5
#define CMP_ZPX		0xD5
#define CMP_ABS		0xCD
#define CMP_ABSX	0xDD
#define CMP_ABSY	0xD9
#define CMP_INDX	0xC1
#define CMP_INDY	0xD1

#define CPX_IM		0xE0
#define CPX_ZP		0xE4
#define CPX_ABS		0xEC

#define CPY_IM		0XC0
#define CPY_ZP		0XC4
#define CPY_ABS		0XCC


// Shifts
#define ASL			0x0A
#define ASL_ZP		0x06
#define ASL_ZPX		0x16
#define ASL_ABS		0x0E
#define ASL_ABSX	0x1E

#define LSR			0x4A
#define LSR_ZP		0x46
#define LSR_ZPX		0x56
#define LSR_ABS		0x4E
#define LSR_ABSX	0x5E

#define ROL			0x2A
#define ROL_ZP		0x26
#define ROL_ZPX		0x36
#define ROL_ABS		0x2E
#define ROL_ABSX	0x3E

#define ROR			0x6A
#define ROR_ZP		0x66
#define ROR_ZPX		0x76
#define ROR_ABS		0x6E
#define ROR_ABSX	0x7E

// Force Interrupt
#define BRK			0x00
#define RTI			0x40

// Branches
#define BCC			0x90
#define BCS			0xB0
#define BEQ			0xF0
#define BMI			0x30
#define BNE			0xD0
#define BPL			0x10
#define BVC			0x50
#define BVS			0x70

// Status Flag Changes
#define CLC			0x18
#define CLD			0xD8
#define CLI			0x58
#define CLV			0xB8
#define SEC			0x38
#define SED			0xF8
#define SEI			0x78

struct StatusFlags
{
	BYTE C : 1;			//0: Carry Flag	
	BYTE Z : 1;			//1: Zero Flag
	BYTE I : 1;			//2: Interrupt disable
	BYTE D : 1;			//3: Decimal mode
	BYTE B : 1;			//4: Break
	BYTE Unused : 1;	//5: Unused
	BYTE V : 1;			//6: Overflow
	BYTE N : 1;			//7: Negative
};

class CPU
{
private:
	std::map<BYTE, std::string> instructionName;
	void InitInstructionName();

public:
	long long tick;

	// Registor
	BYTE	A;		// Accumulator
	BYTE	X;		// Index Registor
	BYTE	Y;
	BYTE	SP;		// Stack Pointer
	WORD	PC;		// program control

	// Processor Status : Flag
	union
	{
		BYTE _PS;
		StatusFlags Flag;
	};

	// for debug
	BYTE lastInst;
	bool enableLog;
	//std::string GetInstName(BYTE opcode);

public:
	CPU();
	~CPU();

	void Reset();
	void Reset(Memory& mem);
	void Reboot(Memory& mem);

	void SetPCAddress(WORD addr);
	int Run(Memory& mem, int cycle);

	void SetRegister(BYTE type, BYTE value);
	BYTE GetRegister(BYTE type);

	// Flag
	bool GetFlag(BYTE flag);
	void SetFlag(BYTE flag, bool set);

	void SetZeroNegative(BYTE Register);
	void SetCarryFlag(WORD value);
	void SetCarryFlagNegative(WORD value);
	void SetOverflow(BYTE oldv0, BYTE v0, BYTE v1);

	BYTE Fetch(Memory& mem, int& cycle);
	WORD FetchWord(Memory& mem, int& cycle);

	BYTE ReadByte(Memory& mem, WORD add, int& cycle);
	WORD ReadWord(Memory& mem, WORD addr, int& cycle);

	void WriteByte(Memory& mem, BYTE value, int addr, int& cycle);
	void WriteWord(Memory& mem, WORD value, int addr, int& cycle);

	//////////////////////////////////////////////////////////////////////////

	// memory Addressing mode
	// Zero page
	WORD addr_mode_ZP(Memory& mem, int& cycle);
	// Zero page + X
	WORD addr_mode_ZPX(Memory& mem, int& cycle);
	// Zero page + X
	WORD addr_mode_ZPY(Memory& mem, int& cycle);
	// ABS
	WORD addr_mode_ABS(Memory& mem, int& cycle);
	// ABS + X
	WORD addr_mode_ABSX(Memory& mem, int& cycle);
	// ABS + X : Page 넘어가는것 무시
	WORD addr_mode_ABSX_NoPage(Memory& mem, int& cycle);
	// ABS + Y
	WORD addr_mode_ABSY(Memory& mem, int& cycle);
	// ABS + Y : Page 넘어가는것 무시
	WORD addr_mode_ABSY_NoPage(Memory& mem, int& cycle);

	// Indexed indirect X
	WORD addr_mode_INDX(Memory& mem, int& cycle);
	// Indexed indirect Y
	WORD addr_mode_INDY(Memory& mem, int& cycle);

	//////////////////////////////////////////////////////////////////////////

	void PushStackByte(Memory& mem, BYTE value, int& cycle);
	void PushStackWord(Memory& mem, WORD value, int& cycle);
	BYTE PopStackByte(Memory& mem, int& cycle);
	WORD PopStackWord(Memory& mem, int& cycle);

	//////////////////////////////////////////////////////////////////////////

	void LoadToRegister(Memory& mem, int& cycle, BYTE& reg);
	void LoadToRegisterFromZP(Memory& mem, int& cycle, BYTE& reg);

	WORD GetStackAddress();

	//////////////////////////////////////////////////////////////////////////	Arithmetic

	void Execute_ADC(BYTE v);
	void Execute_SBC(BYTE v);
	void Execute_CMP(BYTE v);
	void Execute_CPX(BYTE v);
	void Execute_CPY(BYTE v);
	void Execute_ASL(BYTE &v, int &cycle);
	void Execute_LSR(BYTE& v, int& cycle);
	void Execute_ROL(BYTE& v, int& cycle);
	void Execute_ROR(BYTE& v, int& cycle);

	void Execute_BRANCH(bool v, bool condition, Memory& mem, int& cycle);

	//////////////////////////////////////////////////////////////////////////	Snapshot

	void Dump(FILE* fp);
	void LoadDump(FILE* fp);

};

#endif