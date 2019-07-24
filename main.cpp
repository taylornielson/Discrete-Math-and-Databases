#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "Interpreter.h"
#include "Database.h"
using namespace std;


int main()
{
	Datalog myDataLog;
	vector<string> files;
	//files.push_back("test1.txt");
	files.push_back("test2.txt");
	files.push_back("test3.txt");
	files.push_back("test4.txt");
	files.push_back("test5.txt");
	files.push_back("test6.txt");
	files.push_back("test7.txt");
	files.push_back("test8.txt");
	files.push_back("test9.txt");
	files.push_back("test10.txt");
	string pause;
	for (unsigned int i = 0; i < files.size(); ++i)
	{
		string pause;
		Datalog myDatalog;
		Scanner Scanner(files[i]);
		vector<Token> tokenVector = Scanner.getAllTokens();
		Parser myParse(tokenVector);
		myDatalog = myParse.ParseAll();
		Interpreter myInterpreter(myDatalog);
		cout << endl;
	}

	getline(cin, pause);
}



/*
int main(int argc, char* argv[]) {
	string fileName = argv[1];
	string pause;
	Datalog myDatalog;
	Scanner Scanner(fileName);
	vector<Token> tokenVector = Scanner.getAllTokens();
	Parser myParse(tokenVector);
	myDatalog = myParse.ParseAll();
	Interpreter myInterpreter(myDatalog);
	getline(cin,pause);
	return 0;

}; */
