#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include <cstddef>

class Predictor
{
private:
	struct Counter
	{
		int sum = 0;
		void update(bool flag)
		{
			if (flag)
			{
				if (sum < 3)
					++sum;
			}
			else
			{
				if (sum > 0)
					--sum;
			}
		}
		bool _check_jump()
		{
			if (sum <= 1)
				return false;
			else
				return true;
		}
	};
	Counter counter[4];
	int history[2] = {0};

public:
	Predictor() {}
	~Predictor() {}
	void Update(bool flag);
	bool Check_jump();
};

#endif // !_PREDICTOR_H_
