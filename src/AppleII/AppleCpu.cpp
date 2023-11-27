
/*
	MOS 6502 CPU Emulator
*/
#include <assert.h>
#include "AppleCPU.h"
#include "AppleMem.h"
#include "./Tools/Log.h"

#define USEOLD	0

CPU::CPU()
{
	DEBUG_PRINTLN("Construct CPU");
	tick = 0;
	InitInstructionName();
	Reset();
	enableLog = false;
}

CPU::~CPU()
{
}

void CPU::InitInstructionName()
{
/*
	constexpr int instNum = 151;
	BYTE opcode[instNum] = {
		0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1, 0xB1, 0xA2, 0xA6, 0xB6, 0xAE, 0xBE, 0xA0, 0xA4, 0xB4, 0xAC,
		0xBC, 0xEA, 0x20, 0x4C, 0x6C, 0x60, 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91, 0x86, 0x96, 0x8E, 0x84,
		0x94, 0x8C, 0xBA, 0x9A, 0X48, 0x68, 0x28, 0x08, 0x29, 0x25, 0x35, 0x2D, 0x3D, 0x39, 0x21, 0x31, 0x09,
		0x05, 0x15, 0x0D, 0x1D, 0x19, 0x01, 0x11, 0x49, 0x45, 0x55, 0x4D, 0x5D, 0x59, 0x41, 0x51, 0x24, 0x2C,
		0xAA, 0xA8, 0x8A, 0x98, 0xE8, 0xC8, 0xCA, 0x88, 0xE6, 0xF6, 0xEE, 0xFE, 0xC6, 0xD6, 0xCE, 0xDE, 0x69,
		0x65, 0x75, 0x6D, 0x7D, 0x79, 0x61, 0x71, 0xE9, 0xE5, 0xF5, 0xED, 0xFD, 0xF9, 0xE1, 0xF1, 0xC9, 0xC5,
		0xD5, 0xCD, 0xDD, 0xD9, 0xC1, 0xD1, 0xE0, 0xE4, 0xEC, 0XC0, 0XC4, 0XCC, 0x0A, 0x06, 0x16, 0x0E, 0x1E,
		0x4A, 0x46, 0x56, 0x4E, 0x5E, 0x2A, 0x26, 0x36, 0x2E, 0x3E, 0x6A, 0x66, 0x76, 0x6E, 0x7E, 0x00, 0x40,
		0x90, 0xB0, 0xF0, 0x30, 0xD0, 0x10, 0x50, 0x70, 0x18, 0xD8, 0x58, 0xB8, 0x38, 0xF8, 0x78,
	};

	std::string inst_str[instNum] = {
		"LDA_IM", "LDA_ZP", "LDA_ZPX", "LDA_ABS", "LDA_ABSX", "LDA_ABSY", "LDA_INDX", "LDA_INDY",
		"LDX_IM", "LDX_ZP", "LDX_ZPY", "LDX_ABS", "LDX_ABSY", "LDY_IM", "LDY_ZP", "LDY_ZPX",
		"LDY_ABS", "LDY_ABSX", "NOP", "JSR", "JMP_ABS", "JMP_IND", "RTS", "STA_ZP", "STA_ZPX",
		"STA_ABS", "STA_ABSX", "STA_ABSY", "STA_INDX", "STA_INDY", "STX_ZP", "STX_ZPY", "STX_ABS", "STY_ZP",
		"STY_ZPX", "STY_ABS", "TSX", "TXS", "PHA", "PLA", "PLP", "PHP", "AND_IM", "AND_ZP", "AND_ZPX",
		"AND_ABS", "AND_ABSX", "AND_ABSY", "AND_INDX", "AND_INDY", "ORA_IM", "ORA_ZP", "ORA_ZPX", "ORA_ABS",
		"ORA_ABSX", "ORA_ABSY", "ORA_INDX", "ORA_INDY", "EOR_IM", "EOR_ZP", "EOR_ZPX", "EOR_ABS", "EOR_ABSX",
		"EOR_ABSY", "EOR_INDX", "EOR_INDY", "BIT_ZP", "BIT_ABS", "TAX", "TAY", "TXA", "TYA",
		"INX", "INY", "DEX", "DEY", "INC_ZP", "INC_ZPX", "INC_ABS", "INC_ABSX", "DEC_ZP", "DEC_ZPX",
		"DEC_ABS", "DEC_ABSX", "ADC_IM", "ADC_ZP", "ADC_ZPX", "ADC_ABS", "ADC_ABSX", "ADC_ABSY", "ADC_INDX",
		"ADC_INDY", "SBC_IM", "SBC_ZP", "SBC_ZPX", "SBC_ABS", "SBC_ABSX", "SBC_ABSY", "SBC_INDX", "SBC_INDY",
		"CMP_IM", "CMP_ZP", "CMP_ZPX", "CMP_ABS", "CMP_ABSX", "CMP_ABSY", "CMP_INDX", "CMP_INDY", "CPX_IM",
		"CPX_ZP", "CPX_ABS", "CPY_IM", "CPY_ZP", "CPY_ABS", "ASL", "ASL_ZP", "ASL_ZPX", "ASL_ABS",
		"ASL_ABSX", "LSR", "LSR_ZP", "LSR_ZPX", "LSR_ABS", "LSR_ABSX", "ROL", "ROL_ZP", "ROL_ZPX", "ROL_ABS",
		"ROL_ABSX", "ROR", "ROR_ZP", "ROR_ZPX", "ROR_ABS", "ROR_ABSX", "BRK", "RTI", "BCC", "BCS",
		"BEQ", "BMI", "BNE", "BPL", "BVC", "BVS", "CLC", "CLD", "CLI", "CLV", "SEC", "SED", "SEI",
	};
*/
//	for(int i=0; i<instNum; i++) 
//		instructionName.insert(std::make_pair(opcode[i], inst_str[i].c_str()));
}

/*
std::string CPU::GetInstName(BYTE opcode)
{
	//return instructionName[opcode];
}
*/
void CPU::Reset()
{
	A = 0;
	X = 0;
	Y = 0;
	_PS = 0;				// processor status (flags)
	SP = STACK_POS;	// Stack pointer
	PC = PC_START;		// program control
}

// reset all register
void CPU::Reset(Memory &mem)
{
	A = 0;
	X = 0;
	Y = 0;
	//PS = 0;				// processor status (flags)
	_PS = (_PS | FLAG_INTERRUPT_DISABLE) & ~FLAG_DECIMAL_MODE;
	SP = STACK_POS;	// Stack pointer
	// ROM 로드후 정해짐
	PC = mem.ReadByte(0xFFFC) | (mem.ReadByte(0xFFFD) << 8);
	tick = 0;
}

void CPU::Reboot(Memory& mem)
{
	mem.WriteByte(0x3F4, 0);
	PC = mem.ReadByte(0xFFFC) | (mem.ReadByte(0xFFFD) << 8);
	SP = 0xFD;
	Flag.I = 1;
	Flag.Unused = 1;

	mem.ResetRam();
// 	mem.WriteByte(0x4D, 0xAA);   // Joust crashes if this memory location equals zero
// 	mem.WriteByte(0xD0, 0xAA);   // Planetoids won't work if this memory location equals zero
}
void CPU::SetPCAddress(WORD addr)
{
	PC = addr;
}

void CPU::SetRegister(BYTE type, BYTE value)
{
	switch (type)
	{
		case REGISTER_A :
			A = value;
			break;
		case REGISTER_X:
			X = value;
			break;
		case REGISTER_Y:
			Y = value;
			break;
	}
}

BYTE CPU::GetRegister(BYTE type)
{
	switch (type)
	{
	case REGISTER_A:
		return A;
	case REGISTER_X:
		return X;
	case REGISTER_Y:
		return Y;
	}

	assert(-1);
	return 0;
}

void CPU::SetFlag(BYTE flag, bool set)
{
	if (set)
		_PS |= flag;
	else
		_PS &= ~flag;
}

bool CPU::GetFlag(BYTE flag)
{
	return _PS & flag;
}

void CPU::SetZeroNegative(BYTE Register)
{
	//SetFlag(FLAG_ZERO, Register == 0);
	Flag.Z = (Register == 0);

	//SetFlag(FLAG_NEGATIVE, Register & FLAG_NEGATIVE);
	Flag.N = (Register & FLAG_NEGATIVE) > 0;
}

void CPU::SetCarryFlag(WORD value)
{
	Flag.C = (value & 0xFF00) > 0;
	//SetFlag(FLAG_CARRY, (value & 0xFF00) > 0);

}

void CPU::SetCarryFlagNegative(WORD value)
{
	Flag.C = (value < 0x100);
}

void CPU::SetOverflow(BYTE oldv0, BYTE v0, BYTE v1)
{
	bool sign0 = !((oldv0 ^ v1) & FLAG_NEGATIVE);	// 계산전 부호
	bool sign1 = ((v0 ^ v1) & FLAG_NEGATIVE);		// 계산후 부호

	// Overflow는 같은 부호를 더했는데 다른 부호가 나오면 Overflow이다
	//SetFlag(FLAG_OVERFLOW, (sign0 != sign1));
	//Flag.V = (sign0 != sign1);
	Flag.V = sign0 && sign1;
}

BYTE CPU::Fetch(Memory& mem, int &cycle)
{
	BYTE c = mem.ReadByte(PC++);
	cycle--;
	return c;
}

WORD CPU::FetchWord(Memory& mem, int& cycle)
{
	BYTE c0 = mem.ReadByte(PC++);
	BYTE c1 = mem.ReadByte(PC++);

	// 엔디안에 따라 c0 <--> c1해야 할수도 있다
	WORD w = (c1 << 8) | c0;
	cycle-=2;
	return w;
}

// 메모리에서 읽는데 cycle소모 x / PC무관 할때 (Zero page같은것)
BYTE CPU::ReadByte(Memory& mem, WORD addr, int& cycle)
{
	BYTE c = mem.ReadByte(addr);
	cycle--;
	return c;
}

WORD CPU::ReadWord(Memory& mem, WORD addr, int& cycle)
{
	WORD c = mem.ReadWord(addr);
	cycle -= 2;
	return c;
}

void CPU::WriteByte(Memory& mem, BYTE value, int addr, int& cycle)
{
	mem.WriteByte(addr, value);
	cycle --;
}

void CPU::WriteWord(Memory& mem, WORD value, int addr, int& cycle)
{
	mem.WriteWord(value, addr);
	cycle-=2;
}

// 이거를 써야하는 이유는 
/*
	SP는 1byte이고 Stack 메모리는 0x01FF -> 0x0100까지 256 Byte이므로
	Address는 WORD이고 스택의 메모리 위치는 감소하기 때문에 이렇게 계산해햐함
*/
WORD CPU::GetStackAddress()
{
	WORD sp = STACK_ADDRESS + SP;
	return sp;
}

// Byte를 Stack에 Push
void CPU::PushStackByte(Memory& mem, BYTE value, int& cycle)
{
	WriteByte(mem, value, GetStackAddress(), cycle);
	SP--;
	cycle--;
}

// Word를 Stack에 Push
void CPU::PushStackWord(Memory& mem, WORD value, int& cycle)
{
	// Hi byte 먼저
	WriteByte(mem, value >> 8, GetStackAddress(), cycle);
	SP--;
	// Lo byte 나중에
	WriteByte(mem, value & 0xFF, GetStackAddress(), cycle);
	SP--;
}

// 스택에서 1 byte POP
BYTE CPU::PopStackByte(Memory& mem, int& cycle)
{
 	SP++;
 	BYTE popbyte = ReadByte(mem, GetStackAddress(), cycle);
	cycle--;
	return popbyte;
}

// Stack에서 Word pop
WORD CPU::PopStackWord(Memory& mem, int& cycle)
{
	SP++;
	BYTE lo = ReadByte(mem, GetStackAddress(), cycle);
	SP++;
	BYTE hi = ReadByte(mem, GetStackAddress(), cycle);
	WORD popWord = lo | (hi << 8);
	cycle--;

	return popWord;
}

int CPU::Run(Memory &mem, int cycle)
{
	const int CyclesRequested = cycle;

	while (cycle > 0)
	{
		int prevcycle = cycle;

		WORD prevPC = PC;
		// 여기에서 cycle 하나 소모
		BYTE inst = Fetch(mem, cycle);
		lastInst = inst;

		if (enableLog)
		{
			printf("A:[%2X] X:[%2X] Y:[%2X] PC:[%4X] ", A, X, Y, prevPC);
			printf("INST : [%2X] / C:[%d] Z:[%d] I:[%d] D:[%d] B:[%d] U:[%d] V:[%d] N:[%d]\n", inst,
				Flag.C, Flag.Z, Flag.I, Flag.D, Flag.B, Flag.Unused, Flag.V, Flag.N);
		}

		switch (inst)
		{
			case LDA_IM: // 2 cycle
			{
				//LoadToRegister(mem, cycle, A);
				A = Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ZP: // 3 cycle
			{
				// $0000 to $00FF
				// Zero page에서 읽어서 A로
				WORD addr = addr_mode_ZP(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ZPX : // 4 cycle
			{
				// Zero page의 주소와 X 레지스터를 더한 주소에서 읽어 A로..
				// X 레지스터에 $0F이고 LDA $80, X 이면 $80+$0F = $8F에서 A로 읽게됨
/*				
				BYTE zpa = Fetch(mem, cycle);
				zpa += X;
				cycle--;
*/
				WORD addr = addr_mode_ZPX(mem, cycle);
				// Zero page읽으면서 cycle 소모
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			// 절대 주소 지정을 사용하는 명령어는 대상 위치를 식별하기 위해 전체 16 비트 주소를 포함합니다.
			case LDA_ABS: // 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSX:// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				// 메모리엑세스 페이지를 넘어가면 추가 사이클이 소요됨 (하드웨어가 그렇게 만들어짐?)
/*				
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);
				
				WORD t = lo + X;
				if( t > 0xFF ) cycle--;
				WORD addr = (lo | (hi << 8)) + X;
*/				
// 				WORD addr = FetchWord(mem, cycle);
// 				if ( (addr + X) - addr >= 0xFF )
// 					cycle--;	// page 넘어감

				WORD addr = addr_mode_ABSX(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case LDA_ABSY:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
/*				
				BYTE lo = Fetch(mem, cycle);
				BYTE hi = Fetch(mem, cycle);

				WORD t = lo + Y;
				if (t > 0xFF) cycle--;
				WORD addr = (lo | (hi << 8)) + Y;
*/
				WORD addr = addr_mode_ABSY(mem, cycle);
				A = ReadByte(mem, addr, cycle);
				SetZeroNegative(A);

			}
			break;

			case LDA_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case LDA_INDY: // 5 ~ 6 cycle
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A = ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// LDX
			case LDX_IM:	// 2cycle
			{
				//LoadToRegister(mem, cycle, X);
				X = Fetch(mem, cycle);
				SetZeroNegative(X);
			}
			break;

			case LDX_ZP:	// 3cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
 				X = ReadByte(mem, addr, cycle);
 				SetZeroNegative(X);
			}
			break;

			case LDX_ZPY:	// 4cycle
			{
/*				
				BYTE zpage = Fetch(mem, cycle);
				zpage += Y;
				cycle--;
*/
				WORD addr = addr_mode_ZPY(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);
			}
			break;

			case LDX_ABS:	// 4cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			case LDX_ABSY:	// 4 ~ 5 cycle
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				X = ReadByte(mem, addr, cycle);
				SetZeroNegative(X);

			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case LDY_IM : // 2 cycle
			{
				Y = Fetch(mem, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZP: // 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ZPX : // 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABS : // 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			case LDY_ABSX : // 4~5 cycle
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				Y = ReadByte(mem, addr, cycle);
				SetZeroNegative(Y);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STA_ZP	:	// 3 cycle
			{
				// ZeroPage에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ZPX :	// 4 cycle
			{
				// ZP + X에 A레지스터 내용쓰기
				WORD addr = addr_mode_ZPX(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABS:	// 4 cycle
			{
				// WORD address에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSX:	// 5 cycle
			{
				// WORD address + X에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_ABSY:	// 5 cycle
			{
				// WORD address + Y에 A레지스터 내용 쓰기
				WORD addr = addr_mode_ABSY_NoPage(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				WriteByte(mem, A, addr, cycle);
			}
			break;

			case STA_INDY:	// 6 cycle
			{
				// ZeroPage에서 WORD address얻고 address + Y에 가르키는곳에 A레지스터 내용쓰기 
				BYTE zp = Fetch(mem, cycle);
				WORD addr = ReadWord(mem, zp, cycle);
				addr += Y;
				cycle--;
				WriteByte(mem, A, addr, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STX_ZP	:	// 3 cycle
			{
				// ZeroPage에 X레지스터 내용 쓰기
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			case STX_ZPY:	// 4 cycle
			{
				// ZP + Y에 X레지스터 내용쓰기
				WORD addr = addr_mode_ZPY(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			case STX_ABS:	// 4 cycle
			{
				// WORD address에 X레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
				WriteByte(mem, X, addr, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case STY_ZP:	// 3 cycle
			{
				// ZeroPage에 X레지스터 내용 쓰기
				WORD addr = addr_mode_ZP(mem, cycle);
				WriteByte(mem, Y, addr, cycle);

			}
			break;

			case STY_ZPX:	// 4 cycle
			{
				// ZP + X에 Y레지스터 내용쓰기
				WORD addr = addr_mode_ZPX(mem, cycle);
				WriteByte(mem, Y, addr, cycle);
			}
			break;

			case STY_ABS:	// 4 cycle
			{
				// WORD address에 Y레지스터 내용 쓰기
				WORD addr = addr_mode_ABS(mem, cycle);
				WriteByte(mem, Y, addr, cycle);
			}
			break;



			////////////////////////////////////////////////////////////////////////////// JUMP

			case JSR : // 6 cycle
			{
				// The JSR instruction pushes the address (minus one) of the return 
				// point on to the stack and then sets the program counter to the target memory address.
//   				WORD sr_addr = FetchWord(mem, cycle);
// 				// 스택에 PC-1을 Push
// 				PushStackWord(mem, PC - 1, cycle);
// 				PC = sr_addr;
// 				cycle--;

				BYTE lo = ReadByte(mem, PC, cycle);
				PC++;
				WORD address = lo | (ReadByte(mem, PC, cycle) << 8);

				PushStackWord(mem, PC, cycle);
				PC = address;
				cycle--;
			}
			break;

			case JMP_ABS :	// 3 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				PC = addr;
			}
			break;

			case JMP_IND :	// 5 cycle
			{
				WORD addr = FetchWord(mem, cycle);
				addr = ReadWord(mem, addr, cycle);
				PC = addr;
			}
			break;

			case RTS :	// 6 cycle
			{
				WORD addr = PopStackWord(mem, cycle);
				PC = addr + 1;
				cycle -= 2;
			}
			break;

			//////////////////////////////////////////////////////////////////////////////	STACK

			// Transfer (Stack Pointer) to X
			case TSX :	// 2 cycle
			{
				// 스택포인터를 X 레지스터로
				X = SP;
				cycle--;
				// Z / N flag
				SetZeroNegative(X);
			}
			break;

			// Transfer X to (Stack Pointer)
			case TXS :	// 2 cycle
			{
				// X레지스터를 SP로
				SP = X;
				cycle--;
			}
			break;

			// Pushes a copy of the accumulator on to the stack.
			case PHA :	// 3 cycle
			{
				// A 레지스터를 스택에 Push
				PushStackByte(mem, A, cycle);
			}
			break;

			// Pulls an 8 bit value from the stack and into the accumulator. 
			// The zero and negative flags are set as appropriate.
			case PLA :	// 4 cycle
			{
				// 스택에서 8비트를 pull --> A로
				A = PopStackByte(mem, cycle);
				cycle--;
				// Z / N flag
				SetZeroNegative(A);
			}
			break;

			// Pulls an 8 bit value from the stack and into the processor flags. 
			// The flags will take on new states as determined by the value pulled.
			case PLP :	// 4 cycle
			{
				// pop 8 bit를 --> PS (Flag) : 플레그들은 Pop된 값에의하여 새로운 플레그 상태를 갖음
#if USEOLD
				BYTE _PS = PopStackByte(mem, cycle);
				cycle--;
				_PS &= ~(FLAG_UNUSED | FLAG_BREAK);
				PS = 0;
				PS |= _PS;
				// B , Unused는 사용하지 않음
				Flag.B = 0;
				Flag.Unused = 0;
#else
				_PS = PopStackByte(mem, cycle) | FLAG_UNUSED;
				//SP++;
				//PS = ReadByte(mem, 0x100 + SP, cycle) | FLAG_UNUSED;
#endif
			}
			break;

			// Pushes a copy of the status flags on to the stack.
			case PHP :	// 3 cycle
			{
				// PS -> Stack에 Push
				BYTE __PS = _PS | FLAG_BREAK;
				PushStackByte(mem, __PS, cycle);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			case AND_IM :	// 2 cycle
			{
				A &= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ZP:	// 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ZPX:	// 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABS:	// 4cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABSX:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_ABSY:	// 4 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A &= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case AND_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A &= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case AND_INDY:	// 5 cycle / 페이지 넘어가면 1 cycle 추가
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A &= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;


			case ORA_IM :
			{
				A |= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A |= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case ORA_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A |= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case ORA_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A |= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;


			case EOR_IM:
			{
				A ^= Fetch(mem, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);
				SetZeroNegative(A);
			}
			break;

			case EOR_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			case EOR_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				A ^= ReadByte(mem, addr, cycle);

				SetZeroNegative(A);
			}
			break;

			// Bit Test Zero page
			case BIT_ZP:
			{
				// Zp에서 읽은 값과 A를 & 테스트 하고 플레그들을 셋팅 / Set if the result if the AND is zero
				// N 플레그는 7bit, Set to bit 7 of the memory value
				// V 플레그는 6Bit , Set to bit 6 of the memory value
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE R = ReadByte(mem, addr, cycle);

				Flag.Z = !(A & R);	
				Flag.N = (R & FLAG_NEGATIVE) != 0;
				Flag.V = (R & FLAG_OVERFLOW) != 0;
			}
			break;

			case BIT_ABS :
			{
				// Zp에서 읽은 값과 A를 & 테스트 하고 플레그들을 셋팅
				// N 플레그는 7bit / V 플레그는 6Bit
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE R = ReadByte(mem, addr, cycle);

				Flag.Z = !(A & R);
				Flag.N = (R & FLAG_NEGATIVE) != 0;
				Flag.V = (R & FLAG_OVERFLOW) != 0;

			}
			break;

			////////////////////////////////////////////////////////////////////////////// Register Transfer

			case TAX :
			{
				// Transfer Accumulator to X
				X = A;
				cycle--;
				SetZeroNegative(X);
			}
			break;

			case TAY:
			{
				//Transfer Accumulator to Y
				Y = A;
				cycle--;
				SetZeroNegative(Y);
			}
			break;

			case TXA:
			{
				// Transfer X to Accumulator
				A = X;
				cycle--;
				SetZeroNegative(A);
			}
			break;

			case TYA:
			{
				// Transfer Y to Accumulator
				A = Y;
				cycle--;
				SetZeroNegative(A);
			}
			break;


			////////////////////////////////////////////////////////////////////////////// increase / decrease

			case INX :	// 2 cycle
			{
				// Increment X Register / X,Z,N = X+1
				X++;
				cycle--;
				Flag.Z = (X == 0);
				Flag.N = (X & FLAG_NEGATIVE) != 0;
			}
			break;

			case INY :	// 2 cycle
			{
				// Increment Y Register / Y,Z,N = Y+1
				Y++;
				cycle--;
				Flag.Z = (Y == 0);
				Flag.N = (Y & FLAG_NEGATIVE) != 0;
			}
			break;
			case DEX:	// 2 cycle
			{
				// Decrease X Register / X,Z,N = X+1
				X--;
				cycle--;
				Flag.Z = (X == 0);
				Flag.N = (X & FLAG_NEGATIVE) != 0;

			}
			break;

			case DEY:	// 2 cycle
			{
				// Decrement Y Register / Y,Z,N = Y+1
				Y--;
				cycle--;
				Flag.Z = (Y == 0);
				Flag.N = (Y & FLAG_NEGATIVE) != 0;

			}
			break;

			case INC_ZP:
			{
				// Increment Memory by One / M + 1 -> M
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case INC_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case INC_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);				
			}
			break;
			case INC_ABSX:
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v++;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);				
			}
			break;
			case DEC_ZP:// 5 cycle
			{
				// Decrement Memory by One / M - 1 -> M
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);

			}
			break;
			case DEC_ZPX: // 6 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;
			case DEC_ABS:	// 6 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;

			case DEC_ABSX:	// 7 cycle
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				v--;
				cycle--;
				WriteByte(mem, v, addr, cycle);
				SetZeroNegative(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////	Arithmetic

			// Add with Carry
			// This instruction adds the contents of a memory location to the accumulator together with 
			// the carry bit.If overflow occurs the carry bit is set, this enables multiple byte addition to be performed.
			case ADC_IM	:	// 2 cycle
			{
				// A + M + C -> A, C
				BYTE v = Fetch(mem, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ZP:	// 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ZPX:	// 4 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABS:	// 4 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABSX:	// 4~5 cycle
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_ABSY:	// 4~5 cycle
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_INDX:	// 6 cycle
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			case ADC_INDY:	// 5~6 cycle
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ADC(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// SBC - Subtract with Carry
			// A, Z, C, N = A - M - (1 - C)
			case SBC_IM	:	// 2 cycle
			{
				BYTE v = Fetch(mem, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_ZP:	// 3 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			case SBC_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_SBC(v);
			}
			break;

			////////////////////////////////////////////////////////////////////////////// Compare

			// Z,C,N = A-M
			// This instruction compares the contents of the accumulator with another memory held 
			// valueand sets the zeroand carry flags as appropriate.
			case CMP_IM:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;
			
			case CMP_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABSX:
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_ABSY:
			{
				WORD addr = addr_mode_ABSY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			case CMP_INDX:
			{
				WORD addr = addr_mode_INDX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}

			break;

			case CMP_INDY:
			{
				WORD addr = addr_mode_INDY(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CMP(v);
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// Compare X Register
			// Z, C, N = X - M
			// This instruction compares the contents of the X register with another memory
			// held value and sets the zero and carry flags as appropriate.
			case CPX_IM	:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CPX(v);
			}
			break;

			case CPX_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPX(v);
			}
			break;

			case CPX_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPX(v);
			}
			break;

			case CPY_IM:
			{
				BYTE v = Fetch(mem, cycle);
				Execute_CPY(v);
			}
			break;

			case CPY_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPY(v);
			}
			break;

			case CPY_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_CPY(v);
			}
			break;

			////////////////////////////////////////////////////////////////////////////// SHIFT

			// Arithmetic Shift Left
			case ASL : // 2 cycle
			{
				// A,Z,C,N = M*2 or M,Z,C,N = M*2
				// Carry Bit 계산을 먼저해야한다. Shift할 값자체가 -(NEG)인 경우 왼쪽 shift는 Carry를 일으키기 때문
				Execute_ASL(A, cycle);
			}
			break;

			case ASL_ZP: // 5 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ASL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;

			case ASL_ZPX: // 6 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ASL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;

			case ASL_ABS: // 6 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ASL(v, cycle);
				WriteByte(mem, v, addr, cycle);

			}
			break;

			case ASL_ABSX: // 7 cycle
			{
				WORD addr = addr_mode_ABSX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ASL(v, cycle);
				WriteByte(mem, v, addr, cycle);

			}
			break;

			// Logical Shift Right : A,C,Z,N = A/2 or M,C,Z,N = M/2
			// Each of the bits in A or M is shift one place to the right. 
			// The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
			// Carry Flag :	Set to contents of old bit 0
			case LSR:	// 2 cycle
			{
				Execute_LSR(A, cycle);
			}
			break;
			case LSR_ZP:	// 5 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_LSR(v, cycle);
				WriteByte(mem, v, addr, cycle);
}
			break;
			case LSR_ZPX:	// 6 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_LSR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case LSR_ABS:	// 6 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_LSR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case LSR_ABSX:	// 7 cycle
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_LSR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;

			// Move each of the bits in either A or M one place to the left. 
			// Bit 0 is filled with the current value of the carry flag whilst 
			// the old bit 7 becomes the new carry flag value.
			case ROL:	// 2 cycle
			{
				Execute_ROL(A, cycle);
			}
			break;
			case ROL_ZP:	// 5 cycle
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROL_ZPX:	// 6 cycle
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROL_ABS:	// 6 cycle
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROL_ABSX:	// 7 cycle
			{
				// 여기는 ABS No page
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROL(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;

			// Rotate Right
			// Move each of the bits in either A or M one place to the right.
			// Bit 7 is filled with the current value of the carry flag whilst 
			// the old bit 0 becomes the new carry flag value.
			case ROR:
			{
				Execute_ROR(A, cycle);
			}
			break;
			case ROR_ZP:
			{
				WORD addr = addr_mode_ZP(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROR_ZPX:
			{
				WORD addr = addr_mode_ZPX(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROR_ABS:
			{
				WORD addr = addr_mode_ABS(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;
			case ROR_ABSX:
			{
				WORD addr = addr_mode_ABSX_NoPage(mem, cycle);
				BYTE v = ReadByte(mem, addr, cycle);
				Execute_ROR(v, cycle);
				WriteByte(mem, v, addr, cycle);
			}
			break;


			////////////////////////////////////////////////////////////////////////////// Branches

			// Branch if carry flag clear
			// If the carry flag is clear then add the relative displacement to the program 
			// counter to cause a branch to a new location.
			case BCC :
			{
				Execute_BRANCH(Flag.C, false, mem, cycle);
/*
				BYTE offset = Fetch(mem, cycle);
				if (!Flag.C)
				{
					// Page를 넘어가면 Cycle 증가
					BYTE lo = PC & 0x00FF;
					WORD t = lo + (char)offset;
					if (t > 0xFF) cycle--;

					PC += (char)offset;
					cycle--;
				}*/
			}
			break;

			// Branch if Carry Set
			// If the carry flag is set then add the relative displacement to the program 
			// counter to cause a branch to a new location.
			// BCC 반대
			case BCS: // 2 ~ 4 cycle
			{
				Execute_BRANCH(Flag.C, true, mem, cycle);
			}
			break;

			// Branch if Equal
			// If the zero flag is set then add the relative displacement to the program counter 
			// to cause a branch to a new location.
			// 2 (+1 if branch succeeds +2 if to a new page)
			case BEQ:	// 2 cycle + Zero이면 1 cycle추가 + Page넘어가면 1 cycle 추가
			{
				Execute_BRANCH(Flag.Z, true, mem, cycle);
			}
			break;

			// If the zero flag is clear then add the relative displacement 
			// to the program counter to cause a branch to a new location.
			// BEQ랑 반대
			case BNE:
			{
				Execute_BRANCH(Flag.Z, false, mem, cycle);
			}
			break;

			// Branch if negative flag set
			case BMI:
			{
				Execute_BRANCH(Flag.N, true, mem, cycle);
			}
			break;

			// Branch if negative flag clear
			case BPL:
			{
				Execute_BRANCH(Flag.N, false, mem, cycle);
			}
			break;

			// Branch if overflow flag clear
			case BVC:
			{
				Execute_BRANCH(Flag.V, false, mem, cycle);
			}
			break;

			// Branch if overflow flag set
			case BVS:
			{
				Execute_BRANCH(Flag.V, true, mem, cycle);
			}
			break;

			////////////////////////////////////////////////////////////////////////////// Status Flag Changes

			// Clear carry flag
			case CLC :	// 2 cycle
			{
				Flag.C = 0;
				cycle--;
			}
			break;

			// Clear Decimal Mode
			case CLD:	// 2 cycle
			{
				Flag.D = 0;
				cycle--;
			}
			break;

			// Clear Interrupt Disable
			case CLI:	// 2 cycle
			{
				Flag.I = 0;
				cycle--;
			}
			break;

			// Clear Overflow Flag
			case CLV:	// 2 cycle
			{
				Flag.V = 0;
				cycle--;
			}
			break;

			// Set carry flag
			case SEC:	// 2 cycle
			{
				Flag.C = 1;
				cycle--;
			}
			break;

			// Set decimal mode flag
			case SED:	// 2 cycle
			{
				Flag.D = 1;
				cycle--;
			}
			break;

			// Set interrupt disable flag
			case SEI:	// 2 cycle
			{
				Flag.I = 1;
				cycle--;
			}
			break;

			//////////////////////////////////////////////////////////////////////////////

			// BRK 명령은 인터럽트 요청의 생성을 강제한다.
			// 프로그램 카운터 및 프로세서 상태가 스택에서 푸시된 다음 $ FFFE/F의 IRQ 인터럽트 벡터가 
			// PC에로드되고 상태의 중단 플래그가 1로 설정됩니다.
			case BRK :	// 7 cycle
			{
#if USEOLD
				// PC Push
				// BRK는 PC를 +1하지 않고 +2한다고 함. 그래서 PC+1 push
				// https://www.c64-wiki.com/wiki/BRK
				PushStackWord(mem, PC+1, cycle);

				// SP Push
				BYTE _PS = PS | FLAG_BREAK | FLAG_UNUSED;
				PushStackByte(mem, _PS, cycle);

				WORD interruptVector = ReadWord(mem, 0xFFFE, cycle);
				PC = interruptVector;
				Flag.B = 1;
				Flag.I = 1;
#else
				PC++;
				WriteByte(mem, ((PC) >> 8) & 0xFF, 0x100 + SP, cycle);
				SP--;
				WriteByte(mem, PC & 0xFF, 0x100 + SP, cycle);
				SP--;
				WriteByte(mem, _PS | FLAG_BREAK, 0x100 + SP, cycle);
				SP--;
				Flag.I = 1;
				Flag.D = 0;
				PC = ReadByte(mem, 0xFFFE, cycle) | ReadByte(mem, 0xFFFF, cycle) << 8;
#endif
				printf("BREAK!! : %x\n", PC);
			}
			break;

			// Return from Interrupt
			// RTI 명령은 인터럽트 처리 루틴의 끝에서 사용됩니다.
			// 프로그램 카운터 뒤에 오는 스택에서 프로세서 플래그를 가져옵니다.
			case RTI :	// 6 cycle
			{
				//BYTE PS = PopStackByte(mem, cycle);
				//PC = PopStackWord(mem, cycle);

				SP++;
				_PS = ReadByte(mem, 0x100 + SP, cycle);
				SP++;
				PC = ReadByte(mem, 0x100 + SP, cycle);
				SP++;
				PC |= ReadByte(mem, 0x100 + SP, cycle) << 8;
				cycle -= 5;
			}
			break;

			case APPLE_NOP :
				cycle--;
			break;

			//////////////////////////////////////////////////////////////////////////////

			default:
				printf("Unknown instruction : %x\n", inst);
				//throw -1;
				break;
		}

		tick+= prevcycle-cycle;
	}

	return CyclesRequested - cycle;
}

void CPU::LoadToRegister(Memory& mem, int& cycle, BYTE &reg)
{
	reg = Fetch(mem, cycle);
	SetZeroNegative(reg);
}

// ZP에 있는 값을 레지스터에 로드
void CPU::LoadToRegisterFromZP(Memory& mem, int& cycle, BYTE& reg)
{
	BYTE zpa = Fetch(mem, cycle);
	reg = ReadByte(mem, zpa, cycle);
	SetZeroNegative(reg);
}

////////////////////////////////////////////////////////////////////////////// memory addressing mode

// ZeroPage
WORD  CPU::addr_mode_ZP(Memory &mem, int &cycle)
{
	BYTE address = Fetch(mem, cycle);
	return address;
}

// Zero page + X
WORD CPU::addr_mode_ZPX(Memory& mem, int& cycle)
{
	BYTE address = Fetch(mem, cycle) + X;
	cycle--;
	return address;
}

// Zero page + X
WORD CPU::addr_mode_ZPY(Memory& mem, int& cycle)
{
	BYTE address = Fetch(mem, cycle) + Y;
	cycle--;
	return address;
}

// ABS
WORD CPU::addr_mode_ABS(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	return address;
}

// ABS + X
WORD CPU::addr_mode_ABSX(Memory& mem, int& cycle)
{
#if 1
	BYTE lo = Fetch(mem, cycle);
	BYTE hi = Fetch(mem, cycle);
	WORD t = lo + X;
	if (t & 0xFF00) cycle--;
	WORD address = (lo | (hi << 8)) + X;
	return address;

#else
	WORD address = Fetch(mem, cycle);
	if ((address + X) & 0xFF00)
		cycle--;
	address |= Fetch(mem, cycle) << 8;
	address += X;
	return address;
#endif

}

// ABS + X : Page 넘어가는것 무시(그냥 하드웨어가 이렇게 생김)
WORD CPU::addr_mode_ABSX_NoPage(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	address += X;
	cycle--;
	return address;
}

// ABS + Y
WORD CPU::addr_mode_ABSY(Memory& mem, int& cycle)
{
	BYTE lo = Fetch(mem, cycle);
	BYTE hi = Fetch(mem, cycle);

	WORD t = lo + Y;
	if (t > 0xFF) cycle--;
	WORD address = (lo | (hi << 8)) + Y;
	return address;
}

WORD CPU::addr_mode_ABSY_NoPage(Memory& mem, int& cycle)
{
	WORD address = FetchWord(mem, cycle);
	address += Y;
	cycle--;
	return address;
}

WORD CPU::addr_mode_INDX(Memory& mem, int& cycle)
{
	BYTE t = Fetch(mem, cycle);
	BYTE inx = t + X;
	cycle--;
	WORD address = ReadWord(mem, inx, cycle);
	return address;
}


WORD CPU::addr_mode_INDY(Memory& mem, int& cycle)
{
#if 1
	// zero page에서 word 읽고 Y레지스터와 더한 주소의 1바이트를 A에 로드
	// 읽을 주소가 page를 넘으면 1사이클 감소
	BYTE addr = Fetch(mem, cycle);
	BYTE lo = ReadByte(mem, addr, cycle);
	BYTE hi = ReadByte(mem, addr + 1, cycle);

	WORD t = lo + Y;
	if (t > 0xFF) cycle--;	// page 넘어감

	WORD index_addr = ((hi << 8) | lo) + Y;
	return index_addr;
#else
	BYTE addr = Fetch(mem, cycle);
	WORD EAddr = ReadWord(mem, addr, cycle);
	WORD EAddrY = EAddr + Y;
	const bool CrossedPageBoundary = (EAddr ^ EAddrY) >> 8;
	if (CrossedPageBoundary)
	{
		cycle--;
	}
	return EAddrY;
#endif
}

void CPU::Execute_ADC(BYTE v)
{
#if !USEOLD
	BYTE oldA = A;
	WORD Result = A + v + Flag.C;
	// Decimal mode
	if (Flag.D)
		Result += ((((Result + 0x66) ^ A ^ v) >> 3) & 0x22) * 3;
	A = (Result & 0xFF);
	SetZeroNegative(A);
	SetCarryFlag(Result);
 	SetOverflow(oldA, A, v);
#else
	// Decimal mode 무시하면 Lode runner에서 점수 Hex로 나옴

	WORD result = A + v + Flag.C;
	Flag.V = ((result ^ A) & (result ^ v) & 0x0080) != 0;
	if (Flag.D)
		result += ((((result + 0x66) ^ A ^ v) >> 3) & 0x22) * 3;
	Flag.C = result > 0xFF;
	A = result & 0xFF;
	SetZeroNegative(A);
#endif
}

void CPU::Execute_SBC(BYTE v)
{
#if !USEOLD
	Execute_ADC(~v);
#else
	v ^= 0xFF;
	if (Flag.D)
		v -= 0x0066;
	WORD result = A + v + (Flag.C);
	Flag.V = ((result ^ A) & (result ^ v) & 0x0080) != 0;
	if (Flag.D)
		result += ((((result + 0x66) ^ A ^ v) >> 3) & 0x22) * 3;
	Flag.C = result > 0xFF;
	A = result & 0xFF;
	SetZeroNegative(A);
#endif
}

void CPU::Execute_CMP(BYTE v)
{
	//WORD t = A - v;
	// 	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	// 	Flag.Z = A == v;					// Set if A = M
	// 	Flag.C = A >= v;					// Set if A >= M

	Flag.Z = ((A - v) & 0xFF) == 0;
	Flag.N = ((A - v) & FLAG_NEGATIVE) != 0;
	Flag.C = (A >= v) != 0;
}

void CPU::Execute_CPX(BYTE v)
{
	//WORD t = X - v;
	// 	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	// 	Flag.Z = X == v;					// Set if X = M
	// 	Flag.C = X >= v;					// Set if X >= M

	Flag.Z = ((X - v) & 0xFF) == 0;
	Flag.N = ((X - v) & FLAG_NEGATIVE) != 0;
	Flag.C = (X >= v) != 0;
}

void CPU::Execute_CPY(BYTE v)
{
	//WORD t = Y - v;
	// 	Flag.N = (t & FLAG_NEGATIVE) > 0;	// Set if bit 7 of the result is set
	// 	Flag.Z = Y == v;					// Set if Y = M
	// 	Flag.C = Y >= v;					// Set if Y >= M

	Flag.Z = ((Y - v) & 0xFF) == 0;
	Flag.N = ((Y - v) & FLAG_NEGATIVE) != 0;
	Flag.C = (Y >= v) != 0;
}

void CPU::Execute_ASL(BYTE &v, int &cycle)
{
	Flag.C = (v & FLAG_NEGATIVE) > 0;
	v = v << 1;
	cycle--;
	SetZeroNegative(v);
}

void CPU::Execute_LSR(BYTE& v, int& cycle)
{
	Flag.C = (v & 0x01);
	v = v >> 1;
	cycle--;
	SetZeroNegative(v);
}

/*
			   +------------------------------+
			   |         M or A               |
			   |   +-+-+-+-+-+-+-+-+    +-+   |
  Operation:   +-< |7|6|5|4|3|2|1|0| <- |C| <-+         N Z C I D V
				   +-+-+-+-+-+-+-+-+    +-+             / / / _ _ _
*/
void CPU::Execute_ROL(BYTE& v, int& cycle)
{
	// 이전의 carry flag값을 Shift후의 0bit에 채워준다
	BYTE oldcarry = Flag.C ? 0x01 : 0x00;
	Flag.C = (v & FLAG_NEGATIVE) > 0;
	v <<= 1;
	v |= oldcarry;
	cycle--;
	SetZeroNegative(v);
}

/*
			   +------------------------------+
			   |                              |
			   |   +-+    +-+-+-+-+-+-+-+-+   |
  Operation:   +-> |C| -> |7|6|5|4|3|2|1|0| >-+         N Z C I D V
				   +-+    +-+-+-+-+-+-+-+-+             / / / _ _ _
*/
void CPU::Execute_ROR(BYTE& v, int& cycle)
{
	// 최하비트가 1인가? -> 다음 캐리비트로 설정
	BYTE oldcarry = (v & FLAG_CARRY) > 0;
	v = v >> 1;
	// 이전 Carry가 1이면 NEGATIVE 채움
	v |= (Flag.C ? FLAG_NEGATIVE : 0);
	cycle--;
	Flag.C = oldcarry;
	SetZeroNegative(v);
}


void CPU::Execute_BRANCH(bool v, bool condition, Memory &mem, int &cycle)
{
	SBYTE offset = (SBYTE)Fetch(mem, cycle);
	if (v == condition)
	{
#if !USEOLD
		// Page를 넘어가면 Cycle 증가
		BYTE lo = PC & 0x00FF;
		WORD t = lo + (SBYTE)offset;
		if (t > 0xFF) cycle--;

		PC += (SBYTE)offset;
		cycle--;
#else
		if (offset & FLAG_NEGATIVE)
			offset |= 0xFF00;  // jump backward
		if (((PC & 0xFF) + offset) & 0xFF00)  // page crossing
			cycle--;
		PC += offset;
		cycle--;
#endif
	}
}


void CPU::Dump(FILE* fp)
{
	fwrite(&tick, 1, sizeof(long long), fp);

	fwrite(&A, 1, sizeof(BYTE), fp);
	fwrite(&X, 1, sizeof(BYTE), fp);
	fwrite(&Y, 1, sizeof(BYTE), fp);
	fwrite(&SP, 1, sizeof(BYTE), fp);
	fwrite(&_PS, 1, sizeof(BYTE), fp);
	fwrite(&PC, 1, sizeof(WORD), fp);
}

void CPU::LoadDump(FILE* fp)
{
	fread(&tick, 1, sizeof(long long), fp);

	fread(&A, 1, sizeof(BYTE), fp);
	fread(&X, 1, sizeof(BYTE), fp);
	fread(&Y, 1, sizeof(BYTE), fp);
	fread(&SP, 1, sizeof(BYTE), fp);
	fread(&_PS, 1, sizeof(BYTE), fp);
	fread(&PC, 1, sizeof(WORD), fp);
}
