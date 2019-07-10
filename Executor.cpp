#include "Executor.h"

void Executor::Execute(REG inst, uint32_t &_buf_fetcher)
{
	this->stall = false;
	switch (inst.type)
	{
	case _R:
		Executor_R(inst);
		break;
	case _I:
		Executor_I(inst);
		break;
	case _S:
		Executor_S(inst);
		break;
	case _B:
		Executor_B(inst, _buf_fetcher);
		break;
	case _U:
		Executor_U(inst);
		break;
	case _J:
		Executor_J(inst);
		break;
	}
	reg[0] = 0;
	this->buf_new = inst;
	this->bubble = false;
	this->stall = 0;
}

void Executor::Executor_R(REG &inst)
{
	switch (inst.r_type)
	{
	case ADD:
		inst.sum = inst.rs1_sum + inst.rs2_sum;
		break;
	case SUB:
		inst.sum = inst.rs1_sum - inst.rs2_sum;
		break;
	case SLT:
		inst.sum = inst.rs1_sum < inst.rs2_sum;
		break;
	case SLTU:
		inst.sum = (uint32_t)(inst.rs1_sum) < (uint32_t)(inst.rs2_sum);
		break;
	case XOR:
		inst.sum = inst.rs1_sum ^ inst.rs2_sum;
		break;
	case AND:
		inst.sum = inst.rs1_sum & inst.rs2_sum;
		break;
	case OR:
		inst.sum = inst.rs1_sum | inst.rs2_sum;
		break;
	case SLL:
	{
		uint32_t r2 = (((uint32_t)(inst.rs2_sum) << 27) >> 27);
		inst.sum = inst.rs1_sum << r2;
	}
	break;
	case SRA:
	{
		uint32_t r2 = (((uint32_t)(inst.rs2_sum) << 27) >> 27);
		inst.sum = inst.rs1_sum >> r2;
	}
	break;
	case SRL:
	{
		uint32_t r1 = inst.rs1_sum, r2 = inst.rs2_sum;
		inst.sum = r1 >> ((r2 << 27) >> 27);
	}
	break;
	}
}

void Executor::Executor_I(REG &inst)
{
	switch (inst.i_type)
	{
	case JALR:
		inst.sum = inst._pc + 4;
		pc = ((inst.imm + inst.rs1_sum) & (-2));
		break;
	case ADDI:
		inst.sum = inst.rs1_sum + inst.imm;
		break;
	case XORI:
		inst.sum = inst.rs1_sum ^ inst.imm;
		break;
	case ANDI:
		inst.sum = inst.rs1_sum & inst.imm;
		break;
	case ORI:
		inst.sum = inst.rs1_sum | inst.imm;
		break;
	case SLLI:
	{
		uint32_t _imm = inst.imm;
		inst.sum = inst.rs1_sum << ((_imm << 27) >> 27);
	}
	break;
	case SLTI:
		inst.sum = inst.rs1_sum < inst.imm;
		break;
	case SLTIU:
		inst.sum = (uint32_t)(inst.rs1_sum) < (uint32_t)(inst.imm);
		break;
	case SRLI:
	{
		uint32_t _imm = inst.imm;
		uint32_t r1 = inst.rs1_sum;
		inst.sum = r1 >> ((_imm << 27) >> 27);
	}
	break;
	case SRAI:
	{
		uint32_t _imm = inst.imm;
		inst.sum = inst.rs1_sum >> ((_imm << 27) >> 27);
	}
	break;
	case LB:
	case LH:
	case LW:
	case LBU:
	case LHU:
		inst.imm += inst.rs1_sum;
		break;
	}
}
void Executor::Executor_S(REG &inst)
{
	inst.imm += inst.rs1_sum;
}
void Executor::Executor_B(REG &inst, uint32_t &_buf_fetcher)
{
	Predictor &predictor = pred[(inst._pc >> 2) % 256];
	bool jumpFlag = predictor.Check_jump();
	switch (inst.b_type)
	{
	case BEQ:
		if (inst.rs1_sum == inst.rs2_sum)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(inst.rs1_sum == inst.rs2_sum);
		break;
	case BNE:
		if (inst.rs1_sum != inst.rs2_sum)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(inst.rs1_sum != inst.rs2_sum);
		break;
	case BLT:
		if (inst.rs1_sum < inst.rs2_sum)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(inst.rs1_sum < inst.rs2_sum);
		break;
	case BGE:
		if (inst.rs1_sum >= inst.rs2_sum)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(inst.rs1_sum >= inst.rs2_sum);
		break;
	case BLTU:
	{
		uint32_t r1 = inst.rs1_sum, r2 = inst.rs2_sum;
		if (r1 < r2)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(r1 < r2);
	}
	break;
	case BGEU:
	{
		uint32_t r1 = inst.rs1_sum, r2 = inst.rs2_sum;
		if (r1 >= r2)
		{
			if (jumpFlag)
			{
				pc = inst._pc + inst.imm + 4;
			}
			else
			{
				_buf_fetcher = 0;
				pc = inst._pc + inst.imm;
			}
		}
		else
		{
			if (jumpFlag)
			{
				_buf_fetcher = 0;
			}
		}
		predictor.Update(r1 >= r2);
	}
	break;
	}
}
void Executor::Executor_U(REG &inst)
{

	if (inst.u_type == LUI)
	{
		inst.sum = inst.imm;
	}
	else //AUIPC
	{
		inst.sum = inst._pc + inst.imm;
	}
}
void Executor::Executor_J(REG &inst)
{

	inst.sum = inst._pc + 4;
	pc = inst._pc + inst.imm;
}

bool Executor::check_control_hazard()
{
	if (this->buf_new.type == _J || (this->buf_new.type == _I && this->buf_new.i_type == JALR))
		return true;
	else
		return false;
}