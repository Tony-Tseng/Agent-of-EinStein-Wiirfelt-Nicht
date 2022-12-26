#pragma once

#include "board.hpp"

class Strategy {
public:
	virtual ~Strategy()=default;
	virtual float Evaluate(Board*, int) = 0;
};

class DefaultEvaluate : public Strategy {
public:
	~DefaultEvaluate() {}
	float Evaluate(Board*, int);
};