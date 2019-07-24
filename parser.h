#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "Datalog.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Rule.h"
#include <vector>
#include <sstream>
#include <iterator>
using namespace std;

class Parser
{
public:
	Parser(vector<Token> tokens) {
		this->tokens = tokens;
		pos = 0;
	}
	Datalog ParseAll();
	Datalog myDatalog;
private:
	string createID;
	vector<Parameter> params;
	vector<Predicate> Schemes;
	vector<Predicate> Facts;
	vector<Predicate> Query;
	vector<Rule> Rules;
	vector<Predicate> followPreds;
	vector<Token> tokens;
	Predicate headP;
	int counter;
	int pos;
	bool isRule = false;
	bool isExpression = false;
	string myExpression;

	void parseScheme();
	void idList();
	void schemeList();
	void factList();
	void parseFact();
	void stringList();
	void ruleList();
	void parseRule();
	void headPredicate();
	void parameter();
	void parameterList();
	void predicate();
	void predicateList();
	void expression();
	void parseOperator();
	void parseQuery();
	void queryList();
	void match(TokenType t);



};
#endif // !PARSER_H
