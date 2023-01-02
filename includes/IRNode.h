//
// Created by Administrator on 2022-10-24.
//

#ifndef MYSBCOMPILER_IRNODE_H
#define MYSBCOMPILER_IRNODE_H




#include"symbolTable.h"
#include<vector>
#include<string>


using namespace std;
enum valueType{
    VOID,
    I1,
    I32,
    LABEL,
    ARRAY,
    POINTER,
    FUNCTION,
};
class Use;
class Value;
class User;
class Use{
public:
    Value *Val;
    User *U;
    Use(Value *a,User* b)
    {
        Val=a;
        U=b;
    }
};
class Value {
public:

    vector<Use*> useList;
    valueType type;
    string name;
    static int num;
    bool isConst;
    int needAllo=0;
    int isglobal=0;
string label11;
    Value(){}
    Value(string a, valueType b,int iscon,int needAllo){
        isConst=iscon;
        if(needAllo==1)
        {
            //cout<<"going to allocate"<<num<<endl;
            name= to_string(num++);
        }else
        name= a;
        type=b;
    }
    string getName() {
        return name;
    };

};
class Pointer:public Value{
    vector<int> dims;
};
class User : public Value {
public:
    std::vector<Value*> operandList;
    void addOperand(Value* a){
        operandList.push_back(a);
        Use(a,this);
    }
    User(string a,valueType b,int c):Value(a,b,0,c)
    {
    }
    User(){}
};
class Function;
class Instruction;
class basicBlock:public Value{
public:
    Function *Parent;
    vector<Instruction *>Ins;
    basicBlock(Function * a):Value("",LABEL,1,1){
        Parent=a;

    }
    void addIns(Instruction *a){
        Ins.push_back(a);
    }
};

class argument:public Value
{
public:
    string name;
    int dim;
};
class Module;
class symbolTable;
class Function:public User{
public:
    string funcName;
    bool isDecl;
    vector<basicBlock*> BasicBlockList={};
    vector<Value*>params;
//    symbolTable * symbolT;
    int retType;
    Module * parent;
    funcSymbol * funcs;
    Function(funcSymbol* a,int isd,Module * f):User(a->name,FUNCTION,0){
        funcName=a->name;
        isDecl=isd;
        parent=f;
        funcs=a;
        retType=a->retType;
    }
};
class localVar:public User{
public:
    varSymbol *sym;
    int numarray=0;
    vector<int>dims;

//    localVar(varSymbol * a):Value(a->name,a->dim==0?I32:ARRAY,a->isConst,1){
    localVar(varSymbol * a,int isg):User(a->name,a->dim==0?I32:ARRAY,1-isg){
        sym=a;
        a->addr=this;
    }
    string typestr(){
        cout<<"typestr"<<endl;
        if(type==I32) {
            cout<<"if 1"<<endl;
            if(sym!= nullptr&&sym->ispointer==0) {
                cout<<"if 11"<<endl;
                return "i32";
            }
            else {
                cout<<"if 12"<<endl;
                return "i32 *";
            }
        }
        else if(sym!= nullptr&&sym->dim==1)
        {
            cout<<"yiwei"<<endl;
            cout<<sym->dims[0]<<endl;
            return "["+ to_string(sym->dims[0])+" x i32]";
        }

        else
            return "["+ to_string(sym->dims[0])+" x ["+to_string(sym->dims[1])+" x i32]]";
    }
};
class pointer : public Value{
public:
    int lastdim=0;
    localVar *lv;
    Instruction * getele;
    pointer(localVar *a,Instruction *b)
    {
        type=POINTER;
        lv=a;
        getele=b;
    }

};
class GlobalVariable:public localVar{
public:
    Module * parent;


    vector<vector<int> > initval;
    GlobalVariable(varSymbol * a,Module*b): localVar(a,1)
    {
        isglobal=1;
        parent=b;

    }
    string initstr(){

        if(sym->dim==0)
        {
            return to_string(initval[0][0]);
        }
        else if(sym->dim==1)
        {
            if(initval[0].empty())
            {
                return "zeroinitializer";
            }
            string ret="[";
            for (auto i : initval[0])
            {
                ret+=(" i32 "+ to_string(i)+",");
            }
            ret.pop_back();
            ret+="]";
            return ret;
        }else
        {
            int subdim=sym->dims[1];
            string ret="[";
            for(auto i : initval)
            {
                ret+=("["+ to_string(subdim)+" x i32] ");
                if(i.empty())
                {
                    ret=ret+"zeroinitializer,";
                }
               else
                {
                    ret+="[";
                    for(auto j : i)
                    {
                        ret=ret+" i32 "+ to_string(j)+",";
                    }
                    ret.pop_back();
                    ret+="],";
                }

            }ret.pop_back();ret+="]";
            return ret;
        }
        //@aaa = global [6 x i32] [i32 1, i32 2, i32 3, i32 0, i32 0, i32 0], align 16
        //[[5 x i32] [i32 1, i32 2, i32 0, i32 0, i32 0], [5 x i32] zeroinitializer, [5 x i32] zeroinitializer],
    }
};
class Module {
public:
    std::vector<Function*> funcList;
    std::vector<GlobalVariable*>globalList; //全局变量
};
class constant:public Value{
public:

    constant(valueType a,string b):Value(b,a,1,0){
        type=a;
    }
};
class constantInt :public constant{
public:
    int constValue;
    constantInt(int a): constant(I32, to_string(a))
    {
        constValue=a;
        label11="constantInt";
    }

};

//class constantBool :public constant{
//public:
////    bool constValue;
//    constantBool(bool a): constant(I1, to_string(a))
//    {
////        constValue=a;
//    }
//};

#endif //MYSBCOMPILER_IRNODE_H
