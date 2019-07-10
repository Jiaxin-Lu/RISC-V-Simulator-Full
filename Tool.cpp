#include "Tool.h"
std::string type_name[6]{"R_type", "I_type", "S_type", "B_type", "U_type", "J_type"};
std::string R_type_name[] = {"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND"};
std::string I_type_name[] = {"SLLI", "SRLI", "SRAI", "JALR", "LB", "LH", "LW", "LBU", "LHU",
							 "ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI"};
std::string S_type_name[] = {"SB", "SH", "SW"};
std::string B_type_name[] = {"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU"};
std::string U_type_name[] = {"LUI", "AUIPC"};
std::string REG_type_name[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0",
							   "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5",
							   "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
Types getType(uint32_t _buf)
{
	uint32_t opcode = (_buf << 25) >> 25;
	Types _type = ERROR;
	switch (opcode)
	{
	case 0x33:
		_type = _R;
		break;
	case 0x67:
	case 0x03:
	case 0x13:
		_type = _I;
		break;
	case 0x23:
		_type = _S;
		break;
	case 0x63:
		_type = _B;
		break;
	case 0x37:
	case 0x17:
		_type = _U;
		break;
	case 0x6f:
		_type = _J;
		break;
	}
	return _type;
}
