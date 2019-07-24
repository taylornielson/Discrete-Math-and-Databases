#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


 enum TokenType { COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, 
				QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, ENDFILE  };

 inline string typeToString(TokenType T) {
	switch (T) {
	case COMMA: return "COMMA"; break;
	case PERIOD: return "PERIOD"; break;
	case Q_MARK: return "Q_MARK"; break;
	case LEFT_PAREN: return "LEFT_PAREN"; break;
	case RIGHT_PAREN: return "RIGHT_PAREN"; break;
	case COLON: return "COLON"; break;
	case COLON_DASH: return "COLON_DASH"; break;
	case MULTIPLY: return "MULTIPLY"; break;
	case ADD: return "ADD"; break;
	case SCHEMES: return "SCHEMES"; break;
	case FACTS: return "FACTS"; break;
	case RULES: return "RULES"; break;
	case QUERIES: return "QUERIES"; break;
	case ID: return "ID"; break;
	case STRING: return "STRING"; break;
	case COMMENT: return "COMMENT"; break;
	case WHITESPACE: return "WHITESPACE"; break;
	case ENDFILE: return "EOF"; break;
	default: return "UNDEFINED"; break;
	}
	
}




class Token {
    public:
	Token() {};
    Token(string content, int lineNum, TokenType type)
    {
        this->content = content;
        this->lineNum = lineNum;
        this->type = type;
    }
	string printToken()
	{
		stringstream ss;
		ss << "(" << typeToString(type) << "," << "\"" << content << "\"," << lineNum << ")" << endl;
		return ss.str();
	}
	string getTypeString()
	{
		return typeToString(type);
	}

	string getContent()
	{
		return content;
	}

	TokenType getType()
	{
		return type;
	}

    private:
    string content = "";
    int lineNum = -1;
    TokenType type;

};
#endif