#include "Fetch.h"

void Fetcher::Fetch(const int &pc_)
{
	memcpy(&buf_new, memory + pc_, sizeof(4));
	this->_pc = pc_;
	this->bubble = false;
	this->stall = false;
}