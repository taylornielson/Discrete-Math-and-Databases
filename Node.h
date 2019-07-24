#ifndef NODE_H
#define NODE_H
#include <set>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

class Node
{
public:
	Node() {};
	
	void setName(string n)
	{
		name = n;
	}
	
	void addDependency(int i)
	{
		dependencies.insert(i);
	}
	string getName()
	{
		return name;
	}
	string toString()
	{
		string ret;
		stringstream ss;
		set<int>::iterator it;
		for (it = dependencies.begin(); it != dependencies.end(); ++it)
		{
			ss << "R" << *it << ",";
		}
		ret = ss.str();
		if (ret.size() > 0)
		{
			ret.pop_back();
		}
		
		return ret;
	}
	set<int> getDependencies()
	{
		return dependencies;
	}
	void clearDep()
	{
		dependencies.clear();
	}
	bool getVisit()
	{
		return visited;
	}
	void setVisit(bool b)
	{
		visited = b;
	}
private:
	string name;
	set<int> dependencies;
	bool visited = false;
};

#endif