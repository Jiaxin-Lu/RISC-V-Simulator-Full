#pragma once
#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include "Simulation.h"
#include "Tool.h"
#include "Predictor.h"
#include <cmath>

class Executor
{
	friend class Simulator;

public:
	Executor() = default;
	~Executor() = default;
	uint32_t stall = 0;
	bool bubble = false;
	REG buf_new;
	REG buf;
	void Execute(REG ins, uint32_t &_buf_fetcher);
	void Executor_R(REG &inst);
	void Executor_I(REG &inst);
	void Executor_S(REG &inst);
	void Executor_B(REG &inst, uint32_t &_buf_fetcher);
	void Executor_U(REG &inst);
	void Executor_J(REG &inst);
	bool check_control_hazard();
};

#endif // !_EXECUTOR_H_
