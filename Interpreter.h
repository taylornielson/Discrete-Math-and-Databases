#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <iostream>
#include <string>
#include <vector>
#include "Datalog.h"
#include "Database.h"
#include "Graph.h"
#include "Node.h"
using namespace std;

class Interpreter
{
public:
	Interpreter(Datalog d);
	void createScheme(vector<Predicate> p);
	void loadScheme(vector<Predicate> p);
	void parseRules(vector<Rule> r);
	void pairTuple(Predicate p);
	void joinRules(vector<Predicate> p, Predicate pr);
	Tuple makeTuple(vector<Parameter> p);
	Relation makeRelation(Predicate p);
	Scheme makeScheme(vector<Parameter> s);
	Relation solveSingleQuery(Predicate P);
	bool checkConst(Parameter p);
	void solveQueries(vector<Predicate> p);
	int getDBSize();
	void fixedPoint(vector<int>, vector<Rule>);
private:
	int preSize = 0;
	int postSize = 1;
	int numIts = 0;
	Datalog data;
	Database dBase;
	vector<Predicate> queries;
};


#endif
