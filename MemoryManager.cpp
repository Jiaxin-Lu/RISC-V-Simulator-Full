#include "MemoryManager.h"

void MemoryManager::MemoryAccess(REG &inst)
{
	if (inst.type == _I)
	{
		switch (inst.i_type)
		{
		case LW:
		{
			memcpy(&inst.sum, memory + inst.imm, 4);
		}
		break;
		case LB:
		{
			char tmp = 0;
			memcpy(&tmp, memory + inst.imm, 1);
			inst.sum = tmp;
		}
		break;
		case LBU:
		{
			unsigned char tmp = 0;
			memcpy(&tmp, memory + inst.imm, 1);
			inst.sum = tmp;
		}
		break;
		case LH:
		{
			short tmp = 0;
			memcpy(&tmp, memory + inst.imm, 2);
			inst.sum = tmp;
		}
		break;
		case LHU:
		{
			unsigned short tmp = 0;
			memcpy(&tmp, memory + inst.imm, 2);
			inst.sum = tmp;
		}
		break;
		}
	}
	else if (inst.type == _S)
	{
		switch (inst.s_type)
		{
		case SW:
		{
			memcpy(memory + inst.imm, &inst.rs2_sum, 4);
		}
		break;
		case SH:
		{
			short tmp = inst.rs2_sum;
			memcpy(memory + inst.imm, &tmp, 2);
		}
		break;
		case SB:
		{
			char tmp = inst.rs2_sum;
			memcpy(memory + inst.imm, &tmp, 1);
		}
		break;
		}
	}

	this->buf_new = inst;
	this->bubble = false;
	this->stall = false;
}