#include "Interpreter.h"
#include "Graph.h"
using namespace std;


Interpreter::Interpreter(Datalog d)
{
	data = d;
	createScheme(data.getSchemes());
	loadScheme(data.getFacts());
	parseRules(data.getRules());
	queries = data.getQueries();
	cout << "Schemes populated after " << numIts << " passes through the Rules." << endl;
	solveQueries(queries);
};

int Interpreter::getDBSize()
{
	int holder = 0;
	map<string, Relation>::iterator it;
	for (it = dBase.begin(); it != dBase.end(); ++it)
	{
		holder += it->second.getTupSize();
	}
	return holder;
}

void Interpreter::createScheme(vector<Predicate> p)
{
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		dBase.insert(pair<string, Relation>(p[i].getID(), makeRelation(p[i])));
	}
}

void Interpreter::loadScheme(vector<Predicate> p)
{
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		pairTuple(p[i]);
	}

}
void Interpreter::parseRules(vector<Rule> r)
{
	Graph myGraph(r);
	cout << "Rule Evaluation" << endl;
	vector<vector<int>> scc = myGraph.getSCC();
	for (unsigned int i = 0; i < scc.size(); ++i)
	{
		if (scc.at(i).size() == 1)
		{
			if (myGraph.hasDependencies(scc.at(i).at(0)))
			{
				fixedPoint(scc.at(i), r);
			}
			else
			{
				Predicate headPred = r.at(scc.at(i).at(0)).getHead();
				vector<Predicate> body = r.at(scc.at(i).at(0)).getBody();
				joinRules(body, headPred);
				cout << "1 passes: R" << scc.at(i).at(0) << endl;
			}
		}
		else
		{
			fixedPoint(scc.at(i), r);
		}
	}
	//while (preSize != postSize)
//{
	//preSize = getDBSize();
	/*
	for (unsigned int i = 0; i < r.size(); ++i)
	{
		Predicate headPred = r.at(i).getHead();
		vector<Predicate> body = r.at(i).getBody();
		joinRules(body, headPred);
	}
	*/
	//postSize = getDBSize();
		//++numIts;
	//}
}
void Interpreter::joinRules(vector<Predicate> p, Predicate headPred)
{
	Relation firstRelation;
	Relation secondRelation;
	Relation finalRelation;
	int preSize = dBase.find(headPred.getID())->second.getTupSize();
	if (p.size() > 0)
	{
		firstRelation = solveSingleQuery(p[0]);
		if (p.size() > 1)
		{
			for (unsigned int i = 1; i < p.size(); ++i)
			{
				secondRelation = solveSingleQuery(p[i]);
				finalRelation = firstRelation.joinRelations(secondRelation);
				firstRelation = finalRelation;
			}
		}
	}
	finalRelation = firstRelation.projectRule(headPred);
	finalRelation.renameRule(headPred);
	dBase.find(finalRelation.getName())->second.unionizing(finalRelation);
}
void Interpreter::pairTuple(Predicate p)
{
	Tuple temp;
	Database::iterator it;
	string id = p.getID();
	temp = makeTuple(p.getParams());
	it = dBase.find(id);
	it->second.addTuple(temp);
}
Tuple Interpreter::makeTuple(vector<Parameter> p)
{
	Tuple tempTuple;
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		tempTuple.push_back(p[i].toString());
	}
	return tempTuple;
}

Relation Interpreter::makeRelation(Predicate p)
{
	string id = p.getID();
	return Relation(id, makeScheme(p.getParams()));

}

Scheme Interpreter::makeScheme(vector<Parameter> s)
{
	Scheme tempScheme;
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		tempScheme.push_back(s[i].toString());
	}
	return tempScheme;
}

Relation Interpreter::solveSingleQuery(Predicate p)
{
	map<string, int> usedBefore;
	int j;
	bool isConst = false;
	vector<int> indexes;
	Relation curRelation = dBase[p.getID()];
	vector<Parameter> params = p.getParams();
	for (unsigned int i = 0; i < params.size(); ++i)
	{
		isConst = checkConst(params[i]);
		if (isConst)
		{
			curRelation.select1(i, params[i].toString());
		}
		else
		{
			if( usedBefore.find(params[i].toString()) == usedBefore.end()) //It wasnt in it yet
			{
				usedBefore.insert(pair<string, int>(params[i].toString(), i));
			}
			else //used before
			{
				j = usedBefore.find(params[i].toString())->second;
				curRelation.select2(i,j);
			}
		}
	}
	curRelation = curRelation.project(usedBefore);
	curRelation.rename(usedBefore);
	return curRelation;

}

bool Interpreter::checkConst(Parameter p)
{
	if (p.getConst())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Interpreter::fixedPoint(vector<int> v, vector<Rule> r)
{
	stringstream ss;
	string ret;
	while (preSize != postSize)
	{
			preSize = getDBSize();
			for (unsigned int i = 0; i < v.size(); ++i)
			{
				Predicate headPred = r.at(v.at(i)).getHead();
				vector<Predicate> body = r.at(v.at(i)).getBody();
				joinRules(body, headPred);
			}
			postSize = getDBSize();
		++numIts;
	}
	cout << numIts << " passes: ";
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		ss << "R" << v.at(i) << ",";
	}
	ret = ss.str();
	ret.pop_back();
	cout << ret << endl;
	numIts = 0;
	postSize = preSize + 1;
}

void Interpreter::solveQueries(vector<Predicate> p)
{
	Relation tempRelation;
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		tempRelation = solveSingleQuery(p[i]);
		cout << queries[i].toString() << "? ";
		if (tempRelation.getTuples().size() > 0)
		{
			cout << "Yes" << "(" << tempRelation.getTuples().size() << ")" << endl;
		}
		else
		{
			cout << "No" << endl;
		}
		cout << tempRelation.toString();
	}
}
