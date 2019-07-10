#include "Predictor.h"

void Predictor::Update(bool flag)
{
	size_t _H = ((history[0] << 1) + history[1]);
	counter[_H].update(flag);
	history[0] = history[1], history[1] = flag ? 1 : 0;
}

bool Predictor::Check_jump()
{
	size_t _H = ((history[0] << 1) + history[1]);
	return counter[_H]._check_jump();
}