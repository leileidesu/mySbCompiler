//
// Created by Administrator on 2022-09-30.
//

#ifndef OUT_PARSING_H
#define OUT_PARSING_H
#include<vector>
#include"token.h"
#include <fstream>
#include"synType.h"
#include"exception.h"
class node {
public:
    virtual  void output(std::ofstream &outputStream) =0;
    virtual synType nodeTypeEnum()=0;
    vector<node*> sonNodes;
    explicit node(vector<node*> a){
        this->sonNodes=a;
    }
    virtual void exceptionHandler()=0;
    virtual void IRVisit()=0;
};
class tokenNode:public node,public token {
public:

    string nodeType="tokenNode";
    synType nodeTypeEnum()override{
        return TokenNode_;
    }
    tokenNode(token * a,vector<node*> b):node(b),token(a->str,a->type,a->line)
    { }
     void output(std::ofstream &outputStream) override {
        outputStream<<numToEnum[this->type]<<" "<<this->str<<endl;
//         outputStream<<numToEnum[this->type]<<" "<<this->str<<" "<<this->line<<endl;
    };
    static int analyzing(node** ret,vector<token*>::iterator &i,typeCode code);
    void exceptionHandler() override{};
    void IRVisit() override{};

};
class BlockItem : public node{
public:

    string nodeType="BlockItem";
    synType nodeTypeEnum() override{
        return BlockItem_;
    }
     void output(std::ofstream &outputStream) override {

    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class symbolTable;
class Block : public node{
public:

    string nodeType="Block";
    symbolTable * table;
    synType nodeTypeEnum() override{
        return Block_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<Block>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class Stmt : public node{
public:

    string nodeType="Stmt";
    string label;
    synType nodeTypeEnum() override{
        return Stmt_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<Stmt>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override;
};
class FuncType : public node{
public:

    string nodeType="FuncType";
    synType nodeTypeEnum() override{
        return FuncType_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<FuncType>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class FuncFParam : public node{
public:

    string nodeType="FuncFParam";
    synType nodeTypeEnum() override{
        return FuncFParam_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<FuncFParam>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class FuncFParams : public node{
public:
    string nodeType="FuncFParams";
    synType nodeTypeEnum() override{
        return FuncFParams_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<FuncFParams>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class FuncDef : public node{
public:
    string nodeType="FuncDef";
    synType nodeTypeEnum() override{
        return FuncDef_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<FuncDef>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override;
};
class InitVal : public node{
public:

    string nodeType="InitVal";
    synType nodeTypeEnum() override{
        return InitVal_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<InitVal>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class VarDef : public node{
public:

    string nodeType="VarDef";
    synType nodeTypeEnum() override{
        return VarDef_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<VarDef>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override;
};
class ConstInitVal : public node{
public:

    string nodeType="ConstInitVal";
    synType nodeTypeEnum() override{
        return ConstInitVal_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<ConstInitVal>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class ConstDef : public node{
public:

    string nodeType="ConstDef";
    synType nodeTypeEnum() override{
        return ConstDef_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<ConstDef>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override;
};
class VarDecl : public node{
public:

    string nodeType="VarDecl";
    synType nodeTypeEnum() override{
        return VarDecl_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<VarDecl>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class ConstDecl : public node{
public:

    string nodeType="ConstDecl";
    synType nodeTypeEnum() override{
        return ConstDecl_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<ConstDecl>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class Decl : public node{
public:

    string nodeType="Decl";
    synType nodeTypeEnum() override{
        return Decl_;
    }
     void output(std::ofstream &outputStream) override {

    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class MainFuncDef : public node{
public:

    string nodeType="MainFuncDef";
    synType nodeTypeEnum() override{
        return MainFuncDef_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<MainFuncDef>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override;
};
class CompUnit : public node{
public:
    string nodeType="CompUnit";
    synType nodeTypeEnum() override{
        return CompUnit_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<CompUnit>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override;
};
class Exp : public node{
public:
    string nodeType="Exp";
    synType nodeTypeEnum() override{
        return Exp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<Exp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};;
    void IRVisit() override{};
};
class Cond : public node{
public:
    string nodeType="Cond";
    vector<int> group;
    int startBlock=-1;
    int ifblock=-1;
    int elseblock=-1;
    int final=-1;
    synType nodeTypeEnum() override{
        return Cond_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<Cond>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class LVal : public node{
public:
    string nodeType="LVal";
    synType nodeTypeEnum() override{
        return LVal_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<LVal>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override{};
};
class IntConst : public node{
public:
    string nodeType="IntConst";
    synType nodeTypeEnum() override{
        return IntConst_;
    }
     void output(std::ofstream &outputStream) override {

    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class Number : public node{
public:
    string nodeType="Number";
    synType nodeTypeEnum() override{
        return Number_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<Number>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class PrimaryExp : public node{
public:
    string nodeType="PrimaryExp";
    synType nodeTypeEnum() override{
        return PrimaryExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<PrimaryExp>"<<endl;  
//        outputStream<<this->nodeType<<endl;
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class FuncRParams : public node{
public:
    string nodeType="FuncRParams";
    synType nodeTypeEnum() override{
        return FuncRParams_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<FuncRParams>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class ConstExp : public node{
public:
    string nodeType="ConstExp";
    synType nodeTypeEnum() override{
        return ConstExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<ConstExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class UnaryOp : public node{
public:
    string nodeType="UnaryOp";
    synType nodeTypeEnum() override{
        return UnaryOp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<UnaryOp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
};
class UnaryExp: public node{
public:
    string nodeType="UnaryExp";
    synType nodeTypeEnum() override{
        return UnaryExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<UnaryExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override;
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class MulExp: public node{
public:
    string nodeType="MulExp";
    synType nodeTypeEnum() override{
        return MulExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<MulExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class AddExp: public node{
public:
    string nodeType="AddExp";
    synType nodeTypeEnum() override{
        return AddExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<AddExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class RelExp: public node{
public:
    string nodeType="RelExp";
    synType nodeTypeEnum() override{
        return RelExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<RelExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);

};
class EqExp: public node{
public:
    string nodeType="EqExp";
    synType nodeTypeEnum() override{
        return EqExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<EqExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class LAndExp: public node{
public:
    string nodeType="LAndExp";
    synType nodeTypeEnum() override{
        return LAndExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<LAndExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};
class LOrExp : public node{
public:
    string nodeType="LOrExp";
    synType nodeTypeEnum() override{
        return LOrExp_;
    }
     void output(std::ofstream &outputStream) override {
        outputStream<<"<LOrExp>"<<endl;  
    };
    using node::node;
    static int analyzing(node** ret,vector<token*>::iterator &i);
    void exceptionHandler() override{};
    void IRVisit() override{};
    node* IRVisit(node ** ret);
};

void lastOrder(node * root,std::ofstream &stream);
node * parsing();
#endif //OUT_PARSING_H