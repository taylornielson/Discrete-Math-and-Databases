#ifndef RELATION_H
#define RELATION_H
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "Tuple.h"
#include "Scheme.h"
using namespace std;


class Relation : public vector<string>
{
public:
	Relation() {};
	Relation(string n, Scheme s)
	{
		name = n;
		scheme = s;
	}

	void addTuple(Tuple t)
	{
		tuples.insert(t);
	}
	void select1(int i, string s)
	{
		set<Tuple>::iterator it;
		for (it = tuples.begin(); it != tuples.end();)
		{
			if (it->at(i) != s)
			{
				tuples.erase(it);
				it = tuples.begin();
			}
			else
			{
				it++;
			}
		}
	}

	void select2(int i, int j)
	{
		set<Tuple>::iterator it;
		it = tuples.begin();
		while (it != tuples.end())
		{
			if (it->at(i) != it->at(j))
			{
				tuples.erase(it);
				it = tuples.begin();
			}
			else
			{
				++it;
			}

		}
	}

	Relation project(map<string, int> m) //Simplify the complexity
	{
		map<string, int>::iterator it = m.begin();
		set<Tuple>::iterator iti;
		Relation myRelation(name, scheme);
		set<Tuple> mySet;
		Tuple myTuple;
		if (myRelation.scheme.size() < m.size())
		{
			return myRelation;
		}
		if (m.size() == 0)
		{
			for (iti = tuples.begin(); iti != tuples.end(); ++iti)
			{
				myRelation.addTuple(*iti);
			}
			return myRelation;
		}
		else
		{
			innerProject(myRelation, tuples, m);
			/*
			for (Tuple ob : tuples)
			{
				it = m.begin();
				unsigned int i = 0;
				int j = 0;
				for (i = 0; i < ob.size(); ++i)
				{
					while (it != m.end())
					{

						if (j == it->second)
						{
							if (ob.size() <= i)
							{
								return myRelation;
							}
							myTuple.push_back(ob[j]);
							it = m.begin();
							break;
						}
						++it;
					}
					it = m.begin();
					++j;
				}
				myRelation.addTuple(myTuple);
				myTuple.clear();
			}*/
			bool exists;
			it = m.begin();
			int i = 0;
			for (unsigned int j = 0; j < m.size(); ++j)
			{
				it = m.begin();
				while (it != m.end())
				{
					if (i != it->second)
					{
						exists = false;
					}
					else
					{
						exists = true;
						break;
					}
					++it;
				}
				if (!exists)
				{
					myRelation.scheme.erase(myRelation.scheme.begin() + i);
				}
				++i;

			}

			return myRelation;
		}

	}
	void innerProject(Relation &myRelation, set<Tuple> &tuples, map<string, int> &m)
	{
		Tuple myTuple;
		map<string, int>::iterator it;
		for (Tuple ob : tuples)
		{
			it = m.begin();
			unsigned int i = 0;
			int j = 0;
			for (i = 0; i < ob.size(); ++i)
			{
				while (it != m.end())
				{

					if (j == it->second)
					{
						if (ob.size() <= i)
						{
							break;
						}
						myTuple.push_back(ob[j]);
						it = m.begin();
						break;
					}
					++it;
				}
				it = m.begin();
				++j;
			}
			myRelation.addTuple(myTuple);
			myTuple.clear();
		}
	}

	void rename(map<string, int> m)
	{
		map<string, int>::iterator it;
		it = m.begin();
		scheme.clear();
		int max = findMax(m);
		int i = 0;
		for (i = 0; i <= max; ++i)
		{
			for (it = m.begin(); it != m.end(); ++it)
			{
				if (it->second == i)
				{
					scheme.push_back(it->first);
				}

			}

		}

	}
	int findMax(map<string, int> m)
	{
		map<string, int>::iterator it;
		int d = 0;
		for (it = m.begin(); it != m.end(); ++it)
		{
			if (it->second > d)
			{
				d = it->second;
			}
		}
		return d;
	}
	string toString()
	{
		stringstream ss;
		for (Tuple ob : tuples) {
			if (scheme.size() == 0) { break; }
			else
			{
                ss << "  ";
				for (unsigned int i = 0; i < scheme.size()-1; ++i)
				{
					ss << scheme[i] << "=" << ob[i] << ", ";
				}
				ss << scheme[scheme.size()-1] << "=" << ob[scheme.size()-1] << endl;
			}

		}
		string value = ss.str();

		return value;
	}
	set<Tuple> getTuples()
	{
		return tuples;
	}

	Relation joinRelations(Relation beta)
	{
		vector<unsigned int> matchedSchemeAlpha;
		vector<unsigned int> matchedSchemeBeta;
		bool joinable;
		if (beta.scheme.size() > this->scheme.size())
		{
			betaLarger(matchedSchemeAlpha, matchedSchemeBeta, beta);
		}
		else
		{
			alphaLarger(matchedSchemeAlpha, matchedSchemeBeta, beta);
		}
		Scheme newScheme = combineScheme(matchedSchemeBeta, beta);
		Relation newRelation(this->name, newScheme);
		set<Tuple>::iterator it;
		set<Tuple>::iterator iti;
		for (it = this->tuples.begin(); it != this->tuples.end(); ++it)
		{
			for (iti = beta.tuples.begin(); iti != beta.tuples.end(); ++iti)
			{
				joinable = isJoinable(matchedSchemeAlpha, matchedSchemeBeta, *it, *iti);
				if (joinable)
				{
					newRelation.addTuple(joinTuples(*it, *iti, matchedSchemeBeta));
				}
			}
		}
		return newRelation;
	}

	Scheme combineScheme(vector<unsigned int> &matchedSchemeBeta, Relation &beta)
	{
		bool usedAlready = false;
		Scheme newScheme;
		for (unsigned int i = 0; i < this->scheme.size(); ++i)
		{
			newScheme.push_back(this->scheme.at(i));
		}
		/*for (unsigned int i = 0; i < matchedSchemeBeta.size(); ++i)
		{
			beta.scheme.erase(beta.scheme.begin() + matchedSchemeBeta.at(i));
		}*/
		for (unsigned int i = 0; i < beta.scheme.size(); ++i)
		{
			usedAlready = false;
			for (unsigned int j = 0; j < matchedSchemeBeta.size(); ++j)
			{
				if (i == matchedSchemeBeta.at(j))
				{
					usedAlready = true;
					break;
				}
			}
			if (!usedAlready)
			{
				newScheme.push_back(beta.scheme.at(i));
			}
		}
		return newScheme;
	}

	void betaLarger(vector<unsigned int> &matchedSchemeAlpha, vector<unsigned int> &matchedSchemeBeta, Relation &beta)
	{
		for (unsigned int i = 0; i < beta.scheme.size(); ++i)
		{
			for (unsigned int j = 0; j < this->scheme.size(); ++j)
			{
				if (beta.scheme.at(i) == this->scheme.at(j))
				{
					matchedSchemeAlpha.push_back(j);
					matchedSchemeBeta.push_back(i);
					break;
				}
			}
		}
	}
	void alphaLarger(vector<unsigned int> &matchedSchemeAlpha, vector<unsigned int> &matchedSchemeBeta, Relation &beta)
	{
		for (unsigned int i = 0; i < this->scheme.size(); ++i)
		{
			for (unsigned int j = 0; j < beta.scheme.size(); ++j)
			{
				if (this->scheme.at(i) == beta.scheme.at(j))
				{
					matchedSchemeAlpha.push_back(i);
					matchedSchemeBeta.push_back(j);
					break;
				}
			}
		}
	}

	bool isJoinable(vector<unsigned int> &matchedSchemeAlpha, vector<unsigned int> &matchedSchemeBeta, Tuple tupleAlpha, Tuple tupleBeta)
	{
		for (unsigned int i = 0; i < matchedSchemeAlpha.size(); ++i)
		{
			if (matchedSchemeAlpha.at(i) >= tupleAlpha.size())
			{
				return false;
			}
			if (tupleAlpha.at(matchedSchemeAlpha.at(i)) != tupleBeta.at(matchedSchemeBeta.at(i)))
			{
				return false;
			}
		}
		return true;
	}

	Tuple joinTuples(Tuple tupleAlpha, Tuple tupleBeta, vector<unsigned int> &matchedBeta)
	{
		bool alreadyUsed = false;
		Tuple myTuple;
		for (unsigned int i = 0; i < tupleAlpha.size(); ++i)
		{
			myTuple.push_back(tupleAlpha.at(i));
		}
		for (unsigned int i = 0; i < tupleBeta.size(); ++i)
		{
			alreadyUsed = false;
			for (unsigned int j = 0; j < matchedBeta.size(); ++j)
			{
				if (i == matchedBeta.at(j))
				{
					alreadyUsed = true;
					break;
				}
			}
			if (!alreadyUsed)
			{
				myTuple.push_back(tupleBeta.at(i));
			}
		}
		/*
		for (unsigned int i = 0; i < tupleBeta.size(); ++i)
		{
			myTuple.push_back(tupleBeta.at(i));
		} */
		return myTuple;
	}

	void unionizing(Relation beta)
	{
		set<Tuple>::iterator it;
		for (it = beta.tuples.begin(); it != beta.tuples.end(); ++it)
		{
			if (!this->tuples.insert(*it).second)
			{
				this->tuples.insert(*it);
			}
		}
		
	}

	Relation projectRule(Predicate p)
	{
		vector<int> usedNums;
		Scheme newScheme;
		vector<Parameter> params = p.getParams();
		for (unsigned int i = 0; i < params.size(); ++i)
		{
			for (unsigned int j = 0; j < scheme.size(); ++j)
			{
				if (params.at(i).toString() == scheme.at(j))
				{
					usedNums.push_back(j);
				}
			}
		}
		for (unsigned int i = 0; i < usedNums.size(); ++i)
		{
			newScheme.push_back(scheme.at(usedNums[i]));
		}
		Relation newRelation(name, newScheme);
		Tuple myTuple;
		for (Tuple ob : tuples)
		{
			for (unsigned int i = 0; i < usedNums.size(); ++i)
			{
				myTuple.push_back(ob.at(usedNums[i]));
			}
			newRelation.addTuple(myTuple);
			myTuple.clear();
		}
		return newRelation;
	}

	int getTupSize()
	{
		return tuples.size();
	}

	void renameRule(Predicate p)
	{
		name = p.getID();
	}
	string getName()
	{
		return name;
	}

	private:
		string name;
		Scheme scheme;
		set<Tuple> tuples;

};

#endif
