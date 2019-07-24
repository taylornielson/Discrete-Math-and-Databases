#ifndef GRAPH_H
#define GRAPH_H
#include <map>
#include "Node.h"
#include "Rule.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>
using namespace std;

class Graph
{
public:
	Graph(vector<Rule> r) 
	{
		rules = r;
		Node myNode;
		for (unsigned int i = 0; i < rules.size(); ++i)
		{
			myNode.setName(rules[i].getHead().getID());
			nodeMap.insert(pair<int, Node>(i, myNode));
		}
		setDependencies(rules, nodeMap);
		reverseGraph();
		toString();
		toStringReverse();
		dfsForest();
		orderedDFSForest();
		scc = sortVectors(); 
		cout << "Strongly Connected Components :" << endl;
		vector<vector<int>>::iterator it;
		for (it = scc.begin(); it != scc.end(); ++it)
		{
			cout << "{";
			for (unsigned int i = 0; i < it->size(); ++i)
			{
				cout << "R" << it->at(i) << ",";
			}
			cout << "}" << endl;
		}
		cout << "Pass" << endl;
	};
	void setDependencies(vector<Rule> &r, map<int, Node> &m)
	{
		string matchString;
		int matchInt;
		bool matchBool;
		map<int, Node>::iterator it;
		for (unsigned int i = 0; i < r.size(); ++i)
		{
			for (unsigned int j = 0; j < r[i].getBody().size(); ++j)
			{
				matchString = r[i].getBody().at(j).getID();
				matchRules(matchString, matchInt, matchBool, nodeMap, i);
				
			}
		}
	}
	void matchRules(string s, int &myInt, bool &matchBool, map<int, Node> &m, unsigned int &i)
	{
		matchBool = false;
		map<int, Node>::iterator it;
		for (it = nodeMap.begin(); it != nodeMap.end(); ++it)
		{
			if (it->second.getName() == s)
			{
				myInt = it->first;
				matchBool = true;
			}
			else
			{
				matchBool = false;
			}
			if (matchBool)
			{
				m.find(i)->second.addDependency(myInt);
			}
		}
	}
	void reverseGraph()
	{
		reverseNodeMap = nodeMap;
		clearDependencies(reverseNodeMap);
		set<int> dependents;
		set<int>::iterator iti;
		map<int, Node>::iterator it;
		for (it = nodeMap.begin(); it != nodeMap.end(); ++it)
		{
			dependents = it->second.getDependencies();
			for (iti = dependents.begin(); iti != dependents.end(); ++iti)
			{
				reverseNodeMap.find(*iti)->second.addDependency(it->first);
			}
		}
	}
	void toString()
	{
		map<int, Node>::iterator it;
		set<int>::iterator iti;
		cout << "Dependency Graph" << endl;
		for (it = nodeMap.begin(); it != nodeMap.end(); ++it)
		{
			cout << "R" << it->first << ":" << it->second.toString();
			cout << endl;
		}
		
	}
	void toStringReverse()
	{
		map<int, Node>::iterator it;
		set<int>::iterator iti;
		cout << "Reverse Dependency Graph" << endl;
		for (it = reverseNodeMap.begin(); it != reverseNodeMap.end(); ++it)
		{
			cout << "R" << it->first << ":" << it->second.toString();
			cout << endl;
		}

	}
	void clearDependencies(map<int, Node> &m)
	{
		map<int, Node>::iterator it;
		for (it = m.begin(); it != m.end(); ++it)
		{
			it->second.clearDep();
		}
	}
	void dfsForest()
	{
		map<int, Node>::iterator it;
		for (it = reverseNodeMap.begin(); it != reverseNodeMap.end(); ++it)
		{
			if (!it->second.getVisit())
			{
				preOrdering.push(it->first);
				dfs(it->second);
				while (preOrdering.size() > 0)
				{
					postOrdering.push(preOrdering.top());
					preOrdering.pop();
				}
			}
		}
	}
	void orderedDFSForest()
	{
		int j;
		setAllFalse();
		int size = postOrdering.size();
		vector<int> singleSCC;
		preOrdering1 = postOrdering;
		int size2 = postOrdering.size();
		for (int i = 0; i < size; ++i)
		{
			postOrdering.pop();
		}
		for (unsigned int i = size2; i > 0; --i)
		{
			j = preOrdering1.top();
			preOrdering1.pop();
			if (!nodeMap.find(j)->second.getVisit())
			{
				preOrdering.push(j);
				dfsForward(nodeMap.find(j)->second);
				while (preOrdering.size() > 0)
				{
					postOrdering.push(preOrdering.top());
					preOrdering.pop();
				}
				int size3 = postOrdering.size();
				for (int k = 0; k < size3; ++k)
				{
					singleSCC.push_back(postOrdering.top());
					postOrdering.pop();
				}
				scc.push_back(singleSCC);
				singleSCC.clear();
			}
		}
	}

	void dfs(Node &n) 
	{
		
		set<int> depends = n.getDependencies();
		if (depends.size() == 0 || allVisited(n))
		{
			n.setVisit(true);
			postOrdering.push(preOrdering.top());
			preOrdering.pop();
		}
		else
		{
			n.setVisit(true);
			for (set<int>::iterator it = depends.begin(); it != depends.end(); ++it)
			{
				if (!reverseNodeMap.find(*it)->second.getVisit())
				{
					reverseNodeMap.find(*it)->second.setVisit(true);
					if (preOrdering.top() != *it)
					{
						preOrdering.push(*it);
					}
					dfs(reverseNodeMap.find(*it)->second);
			
				}
			}
		}
	}

	void dfsForward(Node &n) 
	{

		set<int> depends = n.getDependencies();
		if (depends.size() == 0 || allVisitedStraight(n))
		{
			n.setVisit(true);
			postOrdering.push(preOrdering.top());
			preOrdering.pop();
		}
		else
		{
			n.setVisit(true);
			for (set<int>::iterator it = depends.begin(); it != depends.end(); ++it)
			{
				if (!nodeMap.find(*it)->second.getVisit())
				{
					nodeMap.find(*it)->second.setVisit(true);
					if (preOrdering.empty() || preOrdering.top() != *it)
					{
						preOrdering.push(*it);
					}
					dfsForward(nodeMap.find(*it)->second);

				}
			}
		}
	}

	bool allVisitedStraight(Node n)
	{
		set<int> dependents = n.getDependencies();
		set<int>::iterator it;
		for (it = dependents.begin(); it != dependents.end(); ++it)
		{
			if (!nodeMap.find(*it)->second.getVisit())
			{
				return false;
			}
		}
		return true;
	}

	bool allVisited(Node n)
	{
		set<int> dependents = n.getDependencies();
		set<int>::iterator it;
		for (it = dependents.begin(); it != dependents.end(); ++it)
		{
			if (!reverseNodeMap.find(*it)->second.getVisit())
			{
				return false;
			}
		}
		return true;
	}
	void setAllFalse()
	{
		for (unsigned int i = 0; i < postOrdering.size(); ++i)
		{
			nodeMap.find(i)->second.setVisit(false);
		}
	}
	vector<vector<int>> sortVectors()
	{
		vector<int> myVector;
		vector<vector<int>> mySet;
		vector<vector<int>>::iterator it;
		for (it = scc.begin(); it != scc.end(); ++it)
		{
			myVector = sortVector(*it);
			mySet.push_back(myVector);
		}
		return mySet;
	}
	vector<int> sortVector(vector<int> v) 
	{
		sort(v.begin(), v.end());
		return v;
	}
	vector<vector<int>> getSCC()
	{
		return scc;
	}
	bool hasDependencies(int i)
	{
		set<int> dependencies = nodeMap.find(i)->second.getDependencies();
		for (set<int>::iterator it = dependencies.begin(); it != dependencies.end(); ++it)
		{
			if (*it == i)
			{
				return true;
			}
		}
		return false;
		
	}
private:
	map<int, Node> nodeMap;
	map<int, Node> reverseNodeMap;
	vector<Rule> rules;
	stack<int> preOrdering;
	stack<int> preOrdering1;
	stack<int> postOrdering;
	vector<vector<int>> scc;
};

#endif