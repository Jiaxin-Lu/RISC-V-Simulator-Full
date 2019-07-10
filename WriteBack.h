#pragma once
#ifndef _WRITE_BACK_H_
#define _WRITE_BACK_H_

#include "Simulation.h"
#include "Tool.h"

class WriteBack
{
	friend class Simulator;

public:
	bool bubble = false;
	int stall = 0;
	void Write(const REG &inst);
};
#endif // !_WRITE_BACK_H_
