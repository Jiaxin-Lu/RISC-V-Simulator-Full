#include "Decode.h"

void Decoder::Decode(uint32_t _buf, int _pc)
{
	this->stall = false;
	if (_buf == 0)
	{
		buf_new.flag = false;
		return;
	}
	REG inst;
	inst.type = getType(_buf);
	inst._pc = _pc;
	inst.flag = true;
	switch (inst.type)
	{
	case _R:
		Decode_R(_buf, inst);
		break;
	case _I:
		Decode_I(_buf, inst);
		break;
	case _S:
		Decode_S(_buf, inst);
		break;
	case _B:
		Decode_B(_buf, inst);
		break;
	case _U:
		Decode_U(_buf, inst);
		break;
	case _J:
		Decode_J(_buf, inst);
		break;
	}
	this->buf_new = inst;
	this->bubble = false;
}

void Decoder::Decode_R(uint32_t _buf, REG &inst)
{
	uint32_t funct7 = _buf >> 25;
	uint32_t funct3 = (_buf << 17) >> 29;
	inst.rs2 = (_buf << 7) >> 27;
	inst.rs1 = (_buf << 12) >> 27;
	inst.rd = (_buf << 20) >> 27;

	switch (funct3)
	{
	case 0:
		if (funct7 == 0)
			inst.r_type = ADD;
		else
			inst.r_type = SUB;
		break;
	case 1:
		inst.r_type = SLL;
		break;
	case 2:
		inst.r_type = SLT;
		break;
	case 3:
		inst.r_type = SLTU;
		break;
	case 4:
		inst.r_type = XOR;
		break;
	case 5:
		if (funct7 == 0)
			inst.r_type = SRL;
		else
			inst.r_type = SRA;
		break;
	case 6:
		inst.r_type = OR;
		break;
	case 7:
		inst.r_type = AND;
		break;
	}

	//std::cout << type_name[inst.type] << " " << R_type_name[inst.r_type] << std::endl;
}

void Decoder::Decode_I(uint32_t _buf, REG &inst)
{
	uint32_t funct3 = (_buf << 17) >> 29;
	inst.imm = (int)(_buf) >> 20;
	inst.rs1 = (_buf << 12) >> 27;
	inst.rd = (_buf << 20) >> 27;
	uint32_t opcode = (_buf << 25) >> 25;
	if (opcode == 0x67)
	{
		inst.i_type = JALR;
	}
	else if (opcode == 0x03)
	{
		switch (funct3)
		{
		case 0:
			inst.i_type = LB;
			break;
		case 1:
			inst.i_type = LH;
			break;
		case 2:
			inst.i_type = LW;
			break;
		case 4:
			inst.i_type = LBU;
			break;
		case 5:
			inst.i_type = LHU;
			break;
		}
	}
	else
	{
		switch (funct3)
		{
		case 0:
			inst.i_type = ADDI;
			break;
		case 1:
			inst.i_type = SLLI;
			break;
		case 2:
			inst.i_type = SLTI;
			break;
		case 3:
			inst.i_type = SLTIU;
			break;
		case 4:
			inst.i_type = XORI;
			break;
		case 5:
			if ((_buf >> 25) == 0)
			{
				inst.i_type = SRLI;
			}
			else
			{
				inst.i_type = SRAI;
			}
			break;
		case 6:
			inst.i_type = ORI;
			break;
		case 7:
			inst.i_type = ANDI;
			break;
		}
	}
	//std::cout << type_name[inst.type] << " " << I_type_name[inst.i_type] << std::endl;
}

void Decoder::Decode_S(uint32_t _buf, REG &inst)
{
	inst.imm = ((int)(_buf) >> 25) << 5;
	inst.imm += ((_buf << 20) >> 27);
	inst.rs2 = (_buf << 7) >> 27;
	inst.rs1 = (_buf << 12) >> 27;
	uint32_t funct3 = (_buf << 17) >> 29;
	switch (funct3)
	{
	case 0:
		inst.s_type = SB;
		break;
	case 1:
		inst.s_type = SH;
		break;
	case 2:
		inst.s_type = SW;
		break;
	}
	//std::cout << type_name[inst.type] << " " << S_type_name[inst.s_type] << std::endl;
}

void Decoder::Decode_B(uint32_t _buf, REG &inst)
{
	//int buff = _buf;
	inst.imm = (-(int)(_buf >> 31)) << 12;
	inst.imm += ((_buf << 1) >> 26) << 5;
	inst.imm += ((_buf << 20) >> 28) << 1;
	inst.imm += ((_buf << 24) >> 31) << 11;

	inst.rs2 = (_buf << 7) >> 27;
	inst.rs1 = (_buf << 12) >> 27;
	uint32_t funct3 = (_buf << 17) >> 29;
	switch (funct3)
	{
	case 0:
		inst.b_type = BEQ;
		break;
	case 1:
		inst.b_type = BNE;
		break;
	case 4:
		inst.b_type = BLT;
		break;
	case 5:
		inst.b_type = BGE;
		break;
	case 6:
		inst.b_type = BLTU;
		break;
	case 7:
		inst.b_type = BGEU;
		break;
	}
	//std::cout << type_name[inst.type] << " " << B_type_name[inst.b_type] << std::endl;
}

void Decoder::Decode_U(uint32_t _buf, REG &inst)
{
	inst.imm = ((int)(_buf) >> 12) << 12;
	inst.rd = (_buf << 20) >> 27;
	uint32_t opcode = (_buf << 25) >> 25;
	if (opcode == 0x37)
		inst.u_type = LUI;
	else
		inst.u_type = AUIPC;
	//std::cout << type_name[inst.type] << " " << U_type_name[inst.u_type] << std::endl;
}

void Decoder::Decode_J(uint32_t _buf, REG &inst)
{
	//int buff = _buf;
	inst.imm = (-(int)(_buf >> 31)) << 20;
	inst.imm += ((_buf << 1) >> 22) << 1;
	inst.imm += ((_buf << 11) >> 31) << 11;
	inst.imm += ((_buf << 12) >> 24) << 12;
	inst.rd = (_buf << 20) >> 27;
	inst.j_type = JAL;
	//if ((_buf >> 31) == 1) puts("true");
	//std::cout << std::dec << type_name[inst.type] << " " << _buf << " " << _buf << std::endl;
}

bool Decoder::_check_data(int rs, const REG &ex_buf, const REG &mem_buf, const bool &ex_bubble, const bool &mem_bubble, int &sum)
{
	//printf("tot : %d \n", tot[rs]);
	if (tot[rs] <= 0)
	{
		sum = reg[rs];
		return true;
	}
	else if (ex_buf.rd == rs && ex_buf.flag && (!ex_bubble))
	{
		if (ex_buf.type != _I)
		{
			sum = ex_buf.sum;
			return true;
		}
		else if (ex_buf.i_type < LB || ex_buf.i_type > LHU)
		{
			sum = ex_buf.sum;
			return true;
		}
		else
			return false;
	}
	if (mem_buf.rd == rs && mem_buf.flag && (!mem_bubble))
	{
		sum = mem_buf.rd == 0 ? 0 : mem_buf.sum;
		return true;
	}
	return false;
}

bool Decoder::check_data_hazard(const REG &ex_buf, const REG &mem_buf, const bool &ex_bubble, const bool &mem_bubble)
{
	if (buf_new.flag == false)
		return false;
	switch (buf_new.type)
	{
	case _R:
	{
		int r1 = 0, r2 = 0;
		bool f1 = _check_data(buf_new.rs1, ex_buf, mem_buf, ex_bubble, mem_bubble, r1);
		bool f2 = _check_data(buf_new.rs2, ex_buf, mem_buf, ex_bubble, mem_bubble, r2);
		if (f1 && f2)
		{
			buf_new.rs1_sum = r1, buf_new.rs2_sum = r2;
			++tot[buf_new.rd];
		}
		else
			return true;
	}
	break;
	case _I:
	{
		int r1 = 0;
		bool f1 = _check_data(buf_new.rs1, ex_buf, mem_buf, ex_bubble, mem_bubble, r1);
		if (f1)
		{
			buf_new.rs1_sum = r1, ++tot[buf_new.rd];
		}
		else
			return true;
	}
	break;
	case _S:
	{
		int r1 = 0, r2 = 0;
		bool f1 = _check_data(buf_new.rs1, ex_buf, mem_buf, ex_bubble, mem_bubble, r1);
		bool f2 = _check_data(buf_new.rs2, ex_buf, mem_buf, ex_bubble, mem_bubble, r2);
		if (f1 && f2)
		{
			buf_new.rs1_sum = r1, buf_new.rs2_sum = r2;
		}
		else
			return true;
	}
	break;
	case _B:
	{
		int r1 = 0, r2 = 0;
		bool f1 = _check_data(buf_new.rs1, ex_buf, mem_buf, ex_bubble, mem_bubble, r1);
		bool f2 = _check_data(buf_new.rs2, ex_buf, mem_buf, ex_bubble, mem_bubble, r2);
		if (f1 && f2)
		{
			buf_new.rs1_sum = r1, buf_new.rs2_sum = r2;
			this->JumpPrediction = true;
			this->JumpBranch = buf_new._pc;
			this->JumpPC = buf_new._pc + buf_new.imm;
		}
		else
			return true;
	}
	break;
	case _U:
		++tot[buf_new.rd];
		break;
	case _J:
		++tot[buf_new.rd];
		break;
	}
	return false;
}