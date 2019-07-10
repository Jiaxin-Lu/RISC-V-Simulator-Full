#pragma once
#ifndef _DECODE_H_
#define _DECODE_H_

#include "Simulation.h"
#include "Tool.h"

class Decoder
{
	friend class Simulator;

public:
	REG buf_new;
	REG buf;
	uint32_t stall = 0;
	bool bubble = false;
	bool JumpPrediction = false;
	int JumpPC = 0;
	int JumpBranch = 0;

	void Decode(uint32_t _buf, int _pc);
	void Decode_R(uint32_t _buf, REG &inst);
	void Decode_I(uint32_t _buf, REG &inst);
	void Decode_S(uint32_t _buf, REG &inst);
	void Decode_B(uint32_t _buf, REG &inst);
	void Decode_U(uint32_t _buf, REG &inst);
	void Decode_J(uint32_t _buf, REG &inst);
	bool _check_data(int rs, const REG& ex_buf, const REG& mem_buf, const bool& ex_bubble, const bool& mem_bubble, int& sum);
	bool check_data_hazard(const REG& ex_buf, const REG& mem_buf, const bool& ex_bubble, const bool& mem_bubble);
};

#endif // !_DECODE_H_
