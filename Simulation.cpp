#include "Simulation.h"
#pragma warning(disable : 4996)
using std::cin;
using std::string;
char memory[2097152];
int reg[32];
int pc = 0;
int tot[32] = {0};
Predictor pred[256];
// #define _DEBUG_OUT
void Simulator::Pre()
{
	int pos = 0;
	string mess;
	uint32_t x = 0;
	while (true)
	{
		cin >> mess;
		if (cin.eof())
			return;
		if (mess[0] == '@')
		{
			x = sscanf(mess.c_str() + 1, "%x", &pos);
			continue;
		}
		int x1, x2, x3, x4, _mess;
		scanf("%x%x%x", &x2, &x3, &x4);
		x = sscanf(mess.c_str(), "%x", &x1);
		_mess = x1 + (x2 << 8) + (x3 << 16) + (x4 << 24);
		memcpy(memory + pos, &_mess, sizeof(_mess));
		//std::cout << "mess : " << std::hex << _mess << std::endl;
		pos += 4;
	}
}

void Simulator::Simulate()
{
	Fetcher fetcher;
	Decoder decoder;
	Executor executor;
	MemoryManager memorymanager;
	WriteBack writeback;
	fetcher.f_bubble = false;
	fetcher.bubble = true;
	decoder.bubble = true;
	executor.bubble = true;
	memorymanager.bubble = true;
	int memory_clock = 0;
	while (true)
	{
		//writeback
		if ((!memorymanager.bubble) && (memorymanager.buf_new.flag))
		{
			memorymanager.buf = memorymanager.buf_new;
			writeback.Write(memorymanager.buf);
		}

		//memorymanager
		executor.buf = executor.buf_new;
		if (memory_clock == 0)
		{
			if (executor.bubble)
			{
				memorymanager.bubble = true;
			}
			else if (writeback.stall)
			{
				memorymanager.stall = true;
			}
			else if (executor.buf.flag == false)
			{
				memorymanager.buf_new = executor.buf;
			}
			else
			{
				memorymanager.MemoryAccess(executor.buf);
				memory_clock = (memory_clock + 1) % 3;
				if (memory_clock == 0)
				{
					memorymanager.stall = false;
					memorymanager.bubble = false;
				}
				else
				{
					memorymanager.stall = true;
					memorymanager.bubble = true;
				}
			}
		}
		else
		{
			memorymanager.MemoryAccess(executor.buf);
			memory_clock = (memory_clock + 1) % 3;
			if (memory_clock == 0)
			{
				memorymanager.stall = false;
				memorymanager.bubble = false;
			}
			else
			{
				memorymanager.stall = true;
				memorymanager.bubble = true;
			}
		}
#ifdef _DEBUG_OUT
		std::cout << "memorymanager " << memorymanager.buf_new.OUT() << " clock:" << memory_clock << std::endl;
#endif // _DEBUG_OUT

		//executor
		decoder.buf = decoder.buf_new;
		executor.stall = false;
		if (decoder.bubble)
		{
			executor.bubble = true;
#ifdef _DEBUG_OUT
			std::cout << "executor BUBBLE" << std::endl;
#endif // _DEBUG_OUT
		}
		else if (memorymanager.stall)
		{
			executor.stall = true;
#ifdef _DEBUG_OUT
			std::cout << "executor STALL" << std::endl;
#endif // _DEBUG_OUT
		}
		else if (decoder.buf.flag == false)
		{
			executor.buf_new = decoder.buf;
#ifdef _DEBUG_OUT
			std::cout << "executor INVALID BUF" << std::endl;
#endif // _DEBUG_OUT
		}
		else
		{
			executor.Execute(decoder.buf, fetcher.buf_new);
			if (executor.check_control_hazard())
			{
				fetcher.buf_new = 0;
#ifdef _DEBUG_OUT
				puts("control hazard");
#endif // _DEBUG_OUT
			}
#ifdef _DEBUG_OUT
			std::cout << "executor " << executor.buf_new.OUT() << std::endl;
#endif // _DEBUG_OUT
		}

		//decoder
		decoder.stall = false;
		fetcher.buf = fetcher.buf_new;
		if (fetcher.bubble)
		{
			decoder.bubble = true;
#ifdef _DEBUG_OUT
			std::cout << "decoder BUBBLE" << std::endl;
#endif // _DEBUG_OUT
		}
		else if (executor.stall)
		{
			decoder.stall = true;
#ifdef _DEBUG_OUT
			std::cout << "decoder STALL" << std::endl;
#endif // _DEBUG_OUT
		}
		else
		{
			decoder.Decode(fetcher.buf, fetcher._pc);
			if (decoder.check_data_hazard(executor.buf_new, memorymanager.buf_new, executor.bubble, memorymanager.bubble))
			{
				decoder.bubble = true;
				decoder.stall = true;
#ifdef _DEBUG_OUT
				puts("data hazard");
#endif // _DEBUG_OUT
			}
			else
			{
				decoder.bubble = false;
				decoder.stall = false;
			}
#ifdef _DEBUG_OUT
			std::cout << "decoder " << decoder.buf_new.OUT() << std::endl;
#endif // _DEBUG_OUT
		}

		//fetcher
		if (!decoder.stall)
		{
			if (decoder.JumpPrediction)
			{
				Predictor predictor = pred[(decoder.JumpBranch >> 2) % 256];
				if (predictor.Check_jump())
				{
					fetcher.Fetch(decoder.JumpPC);
				}
				else
				{
					fetcher.Fetch(pc);
					pc += 4;
				}
				decoder.JumpPrediction = false;
			}
			else
			{
				fetcher.Fetch(pc);
				pc += 4;
			}
		}
#ifdef _DEBUG_OUT
		std::cout << "fetcher buf:" << std::hex << fetcher.buf_new << " pc:" << fetcher._pc << std::endl;
#endif // _DEBUG_OUT
		if (fetcher.buf_new == 0xc68223)
			break;
	}
	std::cout << std::dec << (((unsigned int)reg[10]) & 255u) << std::endl;
}