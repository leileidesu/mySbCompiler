//
// Created by Administrator on 2022-10-08.
//

#ifndef OUT_EXCEPTION_H
#define OUT_EXCEPTION_H
#include <exception>
#include<string>
//#include"parsing.h"
using namespace std;
class myException : public exception{
public:
    int line;
    string code;
    const char* what() const throw() override{
        return (to_string(line)+" " +code).data();
    }

};
class illegal_char:public myException{
public:
    illegal_char(int a,string c="a")
    {
        line=a;
        code=c;
    }
};
class duplicated_ident:public myException{
public:
    duplicated_ident(int a,string c="b")
    {
        line=a;
        code=c;
    }
};
class undefined_ident:public myException{
public:
    undefined_ident(int a,string c="c")
    {
        line=a;
        code=c;
    }
};
class mismatch_param_num:public myException{
public:
    mismatch_param_num(int a,string c="d")
    {
        line=a;
        code=c;
    }
};
class mismatch_param_type:public myException{
public:
    mismatch_param_type(int a,string c="e")
    {
        line=a;
        code=c;
    }
};
class surplus_return:public myException{
public:
    surplus_return(int a,string c="f")
    {
        line=a;
        code=c;
    }
};
class mis_return:public myException{
public:
    mis_return(int a,string c="g")
    {
        line=a;
        code=c;
    }
};
class modify_const:public myException{
public:
    modify_const(int a,string c="h")
    {
        line=a;
        code=c;
    }
};
class mis_semicolon:public myException{
public:
    mis_semicolon(int a,string c="i")
    {
        line=a;
        code=c;
    }
};
class mis_r_parent:public myException{
public:
    mis_r_parent(int a,string c="j")
    {
        line=a;
        code=c;
    }
};
class mis_r_bracket:public myException{
public:

    mis_r_bracket(int a,string c="k")
    {
        line=a;
        code=c;
    }
};
class mismatch_print:public myException{
public:
    mismatch_print(int a,string c="l")
    {
        line=a;
        code=c;
    }
};
class mis_while:public myException{
public:
    mis_while(int a,string c="m")
    {
        line=a;
        code=c;
    }
};
//void lastOrder1(node * a);
void exceptionVisitor();
#endif //OUT_EXCEPTION_H
