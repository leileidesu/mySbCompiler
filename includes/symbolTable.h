//
// Created by Administrator on 2022-10-10.
//

#ifndef OUT_SYMBOLTABLE_H
#define OUT_SYMBOLTABLE_H
#include<vector>
#include<string>
#include<map>
using namespace std;
class varSymbol{
public:
    string name;
    int isConst=0;
    int dim;
//    vector<int>dims={};///长度=数组维数，第n个数字：第n个维度的长度
    varSymbol(string a,int b,int c)
    {
        name=a;
        isConst=b;
        dim=c;
    }
};
struct param{
    string name;
    int dim;
};
class funcSymbol{
public:
    string name;
    vector<struct param>params={};//每个参数都是几维
    int retType;//0:void 1:int
    funcSymbol(string a,int b,vector<struct param> c)
    {
        name=a;
        retType=b;
        params=c;
    }
};
class symbolTable{
    ///函数定义，主函数定义，stmt会创造新的block，从而需要新的符号表
public:
    symbolTable * parent;
    funcSymbol * belong;///向上回溯，这个block最先属于哪个函数（涉及到return的判断
    map<string,varSymbol*> varSymbols={};
    map<string,funcSymbol*> funcSymbols={};
    symbolTable(symbolTable*a,funcSymbol * b)
    {
        parent=a;
        belong=b;
    }
};
///气死我了 让我想想，函数的声明和定义不分离，所以每一个声明接下来的第一个Block都是属于这个函数的
///我觉得可以每路过一个声明以后就记住函数名，当要进入新block之前，在当前block的table中找这个函数名的信息。
#endif //OUT_SYMBOLTABLE_H
