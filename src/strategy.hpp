#pragma once

#include "board.hpp"

class Strategy {
public:
	virtual ~Strategy()=default;
	virtual float Evaluate_nearest(Board*, int, int*) = 0;
	virtual std::pair<float, float> GetBound() = 0;
};

class Manhattan : public Strategy {
private:
	float MINVALUE = -8;
	float MAXVALUE = 8;

public:
	~Manhattan() {}
	float Evaluate_nearest(Board*, int, int*);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class CubeStep : public Strategy {
private:
	float MINVALUE = -4;
	float MAXVALUE = 4;
	
public:
	~CubeStep() {}
	float Evaluate_nearest(Board*, int, int*);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Prob : public Strategy {
private:
	float MINVALUE = -6;
	float MAXVALUE = 6;
public:
	~Prob() {}
	float Evaluate_nearest(Board*, int, int*);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};