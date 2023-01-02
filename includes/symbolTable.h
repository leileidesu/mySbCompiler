//
// Created by Administrator on 2022-10-10.
//

#ifndef OUT_SYMBOLTABLE_H
#define OUT_SYMBOLTABLE_H
#include"parsing.h"
#include<iostream>
#include<vector>
#include<string>
#include<map>

using namespace std;
class localVar;

class varSymbol{
public:
    string name;
    int ispointer=0;
    int isConst=0;
    int dim;
    localVar *addr;
    int defined=0;
    vector<int>dims={};///闀垮害=鏁扮粍缁存暟锛岀n涓暟瀛楋細绗琻涓淮搴︾殑闀垮害
    //涓轰簡鏁扮粍鍋氱殑 todo
    vector<int>values={};
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
    int last;
};

class funcSymbol{
public:
    string name;
    vector<struct param>params={};//姣忎釜鍙傛暟閮芥槸鍑犵淮
    int retType;//0:void 1:int
    FuncFParams * ff;
    funcSymbol(string a,int b,vector<struct param> c)
    {
        name=a;
        retType=b;
        params=c;
    }
};

class symbolTable{
    ///鍑芥暟瀹氫箟锛屼富鍑芥暟瀹氫箟锛宻tmt浼氬垱閫犳柊鐨刡lock锛屼粠鑰岄渶瑕佹柊鐨勭鍙疯〃
public:
    symbolTable * parent;
    Block * parentBlock;
    funcSymbol * belong;///鍚戜笂鍥炴函锛岃繖涓猙lock鏈€鍏堝睘浜庡摢涓嚱鏁帮紙娑夊強鍒皉eturn鐨勫垽鏂?
    map<string,varSymbol*> varSymbols={};
    map<string,funcSymbol*> funcSymbols={};
    symbolTable(symbolTable*a,funcSymbol * b)
    {
        parent=a;
        belong=b;
    }
};
///姘旀鎴戜簡 璁╂垜鎯虫兂锛屽嚱鏁扮殑澹版槑鍜屽畾涔変笉鍒嗙锛屾墍浠ユ瘡涓€涓０鏄庢帴涓嬫潵鐨勭涓€涓狟lock閮芥槸灞炰簬杩欎釜鍑芥暟鐨?
///鎴戣寰楀彲浠ユ瘡璺繃涓€涓０鏄庝互鍚庡氨璁颁綇鍑芥暟鍚嶏紝褰撹杩涘叆鏂癰lock涔嬪墠锛屽湪褰撳墠block鐨則able涓壘杩欎釜鍑芥暟鍚嶇殑淇℃伅銆?
#endif //OUT_SYMBOLTABLE_H