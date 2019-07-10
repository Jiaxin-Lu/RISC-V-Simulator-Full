#include <iostream>

#include "Tool.h"
#include "Executor.h"
#include "Decode.h"
#include "Simulation.h"
#include "WriteBack.h"
#include "Fetch.h"
#include "MemoryManager.h"
#include "Predictor.h"

int main()
{
	Simulator simulator;
	simulator.Pre();
	simulator.Simulate();
	return 0;
}