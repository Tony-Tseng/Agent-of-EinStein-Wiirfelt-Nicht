#pragma once

#include "board.hpp"

class Strategy {
public:
	virtual ~Strategy()=default;
	virtual float Evaluate(Board*, int) = 0;
	virtual std::pair<float, float> GetBound() = 0;
};

class Manhattan : public Strategy {
private:
	float MINVALUE = -8;
	float MAXVALUE = 8;

public:
	~Manhattan() {}
	float Evaluate(Board*, int);
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
	float Evaluate(Board*, int);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Manhattan_Prob : public Strategy {
private:
	float MINVALUE = -14;
	float MAXVALUE = 14;
public:
	~Manhattan_Prob() {}
	float Evaluate(Board*, int);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Threaten : public Strategy {
private:
	float MINVALUE = -6;
	float MAXVALUE = 6;
public:
	~Threaten() {}
	float Evaluate(Board*, int);
	float GetEnemy(Board*, int);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};