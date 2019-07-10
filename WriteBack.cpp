#include "WriteBack.h"

void WriteBack::Write(const REG &inst)
{
	if (inst.type == _R || inst.type == _I || inst.type == _U || inst.type == _J)
	{
		reg[inst.rd] = inst.sum;
		-- tot[inst.rd];
	}
	reg[0] = 0;
}