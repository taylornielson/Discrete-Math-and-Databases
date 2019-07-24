#include "parser.h"

void Parser::match(TokenType t)
{
	if (tokens[pos].getType() == t)
	{
		++pos;
	}
	else throw(tokens[pos]);
}
void Parser::parseScheme()
{
	match(ID);
	createID = tokens[pos - 1].getContent();
	match(LEFT_PAREN);
	match(ID);
	params.push_back(tokens[pos - 1].getContent());
	idList();
	match(RIGHT_PAREN);
	Schemes.push_back(Predicate(createID, params));
	params.clear();

}


void Parser::idList()
{
	//COMMA ID idList | lambda
	if (tokens[pos].getType() == COMMA)
	{
		match(COMMA);
		match(ID);
		params.push_back(tokens[pos - 1].getContent());
		idList();
	}
	else return;
}

void Parser::schemeList()
{
	//scheme schemeList | lambdaid
	if (tokens[pos].getType() == ID)
	{
		parseScheme();
		schemeList();
	}
	else return;
}

void Parser::stringList()
{
	//COMMA STRING stringList | lambda
	if (tokens[pos].getType() == COMMA)
	{
		match(COMMA);
		match(STRING);
		myDatalog.addDomain(tokens[pos - 1].getContent());
		params.push_back(tokens[pos - 1].getContent());
		stringList();
	}
	else return;
}

void Parser::parseFact()
{
	//ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
	match(ID);
	createID = tokens[pos - 1].getContent();
	match(LEFT_PAREN);
	match(STRING);
	myDatalog.addDomain(tokens[pos - 1].getContent());
	params.push_back(tokens[pos - 1].getContent());
	stringList();
	match(RIGHT_PAREN);
	match(PERIOD);
	Facts.push_back(Predicate(createID, params));
	params.clear();
}

void Parser::factList()
{
	//factList	->	fact factList | lambda
	if (tokens[pos].getType() != RULES)
	{
		parseFact();
		factList();
	}
	else return;
}

void Parser::ruleList()
{
	//rule ruleList | lambda
	if (tokens[pos].getType() == ID)
	{
		parseRule();
		ruleList();
	}
	else return;
}

void Parser::parseRule()
{
	//headPredicate COLON_DASH  predicateList PERIOD
	headPredicate();
	match(COLON_DASH);
	predicate();
	predicateList();
	match(PERIOD);
	Rules.push_back(Rule(headP, followPreds));
	followPreds.clear();
}

void Parser::predicate()
{
	//ID LEFT_PAREN parameter parameterList RIGHT_PAREN
	match(ID);
	createID = tokens[pos - 1].getContent();
	match(LEFT_PAREN);
	parameter();
	parameterList();
	match(RIGHT_PAREN);
	if (isRule)
	{
		followPreds.push_back(Predicate(createID, params));
		params.clear();
	}
	else
	{
		Query.push_back(Predicate(createID, params));
		params.clear();
	}
	//rules have predicates and parameters
}

void Parser::parameter()
{
	//STRING | ID | expression
	if (tokens[pos].getType() == STRING)
	{
		if (isExpression)
		{
			match(STRING);
			myExpression += tokens[pos - 1].getContent();
		}
		else
		{
			match(STRING);
			params.push_back(tokens[pos - 1].getContent());
			params.back().setConst(true);
		}

	}
	else if (tokens[pos].getType() == ID)
	{
		if (isExpression)
		{
			match(ID);
			myExpression += tokens[pos - 1].getContent();
		}
		else
		{
			match(ID);
			params.push_back(tokens[pos - 1].getContent());
			params.back().setConst(false);
		}
	}
	else
	{
		expression();
		//params.push_back(myExpression);
		
	}
}

void Parser::expression()
{
	isExpression = true;
	//LEFT_PAREN parameter operator parameter RIGHT_PAREN
	match(LEFT_PAREN);
	myExpression += tokens[pos - 1].getContent();
	parameter();
	parseOperator();
	parameter();
	match(RIGHT_PAREN);
	myExpression += ")";
}

void Parser::parameterList()
{
	//COMMA parameter parameterList | lambda
	if (tokens[pos].getType() == COMMA)
	{
		match(COMMA);
		parameter();
		if (isExpression)
		{
			params.push_back(myExpression);
			myExpression = "";
			isExpression = false;
		}
		parameterList();
	}
	else return;
}

void Parser::predicateList()
{
	//COMMA predicate predicateList | lambda
	if (tokens[pos].getType() == COMMA)
	{
		match(COMMA);
		predicate();
		predicateList();
	}
	else return;
}

void Parser::headPredicate()
{
	//ID LEFT_PAREN ID idList RIGHT_PAREN
	match(ID);
	createID = tokens[pos - 1].getContent();
	match(LEFT_PAREN);
	match(ID);
	params.push_back(tokens[pos - 1].getContent());
	idList();
	match(RIGHT_PAREN);
	headP.setData(createID, params);
	params.clear();
}

void Parser::parseOperator()
{
	if (tokens[pos].getType() == ADD)
	{
		match(ADD);
		myExpression += tokens[pos - 1].getContent();
	}
	else
	{
		match(MULTIPLY);
		myExpression += tokens[pos - 1].getContent();
	}
}


void Parser::parseQuery()
{
	//predicate Q_MARK
	predicate();
	match(Q_MARK);
}

void Parser::queryList()
{
	//query queryList | lambda
	if (tokens[pos].getType() == ID)
	{
		parseQuery();
		queryList();
	}
	else return;
}

Datalog Parser::ParseAll()
{
	try
	{
				match(SCHEMES);
				match(COLON);
				parseScheme();
				schemeList();
				match(FACTS);
				match(COLON);
				factList();
				isRule = true;
				match(RULES);
				match(COLON);
				ruleList();
				isRule = false;
				match(QUERIES);
				match(COLON);
				parseQuery();
				queryList();
				match(ENDFILE);
		//cout << "Success!" << endl;
		myDatalog.addData(Schemes, Facts, Query, Rules);
		return myDatalog;
	}

	catch (Token t)
	{
		cout << "Failure!" << endl << "  " << t.printToken();
		myDatalog.addData(Schemes, Facts, Query, Rules);
		return myDatalog;
		
	}
}
