#pragma once
#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "Tool.h"
#include "Simulation.h"
#include <cstring>

class MemoryManager
{
	friend class Simulator;

public:
	MemoryManager() {}
	~MemoryManager() {}
	uint32_t stall = 0;
	bool bubble = false;
	REG buf;
	REG buf_new;
	void MemoryAccess(REG &inst);
};

#endif // !_MEMORY_MANAGER_H_
