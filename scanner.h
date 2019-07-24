#ifndef SCANNER_H
#define SCANNER_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "token.h"
using namespace std;

class Scanner
{
public:
	Scanner(string input)
	{
		in.open(input.c_str());
		currentLine = 1;
		beginingLine = currentLine;
		crnt = ' ';
	}
	~Scanner() {
		this->in.clear();
		this->in.close();
	}

	vector<Token> getAllTokens(); //Write the definitions
	Token getToken();
	void whiteSpace();
	void colonDash();
	void comment();
	void inString();
	void checkKeys();
	void blockComment();
	void keywords(string key);

private:
	int currentLine;
	ifstream in;
	int crnt;
	int beginingLine;
	vector<Token> tokens;
};

#endif // !SCANNER_H