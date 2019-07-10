#pragma once
#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_
#include <string>
#include <iostream>
#include <cstring>
#include "Tool.h"
#include "Decode.h"
#include "Executor.h"
#include "MemoryManager.h"
#include "WriteBack.h"
#include "Fetch.h"
#include "Predictor.h"
extern char memory[2097152];
extern int pc;
extern int reg[32];
extern int tot[32];
extern Predictor pred[256];
class Simulator
{
private:
public:
	Simulator() = default;
	~Simulator() = default;
	void Pre();
	void Simulate();
};

#endif // !_SIMULATOR_H_
