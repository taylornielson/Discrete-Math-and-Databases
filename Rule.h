#ifndef RULE_H
#define RULE_H
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;


class Rule
{
public:
	Rule() {};
	Rule(Predicate headPredicate, vector<Predicate> predicates)
	{
		this->headPredicate = headPredicate;
		//headParameters = headParams;
		bodyPredicates = predicates;
		//bodyParameters = params;
	}

	Predicate getHead()
	{
		return headPredicate;
	}
	vector<Predicate> getBody()
	{
		return bodyPredicates;
	}
	string toString()
	{
		stringstream ss;
		ss << headPredicate.toString() << " :- ";
		for (unsigned int i = 0; i < bodyPredicates.size() - 1; ++i)
		{
			ss << bodyPredicates[i].toString() << ",";
		}
		ss << bodyPredicates[bodyPredicates.size() - 1].toString() << ".";
		return ss.str();
	}

private:
	Predicate headPredicate;
	vector<Predicate> bodyPredicates;




};


#endif
