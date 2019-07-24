#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Relation.h"

class Database : public map<string, Relation>
{

};

#endif