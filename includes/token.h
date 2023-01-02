#ifndef TOKEN_H
#define TOKEN_H

#include "typeCode.h"
#include<iostream>
using namespace std;
// extern vector <string> numToEnum;
class token
{
public:
    string str;
    enum typeCode type;
    int line;
    token(string a, enum typeCode b, int c)
    {
        this->str = a;
        this->type = b;
        this->line = c;
    }
    static  vector<token*> tokens;
};

// extern vector<token*> tokens;


#endif