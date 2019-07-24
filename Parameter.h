#ifndef PARAMETER_H
#define PARAMETER_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Parameter
{
public:
	Parameter(string p)
	{
		param = p;
	}

	string toString()
	{
		return param;
	}
	void setConst(bool b)
	{
		isConst = b;
	}
	bool getConst()
	{
		return isConst;
	}
private:
	string param;
	bool isConst;
};




#endif