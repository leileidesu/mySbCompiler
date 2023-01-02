//
// Created by Administrator on 2022-10-24.
//

#ifndef MYSBCOMPILER_INSTRUCTION_H
#define MYSBCOMPILER_INSTRUCTION_H
#include"IRNode.h"

enum BinaryOperator{
    ADD,
    SUB,
    MUL,
    SDIV,
    SREM,
    AND_,
    OR_,
};
static string BO[]={
        "add","sub","mul","sdiv",
        "srem","and","or"
};
class Instruction :public User{
public:
    basicBlock* parent;
    string label;
    Instruction(int allo):User("",I32,allo){

    }
    virtual string output()=0;
};
class binaryInstruction:public Instruction{
public:
    BinaryOperator binaryOperator;
    binaryInstruction(BinaryOperator a,Value * b,Value* c,valueType d): Instruction(1){
        label="binaryInstruction";
        binaryOperator=a;
        addOperand(b);
        addOperand(c);
        type=d;

    }
    string output(){
        string n1,n2;
        if(operandList[0]->isConst==1)
        {
            n1=operandList[0]->getName();
        }
        else
        {
            n1="%"+operandList[0]->getName();
        }
        if(operandList[1]->isConst==1)
        {
            n2=operandList[1]->getName();
        }
        else
        {
            n2="%"+operandList[1]->getName();
        }
        return "%"+name+" = "+BO[binaryOperator]+" i32 "+n1+", "+n2;
    }
};
class allocaInstruction:public Instruction{
public:
    valueType type;
    vector<int> arrayBias;
    localVar * rootvar;
    string output(){

        string str="%"+operandList[0]->getName()+" = "+"alloca "+rootvar->typestr();
        for(int i=0;i<rootvar->numarray;i++)
        {
            str+="*";
        }
        return str;
    }
    allocaInstruction(localVar * a,vector<int> b): Instruction(0){
        rootvar=a;
        label="allocaInstruction";
        addOperand(a);
        arrayBias=b;
    }
};
class zextInstruction:public Instruction{
public:
    Value * origin;
    string output(){
        return "%"+this->getName()+" = zext i1 %"+origin->getName()+" to i32";
    }
    //%Y = zext i1 true to i32              ; yields i32:1
    zextInstruction(Value* a): Instruction(1){
        origin=a;
    }
};
class zextInstruction1:public Instruction{
public:
    Value * origin;
    string output(){
        return "%"+this->getName()+" = zext i1 %"+origin->getName()+" to i32";
    }
    //%Y = zext i1 true to i32              ; yields i32:1
    zextInstruction1(Value* a): Instruction(1){
        origin=a;
    }
};
class retInstruction:public Instruction{
public:
    int retType=0;
    retInstruction(valueType a,Value * val): Instruction(0)
    {
        label="retInstruction";
        if(a==VOID)
        {
            retType=0;
        }
        else if(a==I32)
        {
            retType=1;
            addOperand(val);
        }
    }

    string output(){
        if(retType==1)
        {
            if(operandList[0]->isConst==1)
                return "ret i32 "+operandList[0]->getName();
            else return "ret i32 %"+operandList[0]->getName();
        }

        return "ret void";
    }
};
class callInstruction:public Instruction{
public:
    callInstruction(Value * val,vector<Value*> par): Instruction(((Function*)val)->retType==1?1:0)
    {
        label="callInstruction";
        addOperand(val);
        for(auto i:par){
            addOperand(i);
        }
    }

    string output(){
        Function* fun=(Function*)operandList[0];
        int ret=fun->retType;
        string retstr;
        retstr=(ret==1?"%"+name+" = ":"")+"call "+(ret==1?"i32 ":"void ")+"@"+fun->funcName+"(";


        for(int i=1;i<operandList.size();i++){
            auto x=fun->funcs;
            string type1="";
            if(fun->funcs->params[i-1].dim==0)
                type1+=" i32";
            else if(fun->funcs->params[i-1].dim==1)
                type1+= " i32*";
            else if(fun->funcs->params[i-1].dim==2)
                type1+= (" ["+ to_string(fun->funcs->params[i-1].last)+" x i32]*");
            Value *tmp=operandList[i];
            retstr+=type1;
            retstr+=" ";
            if(tmp->isConst==1)
                retstr+=tmp->getName();
            else retstr=retstr+"%"+tmp->getName();
            if(i<operandList.size()-1)
                retstr+=",";
        }
        retstr+=")";
        return  retstr;
    }
};
enum CmpOperator{
    EQ_,
    NE_,
    SGT_,
    SGE_,
    SLT_,
    SLE_
};
static string B1[]={
        "eq","ne","sgt","sge","slt","sle"
};
class icmpInstruction:public Instruction{
public:
    CmpOperator Operator;
    icmpInstruction(Value * val1,Value * val2,CmpOperator x): Instruction(1)
    {
        label="icmpInstruction";
        Operator=x;
        addOperand(val1);
        addOperand(val2);
        type=I1;
    }
    string output(){
        string n1,n2;
        if(operandList[0]->isConst==1)
        {
            n1=operandList[0]->getName();
        }
        else
        {
            n1="%"+operandList[0]->getName();
        }
        if(operandList[1]->isConst==1)
        {
            n2=operandList[1]->getName();
        }
        else
        {
            n2="%"+operandList[1]->getName();
        }

        return  "%"+name+" = icmp "+B1[Operator]+" i32 "+n1+", "+n2;
    }
};
class brInstruction:public Instruction{
public:
    basicBlock * bb1;
    basicBlock * bb2;
    brInstruction(Value * val,basicBlock *b1,basicBlock *b2): Instruction(0)
    {
        label="brInstruction";
        addOperand(val);
        bb1=b1;
        bb2=b2;
    }
    brInstruction(basicBlock *b1): Instruction(0)
    {label="brInstruction";
        bb1=b1;
    }
    brInstruction(): Instruction(0)
    {label="break";

    }
    string output(){
        if(operandList.size()!=0)
        return  "br i1 %"+operandList[0]->getName()+", label %"+bb1->getName()+", label %"+bb2->getName();
        else if (label!="break")
            return "br label %"+bb1->getName();
        else return "break";
    }
};
class getelementptrInstruction:public Instruction{
public:
    vector<Value*> arrayBias;
    localVar * rootval;
    string output(){
        string n;
        cout<<"gonna output getele"<<endl;
        if(rootval->isglobal==1)
        {
            n="@"+rootval->getName();
        }
        else
        {
            n="%"+rootval->getName();
        }
        string type1;
        int minus=arrayBias.size()-1;
        int pre=rootval->sym->dim;
        int real=pre;
        if(rootval->numarray!=0)
            real--;
        cout<<"realdim"<<real<<endl;

        if(real==0)
            type1="i32";
        else
        {
            if(real==1)
            {
                type1="["+ to_string(rootval->sym->dims[rootval->sym->dims.size()-1])+" x i32]";
            }else
            {
                type1="["+to_string(rootval->sym->dims[rootval->sym->dims.size()-2])+" x ["+to_string(rootval->sym->dims[rootval->sym->dims.size()-1])+" x i32]]";
            }
        }

cout<<type1<<endl;
        string ret="%"+getName()+" = getelementptr "+type1+", "+type1+"*  "+n;
        cout<<arrayBias.size()<<endl;
        for (auto i : arrayBias){
            auto namee=i->getName();
            if(i->isglobal==1)
            {
                namee="@"+i->getName();
            }
            else if(i->label11!="constantInt")
            {
                namee="%"+i->getName();
            }
            else
            {
                namee=i->getName();
            }
            cout<<namee<<endl;
            ret=ret+", i32 "+ namee;
        }
cout<<ret<<endl;
        return ret;
    }
    getelementptrInstruction(localVar * a,vector<Value*> b): Instruction(1){
        label="getelementptr";
        rootval=a;
        type=POINTER;
        arrayBias=b;

//        type=I1;
    }
};
class loadInstruction:public Instruction{
public:
    loadInstruction(Value *src): Instruction(1)
    {
        label="loadInstruction";
        addOperand(src);
    }
    string output(){
        //%10 = load i32, i32* %2
        string type1;
        if(operandList[0]->type!=POINTER)
        {//一定是个getelement得到的东西
            localVar * v1=(localVar*)operandList[0];

            type1=((localVar*)operandList[0])->typestr();
            return "%"+name+" = load "+type1+", "+type1+"* "+(operandList[0]->isglobal==1?"@":"%")+operandList[0]->getName();
        }

        else {
            cout<<"i got from gelele"<<endl;
            localVar * v1=((pointer*)operandList[0])->lv;

            int predim=v1->sym->dim;
            getelementptrInstruction* in=(getelementptrInstruction*)((pointer*)operandList[0])->getele;
            int k =in->arrayBias.size();
            cout<<k<<endl;
            k--;
            int realdim=predim-k;
            cout<<"hey here"<<endl;
            if(v1->numarray!=0)
            {
                realdim--;
            }
            if(realdim==0)
                type1="i32";
            else
            {
                if(realdim==1)
                {
                    type1="["+ to_string(v1->sym->dims[v1->sym->dims.size()-1])+" x i32]";
                }else
                {
                    type1="["+to_string(v1->sym->dims[v1->sym->dims.size()-2])+" x ["+to_string(v1->sym->dims[v1->sym->dims.size()-1])+" x i32]]";
                }
            }
            return "%"+name+" = load "+type1+", "+type1+"* "+"%"+in->getName();

        }
//        string type1="i32";

    }
};
class storeInstruction:public Instruction{
public:
    storeInstruction(Value *src,Value* dest): Instruction(0)
    {
        label="storeInstruction";
        addOperand(src);
        addOperand(dest);
    }
    string output(){
        cout<<"gonna output store0"<<endl;
        string n1,n2;
        if(operandList[0]->isConst==1)
        {
            n1=operandList[0]->getName();
        }
        else
        {
            n1="%"+operandList[0]->getName();
        }

        if(operandList[1]->type==POINTER)
        {
            cout<<"gonna output store1"<<endl;
            getelementptrInstruction* in=(getelementptrInstruction*)((pointer*)operandList[1])->getele;
            n2="%"+in->getName();
        }else
        {cout<<"gonna output store2"<<endl;
            if(operandList[1]->isglobal==1)
            {
                n2="@"+operandList[1]->getName();
            }
            else
            {
                n2="%"+operandList[1]->getName();
            }
        }
        cout<<"gonna output store3"<<endl;
        string type1;
        //第一个大概是load或者常量 第二个可以是什么pointer，或者local
        if(operandList[1]->type!=POINTER)
        {
            localVar * lv=(localVar*)operandList[1];
            int real=lv->sym->dim;
            string type1;
            cout<<"dim= "<<real<<endl;
            if(real==0)
                type1="i32";
            else
            {
                if(real==1)
                {
                    type1="["+ to_string(lv->sym->dims[lv->sym->dims.size()-1])+" x i32]";
                }else
                {
                    type1="["+to_string(lv->sym->dims[lv->sym->dims.size()-2])+" x ["+to_string(lv->sym->dims[lv->sym->dims.size()-1])+" x i32]";
                }
            }
            cout<<type1<<endl;
            return "store "+type1+" "+n1+", "+type1+"* "+n2;
        }else
        {
            cout<<"imhere"<<endl;
            localVar * v1=((pointer*)operandList[1])->lv;
            int predim=v1->sym->dim;//1
            if(v1->numarray!=0) predim--;
            getelementptrInstruction* in=(getelementptrInstruction*)((pointer*)operandList[1])->getele;
            auto arrayBias=in->arrayBias;
            int minus=arrayBias.size()-1;
            int real=predim-minus;

            cout<<"realdim"<<real<<endl;
            if(real==0)
                type1="i32";
            else
            {
                if(real==1)
                {
                    type1="["+ to_string(v1->sym->dims[v1->sym->dims.size()-1])+" x i32]";
                }else
                {
                    type1="["+to_string(v1->sym->dims[v1->sym->dims.size()-2])+" x ["+to_string(v1->sym->dims[v1->sym->dims.size()-1])+" x i32]";
                }
            }
            return "store "+type1+" "+n1+", "+type1+"* "+n2;
        }


        //store i32 %2, i32* %1
    }
};

#endif //MYSBCOMPILER_INSTRUCTION_H
