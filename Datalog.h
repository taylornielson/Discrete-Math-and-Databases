#ifndef DATALOG_H
#define DATALOG_H
#include "token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <set>
#include <iterator>
#include <string>
using namespace std;

class Datalog
{
public:
	Datalog() {};
	Datalog(vector<Predicate> v1, vector<Predicate> v2, vector<Predicate> v3, vector<Rule> v4 )
	{
		Schemes = v1;
		Facts = v2;
		Query = v3;
		Rules = v4;
	}

	void addData(vector<Predicate> v1, vector<Predicate> v2, vector<Predicate> v3, vector<Rule> v4)
	{
		Schemes = v1;
		Facts = v2;
		Query = v3;
		Rules = v4;
	}

	void toString()
	{
		cout << "Schemes" << "(" << Schemes.size() << "):" << endl;
		for (unsigned int i = 0; i < Schemes.size(); ++i)
		{
			cout << "  " << Schemes[i].toString() << endl;
		}

		cout << "Facts" << "(" << Facts.size() << "):" << endl;
		for (unsigned int i = 0; i < Facts.size(); ++i)
		{
			cout << "  " << Facts[i].toString() << "." << endl;
		}

		cout << "Rules" << "(" << Rules.size() << "):" << endl;
		for (unsigned int i = 0; i < Rules.size(); ++i)
		{
			cout << "  " << Rules[i].toString() << endl;
		}

		cout << "Queries" << "(" << Query.size() << "):" << endl;
		for (unsigned int i = 0; i < Query.size(); ++i)
		{
			cout << "  " << Query[i].toString() << "?" << endl;
		}


			ostringstream stream;
			copy(domain.begin(), domain.end(), ostream_iterator<string>(stream, "  \n"));
			string result = stream.str();

			cout << "Domain (" << domain.size() << "):" << endl;
			if (domain.size() != 0)
			{
				result.pop_back();
			}
			cout << result;


	}

	void addDomain(string item)
	{
		domain.insert(item);
	}

	vector<Predicate> getSchemes()
	{
		return Schemes;
	}
	vector<Predicate> getFacts()
	{
		return Facts;
	}
	vector<Rule> getRules()
	{
		return Rules;
	}
	vector<Predicate> getQueries()
	{
		return Query;
	}
private:
	vector<Predicate> Schemes;
	vector<Predicate> Facts;
	vector<Predicate> Query;
	vector<Rule> Rules;
	set<string> domain;

};



#endif //
