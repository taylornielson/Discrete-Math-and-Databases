#include "scanner.h"

vector<Token> Scanner::getAllTokens()
{
	while (!in.eof())
	{
		getToken();
	}
	if (tokens.empty() || tokens.back().getTypeString() != "EOF")
	{
		tokens.push_back(Token("", currentLine, ENDFILE));
	}
		
	return tokens;
}

Token Scanner::getToken()
{
	if (in.eof())
	{
		return tokens.back();
	}
	crnt = in.get();
	in.peek(); //Necessary?
	whiteSpace();
	beginingLine = currentLine;
	if (crnt < 0)
	{
		tokens.push_back(Token("", beginingLine, ENDFILE));
		return tokens.back();
	}
	switch (crnt)
	{
		case ',':
			tokens.push_back(Token(",", currentLine, COMMA));
			return tokens.back();
		case '.':
			tokens.push_back(Token(".", currentLine, PERIOD));
			return tokens.back();
		case '?':
			tokens.push_back(Token("?", currentLine, Q_MARK));
			return tokens.back();
		case '(':
			tokens.push_back(Token("(", currentLine, LEFT_PAREN));
			return tokens.back();
		case ')':
			tokens.push_back(Token(")", currentLine, RIGHT_PAREN));
			return tokens.back();
		case ':':
			colonDash();
			return tokens.back();
		case '#':
			comment();
			return getToken();
		case '*':
			tokens.push_back(Token("*", currentLine, MULTIPLY));
			return tokens.back();
		case '+':
			tokens.push_back(Token("+", currentLine, ADD));
			return tokens.back();
		case '\'':
			inString();
			return tokens.back();
		default:
			checkKeys();
			return tokens.back();
	}
		
} 

void Scanner::colonDash()
{
	if (in.peek() == '-')
	{
		in.get();
		tokens.push_back(Token(":-", currentLine, COLON_DASH));
	}
	else
	{
		tokens.push_back(Token(":", currentLine, COLON));
	}
}

void Scanner::comment()
{
	string comment;
		comment += crnt;
		if (in.peek() == '|')
		{
			blockComment();
		}
		else
		{
			while (in.peek() != '\n' && (!in.eof()))
			{
                if(!in.eof())
                    {           
        				comment += in.get();
                    }			
            }
			//tokens.push_back(Token(comment, currentLine, COMMENT));
		}

}


void Scanner::blockComment()
{
	string comment;
	comment += '#';
	comment += in.get();
	while (!in.eof())
	{
		if (in.peek() == '\n')
		{
			++currentLine;
		}
		if (in.peek() != '|' && in.peek() >= 0)
		{
			        comment += in.get();
        }
		else
		{
            if(!(in.peek() >=0))
            {
            }
            else
            {			
                comment += in.get();
			    if (in.peek() == '#')
			    {
				    comment += in.get();
				   //tokens.push_back(Token(comment, beginingLine, COMMENT));
				break;
			    }
            }
		}

	}
	if (tokens.empty() || tokens.back().getTypeString() != "COMMENT")
	{
		//tokens.push_back(Token(comment, beginingLine, UNDEFINED));
	}
}
	
	

void Scanner::inString() 
{
	string inputString;
	inputString += crnt;
	while (!in.eof()) {
		if (in.peek() == '\n')
		{
			++currentLine;
		}
		if (in.peek() == '\'')
		{
			inputString +=in.get();
			if (in.peek() != '\'')
			{
				tokens.push_back(Token(inputString, beginingLine, STRING));
				break;
			}
		}
		if (!in.eof())
		{
			inputString += in.get();
		}
		
	}
	if (inputString.back() != '\'') {
		tokens.push_back(Token(inputString, beginingLine, UNDEFINED));
	}
}

void Scanner::keywords(string key)
{
	if (key == "Schemes") {
		tokens.push_back(Token(key, beginingLine, SCHEMES));
	}
	else if (key == "Facts") {
		tokens.push_back(Token(key, beginingLine, FACTS));
	}
	else if (key == "Rules") {
		tokens.push_back(Token(key, beginingLine, RULES));
	}
	else if (key == "Queries") {
		tokens.push_back(Token(key, beginingLine, QUERIES));
	}
	else {
		tokens.push_back(Token(key, beginingLine, ID));
	}
}


void Scanner::checkKeys()
{
	string key;
	if (!isalpha(crnt) && !isspace(crnt))
	{
		key += crnt;
		tokens.push_back(Token(key, currentLine, UNDEFINED));
	}
	else
	{
		key += crnt;
		while ((isalpha(in.peek()) || isdigit(in.peek())))
		{
			key += in.get();
		}
		keywords(key);
	}
}

void Scanner::whiteSpace()
{
	while (isspace(crnt)) 
	{
		if (crnt == '\n')
		{
			currentLine++;
		}
		crnt = in.get();
		//in.peek();
	}
}
