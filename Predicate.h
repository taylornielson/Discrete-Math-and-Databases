#ifndef PREDICATE_H
#define PREDICATE_H
#include "Parameter.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Predicate
{
public:
	Predicate() {};
	Predicate(string i, vector<Parameter> p)
	{
		id = i;
		params = p;
	}
	string toString()
	{
		if (params.size() != 0)
		{
			stringstream ss;
			ss << id << "(";
			for (unsigned int i = 0; i < (params.size() - 1); ++i)
			{
				ss << params[i].toString() << ",";
			}
			ss << params[params.size() - 1].toString() << ")";
			return ss.str();
		}
		else
		{
			return "Empty\n";
		}
		
		
	}

	void setData(string i, vector<Parameter> p)
	{
		id = i;
		params = p;
	}

	string getID()
	{
		return id;
	}

	vector<Parameter> getParams()
	{
		return params;
	}

private:
	string id;
	vector<Parameter> params;
};


#endif
