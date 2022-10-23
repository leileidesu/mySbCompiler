//
// Created by Administrator on 2022-10-11.
//
#include"includes/exception.h"
#include"includes/parsing.h"
#include"includes/symbolTable.h"
symbolTable* currentTable;
symbolTable* rootTable;
extern node *  ASTRoot;
node * currentParse;
string funcName;
int inWhile=0;
vector<myException*> exceptions={};
int calDim(node * exp,int line)
{
    if(exp->nodeTypeEnum()==LVal_)
    {///LVal → Ident {'[' Exp ']'}
        vector<node *> sons=exp->sonNodes;
        string name=((tokenNode*)sons[0])->str;
        int found=0;
        int realdim=0;
        symbolTable* tmpTable=currentTable;
        while(tmpTable!= nullptr){
            if(tmpTable->varSymbols.find(name)!=tmpTable->varSymbols.end())
            {
                found=1;
                realdim=tmpTable->varSymbols[name]->dim;
                break;
            }
            tmpTable=tmpTable->parent;
        }
        if(found==0)
        {
            cout<<"55"<<endl;
            throw undefined_ident(line);
            return -1;///未定义
        }
        int numofexp=0;
        for(auto x:sons)
        {
            if(x->nodeTypeEnum()==Exp_)
                numofexp++;
        }
        return realdim-numofexp;
    }
    if(exp->nodeTypeEnum()==UnaryExp_)
    {
        vector<node *> sons=exp->sonNodes;
        if(sons[0]->nodeTypeEnum()==UnaryOp_)
        {
            return calDim(sons[1],line);
        }else if(sons[0]->nodeTypeEnum()==PrimaryExp_){
            return calDim(sons[0],line);
        }
        else
        {
            string name=((tokenNode*)sons[0])->str;
            if(rootTable->funcSymbols.find(name)==rootTable->funcSymbols.end())
            {
                throw undefined_ident(line);
                return -1;
            }
            vector<struct param> pms=rootTable->funcSymbols[name]->params;
            int numofxingcan=pms.size();
            int numofshican=0;
            if(sons[2]->nodeTypeEnum()==FuncRParams_)
            {
                vector<node *>lists=sons[2]->sonNodes;
                for(auto n:lists){
                    if(n->nodeTypeEnum()==Exp_)
                        numofshican++;
                }
            }
            cout<<"heihei "<<numofshican<<numofshican<<endl;
            if(numofshican!=numofxingcan)
            {
                throw mismatch_param_num(line);
                return -2;//参数数量不对
            }
            if(sons[2]->nodeTypeEnum()==FuncRParams_)
            {
                vector<node *>lists=sons[2]->sonNodes;
                int index=0;
                for(auto n:lists){
                    if(n->nodeTypeEnum()==Exp_)
                    {
                        int zhi= calDim(n,line);
                        if(zhi!=pms[index].dim)
                        {
                            throw mismatch_param_type(line);
                            return -3;//参数维数不对
                        }
                    }
                }
            }
            int rt=rootTable->funcSymbols[name]->retType;
            if(rt==1)return 0;
            else return -1;
        }
    }
    if(exp->nodeTypeEnum()==PrimaryExp_)
    {
        vector<node *> sons=exp->sonNodes;
        if(sons.size()>1)
        {
            return calDim(sons[1],line);///exp
        }else if(sons[0]->nodeTypeEnum()==LVal_)
        {
            return calDim(sons[0],line);///lval
        }else return 0;
    }
    if(exp->nodeTypeEnum()==Exp_)
    {
        if(exp->sonNodes.size()==1)
        {
            exp=exp->sonNodes[0];///addexp
            if(exp->sonNodes.size()==1)
            {
                exp=exp->sonNodes[0];///mulexp
                if(exp->sonNodes.size()==1)
                {
                    exp=exp->sonNodes[0];///unaryexp
                    return calDim(exp,line);
                }
            }
        }
        return 0;
    }
    return 0;
}
int shouldReturn(symbolTable * nowTable){///判断这个节点所在的函数该不该有返回值，要层层追溯因为可以嵌套多个block{{{{}}}}
    symbolTable* tmpTable=nowTable;

    while(tmpTable->belong== nullptr)
    {
        cout<<"im here"<<endl;
        tmpTable=tmpTable->parent;
    }
    return tmpTable->belong->retType;
}
int hasReturn(node * block ,int&line )
{
    ///这里是block
    vector<node *> sons=block->sonNodes;
    line=((tokenNode*)sons[sons.size()-1])->line;
    ///这是最后一个blockitem
    if(sons.size()<3)
        return 0;
    node * blockItem=sons[sons.size()-2];///还没考虑空block
    if(blockItem->sonNodes[0]->nodeTypeEnum()==Decl_)
        return 0;
    node * stmt=blockItem->sonNodes[0];
//    exit(0);
    if(stmt->sonNodes[0]->nodeTypeEnum()==TokenNode_&&((tokenNode*)stmt->sonNodes[0])->type==RETURNTK&&stmt->sonNodes.size()>2)
    {

        return 1;
    }
    return 0;
}

void ConstDef::exceptionHandler() {
    vector<node *> sons=this->sonNodes;
    string name=((tokenNode*)sons[0])->str;

    int line=((tokenNode*)sons[0])->line;
    if(currentTable->varSymbols.find(name)!=currentTable->varSymbols.end())
    {
        ///重复定义
        exceptions.push_back( new duplicated_ident(line));
        return ;
    }
    int isConst=1;
    int dim=0;
    int len=sons.size();
    for(int i=0;i<len;i++){
        if(sons[i]->nodeTypeEnum()==ConstExp_)
        {
            dim++;
        }
    }
    (currentTable->varSymbols).insert(pair<string,varSymbol*>(name,new varSymbol(name,isConst,dim)));
    return;
}
void VarDef::exceptionHandler(){
    vector<node *> sons=this->sonNodes;
    string name=((tokenNode*)sons[0])->str;

    int line=((tokenNode*)sons[0])->line;
    if(currentTable->varSymbols.find(name)!=currentTable->varSymbols.end())
    {
        ///重复定义
       // cout<< duplicated_ident(line).what()<<endl;
        exceptions.push_back( new duplicated_ident(line));
        return ;
    }
    int isConst=0;
    int dim=0;
    int len=sons.size();
    for(int i=0;i<len;i++){
        if(sons[i]->nodeTypeEnum()==ConstExp_)
        {
            dim++;
        }
    }
    (currentTable->varSymbols).insert(pair<string,varSymbol*>(name,new varSymbol(name,isConst,dim)));
//exit(0);
return;
}
void FuncDef::exceptionHandler(){
    vector<node *> sons=this->sonNodes;
    string name=((tokenNode*)sons[1])->str;

    funcName=name;
    ///函数名
    int line=((tokenNode*)sons[1])->line;
    if(currentTable->funcSymbols.find(name)!=currentTable->funcSymbols.end())
    {
        ///函数符号表重复定义
        exceptions.push_back( new duplicated_ident(line));
        return ;
    }
    ///如果没定义就new一个函数symbol

    int retType=1;
    if(((tokenNode*)sons[0]->sonNodes[0])->type==VOIDTK){
        retType=0;
    }
    cout<<"a new fun "<<name<<retType<<endl;
    auto *tmp1=new funcSymbol(name,retType,{});
    if(sons[3]->nodeTypeEnum()==FuncFParams_){
        vector<node *> sonsons=sons[3]->sonNodes;
        int lenlen=sonsons.size();
        for(int i=0;i<lenlen;i+=2){
            vector<node *>sonsonsons=sonsons[i]->sonNodes;
            ///BType Ident ['[' ']' { '[' ConstExp ']' }]
            ///一个参数
            int lenlenlen=sonsonsons.size();
            int dim=0;
            string name1=((tokenNode*)sonsonsons[1])->str;
            ///找形参还有没有name1
            for(auto & param : tmp1->params)
            {
                if(param.name==name1)
                {
                    //cout<< duplicated_ident(line).what()<<endl;
                    exceptions.push_back( new duplicated_ident(line));
                    cout<<"55here"<<endl;
continue;
                }
            }
            for(int j=0;j<lenlenlen;j++)
            {
                if(sonsonsons[j]->nodeTypeEnum()==TokenNode_&&((tokenNode*)sonsonsons[j])->type==LBRACK){
                    dim++;
                }
            }
            tmp1->params.push_back({name1,dim});


        }
    }
    (currentTable->funcSymbols).insert(pair<string,funcSymbol*>(name,tmp1));
//    cout<<"i had a new function params";
//    for(auto i :(currentTable->funcSymbols)[name]->params)
//    {
//        cout<<i.name<<" "<<i.dim;
//    }
    if(retType==0)
        return;
    int errorLine;
    int hr= hasReturn(sons[sons.size()-1],errorLine);
    if(retType==1&&hr==0)
    {
        exceptions.push_back( new mis_return(errorLine));
        return ;
    }
    return ;
}
void MainFuncDef::exceptionHandler(){
//    exit(0);
    vector<node *> sons=this->sonNodes;
    string name=((tokenNode*)sons[1])->str;
    funcName=name;
    int line=((tokenNode*)sons[1])->line;
    if(currentTable->funcSymbols.find(name)!=currentTable->funcSymbols.end())
    {
        ///重复定义

        //cout<< duplicated_ident(line).what()<<endl;
        exceptions.push_back( new duplicated_ident(line));
        return ;
    }

    int retType=1;
    (currentTable->funcSymbols).insert(pair<string,funcSymbol*>(name,new funcSymbol(name,retType,{})));

    int errorLine;

    int hr= hasReturn(sons[sons.size()-1],errorLine);

    if(retType==1&&hr==0)
    {

        exceptions.push_back( new mis_return(errorLine));

        return ;
    }
    return ;

}
void  LVal::exceptionHandler(){
    vector<node *> sons=this->sonNodes;
    string name=((tokenNode*)sons[0])->str;

    int line=((tokenNode*)sons[0])->line;
    symbolTable* tmpTable=currentTable;
    int found=0;
    while(tmpTable!= nullptr){
        if(tmpTable->varSymbols.find(name)!=tmpTable->varSymbols.end())
        {
            found=1;
            break;
        }
        tmpTable=tmpTable->parent;
    }
    if(!found)
    {
cout<<"shit1";
for(auto i : exceptions)
{
    if(i->line==line&&i->code=="c")
        return;
}
        exceptions.push_back( new undefined_ident(line));
        return;
    }

}
void UnaryExp::exceptionHandler(){///-1 fail -2 void 0 num...

    vector<node *> sons=this->sonNodes;
    if(sons[0]->nodeTypeEnum()==PrimaryExp_||sons[0]->nodeTypeEnum()==UnaryOp_)return;
    string name=((tokenNode*)sons[0])->str;
///Ident '(' [FuncRParams] ')'
    int line=((tokenNode*)sons[0])->line;
    try{
        int dim= calDim(this,line);
    }catch (myException &err)
    {
        exceptions.push_back(new myException(err));
    }


    return;

}
void Stmt::exceptionHandler(){
    cout<<"reach stmt"<<endl;
    vector<node *> sons=this->sonNodes;
    int line=((tokenNode*)sons[0])->line;

    if((sons[0]->nodeTypeEnum()==TokenNode_)&&(((tokenNode*)sons[0])->type==RETURNTK))
    {
        cout<<"im return"<<endl;
        int shouldReturn1= shouldReturn(currentTable);
        if(!shouldReturn1&&sons.size()>2)
        {
            //cout<< surplus_return(line).what()<<endl;
            exceptions.push_back( new surplus_return(line));
        }
    }
    if(sons[0]->nodeTypeEnum()==LVal_&&(sons[1]->nodeTypeEnum()==TokenNode_)&&(((tokenNode*)sons[1])->type==ASSIGN))
    {
        ///可以处理const错误
        string identName=((tokenNode*)sons[0]->sonNodes[0])->str;
        symbolTable* tmpTable=currentTable;
        line=((tokenNode*)sons[0]->sonNodes[0])->line;
        int isConst=0;
        int found=0;
        while(tmpTable!= nullptr){
            if(tmpTable->varSymbols.find(identName)!=tmpTable->varSymbols.end())
            {
                found=1;
                isConst=tmpTable->varSymbols[identName]->isConst;
                break;
            }
            tmpTable=tmpTable->parent;
        }
        if(isConst==1)
        {
            exceptions.push_back( new modify_const(line));
        }
    }
    if((sons[0]->nodeTypeEnum()==TokenNode_)&&(((tokenNode*)sons[0])->type==PRINTFTK))
    {

        ///如果字符串本身有问题就不管了
        string fs=((tokenNode*)sons[2])->str;
        int len=fs.length();
        int i;
        int willThrow=0;
        for(i=1;i<len-1;)
        {
            char a=fs[i];
            cout<<i<<" "<<len<<endl;
            if(a==37&&fs[i+1]==100)
            {
                i+=2;
                continue;
            }
            else if(a==92&&fs[i+1]==110)
            {
                i+=2;
                continue;
            }
            else
            {
                if(!(a==32||a==33||a>=40&&a<92||a>92&&a<=126))
                {
                    ///illegal
                    willThrow=1;
                    break;
                }
                else i++;
            }
        }
        if(willThrow)
            return;
        int cntOfPercent=0;
        for(auto ch:fs)
        {
            if(ch=='%')
                cntOfPercent++;
        }
        int cntOfVar=0;
        for(auto nd :sons)
        {
            if(nd->nodeTypeEnum()==Exp_)
            {
                cntOfVar++;
            }
        }
        if(cntOfPercent!=cntOfVar)
        {
            exceptions.push_back( new mismatch_print(line));
        }

    }
    if((sons[0]->nodeTypeEnum()==TokenNode_)&&(((tokenNode*)sons[0])->type==BREAKTK||((tokenNode*)sons[0])->type==CONTINUETK))
    {
        if(inWhile==0)
        {
            //cout<< mis_while(line).what()<<endl;
            exceptions.push_back( new mis_while(line));
        }
    }
//    if((sons[0]->nodeTypeEnum()==TokenNode_)&&(((tokenNode*)sons[0])->type==WHILETK))
//    {
//        inWhile+=1;
//    }
//    cout<<"here"<<endl;
}
void lastOrder1(node * a){
        cout<<numToSyn[a->nodeTypeEnum()]<<endl;
        if(a != nullptr){
            vector<node*>::iterator i;
            if(a->nodeTypeEnum()==Block_)
            {
                cout<<"funcname="<<funcName<<endl;
                for(auto i: currentTable->funcSymbols){
                    cout<<i.first<<endl;
                }

                if(funcName!=""&& currentTable->funcSymbols.find(funcName)!=currentTable->funcSymbols.end())
                {
                    vector<struct param>xingCan=(currentTable->funcSymbols.find(funcName))->second->params;
                    currentTable=new symbolTable(currentTable, (currentTable->funcSymbols.find(funcName))->second);


                    int len=xingCan.size();

                    for(int i=0;i<len;i++)
                    {
                        string name=xingCan[i].name;
                        int dim=xingCan[i].dim;
                        currentTable->varSymbols.insert(pair<string,varSymbol*>(name,new varSymbol(name,0,dim)));
                    }
                    funcName="";
                }

                else
                {
                    currentTable=new symbolTable(currentTable, nullptr);
                }
                cout<<"------- into a new block -------"<<endl;
                for(auto jj:currentTable->varSymbols)
                {
                    cout<<jj.first<<endl;
                }
                cout<<"-----------------------"<<endl;
            }
            a->exceptionHandler();

            for(i = a->sonNodes.begin(); i < a->sonNodes.end (); ++i)
            {
                if(a->nodeTypeEnum()==Stmt_&&a->sonNodes[0]->nodeTypeEnum()==TokenNode_&&((tokenNode*)a->sonNodes[0])->type==WHILETK&&(*i)->nodeTypeEnum()==Stmt_)
                {
                    inWhile++;
                    lastOrder1(*i);
                    inWhile--;
                }
                else lastOrder1(*i);
            }

            if(a->nodeTypeEnum()==Block_)
            {
                currentTable=currentTable->parent;
            }
        }
    }
void exceptionVisitor(){
    //cout<<exceptions.size()<<endl;
    currentTable=new symbolTable(nullptr, nullptr);
    rootTable=currentTable;///这个就是root表了，最根部的符号表
    lastOrder1(ASTRoot);
}
