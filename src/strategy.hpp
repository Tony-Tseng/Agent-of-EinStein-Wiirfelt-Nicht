#pragma once

#include "board.hpp"
#include <iostream>

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

class Threat : public Strategy {
private:
	float MINVALUE = -3;
	float MAXVALUE = 3;
public:
	~Threat() {}
	float Evaluate_nearest(Board*, int, int*);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Piece : public Strategy {
private:
	float MINVALUE = -2;
	float MAXVALUE = 2;
public:
	~Piece() {}
	float Evaluate_nearest(Board*, int, int*);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Corner : public Strategy {
private:
	float MINVALUE = -3;
	float MAXVALUE = 3;
public:
	~Corner() {}
	float Evaluate_nearest(Board*, int, int [2]);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Protection : public Strategy {
private:
	float MINVALUE = -3;
	float MAXVALUE = 3;
public:
	~Protection() {}
	float Evaluate_nearest(Board*, int, int [2]);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};

class Turn : public Strategy {
private:
	float MINVALUE = -1;
	float MAXVALUE = 1;
public:
	~Turn() {}
	float Evaluate_nearest(Board*, int, int [2]);
	std::pair<float, float> GetBound(){
		return std::make_pair(MINVALUE, MAXVALUE);
	}
};
