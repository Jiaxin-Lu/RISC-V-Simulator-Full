#pragma once
#ifndef _FETCH_H_
#define _FETCH_H_
#include "Simulation.h"
#include "Tool.h"
#include <cstddef>

class Fetcher
{
	friend class simulator;

public:
	uint32_t stall = 0;
	bool bubble = false;
	bool f_bubble = false;
	uint32_t buf_new = 0;
	uint32_t buf = 0;
	int _pc = 0;
	Fetcher() {}
	~Fetcher() {}
	void Fetch(const int &pc_);
};

#endif // !_FETCH_H_
