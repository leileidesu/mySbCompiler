//
// Created by Administrator on 2022-10-02.
//

#include "includes/parsing.h"
node * ASTRoot= nullptr;
extern vector<myException*> exceptions;
int tokenNode::analyzing(node** ret,vector<token*>::iterator &i,typeCode code){
    string typename_ = numToEnum[code];
    cout<<"start analysing "<<typename_<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    if((*tmp)->type==code)
    {
        *ret=new tokenNode(*tmp,{});
        i=tmp;
        if(code==STRCON)
        {
            int willThrow=0;
            int len=(*tmp)->str.length();
            int i;
            for(i=1;i<len-1;)
            {
                char a=(*tmp)->str[i];
                cout<<i<<" "<<len<<endl;
                if(a==37&&(*tmp)->str[i+1]==100)
                {
                    i+=2;
                    continue;
                }
                else if(a==92&&(*tmp)->str[i+1]==110)
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
            {
                   // cout<< illegal_char((*tmp)->line).what()<<endl;
                    exceptions.push_back( new illegal_char((*tmp)->line));
            }



        }
        i++;


        return 0;
    }
    return -1;
}
int BlockItem::analyzing(node** ret,vector<token*>::iterator &i){
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if( Decl::analyzing(&res,tmp)==0){
        sonNodes.push_back(res);
    }
    else if(Stmt::analyzing(&res,tmp)==0){
        sonNodes.push_back(res);
    }
    else return -1;
    i=tmp;
    *ret=new BlockItem(sonNodes);
    return 0;
}
int Block::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing Block"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(tokenNode::analyzing(&res,tmp,LBRACE)==0){

        sonNodes.push_back(res);
    }else return -1;
    while(BlockItem::analyzing(&res,tmp)==0){

        sonNodes.push_back(res);
    }
    if(tokenNode::analyzing(&res,tmp,RBRACE)==0){

        sonNodes.push_back(res);
    }else return -1;
    i=tmp;
    *ret=new Block(sonNodes);
    cout<<"Block success"<<endl;
    return 0;
}
int Stmt::analyzing(node** ret,vector<token*>::iterator &i){
//    cout<<"start analysing stmt"<<endl;
//    cout<<"current token is "<<numToEnum[(*i)->type]<<endl;
    typeCode thisType=(*i)->type;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(thisType==LBRACE)
    {
        if(Block::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
            i=tmp;
            *ret=new Stmt(sonNodes);
            cout<<"Stmt success"<<endl;
            return 0;
        }else return -1;
    }
    if(thisType==IFTK)
    {
        if(tokenNode::analyzing(&res,tmp,IFTK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,LPARENT)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(Cond::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,RPARENT)==0){
            sonNodes.push_back(res);
        }else {

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;

            exceptions.push_back( new mis_r_parent(line));
            sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
        }
        if(Stmt::analyzing(&res,tmp)==0){
            ((Stmt*)res)->label="if";
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,ELSETK)==0){
            sonNodes.push_back(res);
            if(Stmt::analyzing(&res,tmp)==0)
            {
                ((Stmt*)res)->label="else";
                sonNodes.push_back(res);
            }else
            {
                sonNodes.pop_back();
                tmp--;
            }
        }
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }

    if(thisType==WHILETK){
        if(tokenNode::analyzing(&res,tmp,WHILETK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,LPARENT)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(Cond::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,RPARENT)==0){
            sonNodes.push_back(res);
        }else {

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;

            exceptions.push_back( new mis_r_parent(line));
            sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
        }
        if(Stmt::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }else return -1;
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    if(thisType==BREAKTK)
    {
        if(tokenNode::analyzing(&res,tmp,BREAKTK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
            sonNodes.push_back(res);
        }else{

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
            exceptions.push_back( new mis_semicolon(line));
            sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
        }
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    if(thisType==CONTINUETK)
    {
        if(tokenNode::analyzing(&res,tmp,CONTINUETK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
            sonNodes.push_back(res);
        }else {
            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
//             cout<<mis_semicolon(line).what()<<endl;
            exceptions.push_back( new mis_semicolon(line));
            sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
        }
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    if(thisType==RETURNTK)
    {
        if(tokenNode::analyzing(&res,tmp,RETURNTK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(Exp::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }
        if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
            sonNodes.push_back(res);
        }else {

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
//             cout<<mis_semicolon(line).what()<<endl;
            exceptions.push_back( new mis_semicolon(line));
            sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
        }
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    if(thisType==PRINTFTK)
    {
        if(tokenNode::analyzing(&res,tmp,PRINTFTK)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,LPARENT)==0){
            sonNodes.push_back(res);
        }else return -1;
        if(tokenNode::analyzing(&res,tmp,STRCON)==0){
            sonNodes.push_back(res);
        }else return -1;
        while(tokenNode::analyzing(&res,tmp,COMMA)==0){
            sonNodes.push_back(res);
            if(Exp::analyzing(&res,tmp)==0)
            {
                sonNodes.push_back(res);
            }else
            {
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
        if(tokenNode::analyzing(&res,tmp,RPARENT)==0){
            sonNodes.push_back(res);
        }else {

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
            exceptions.push_back( new mis_r_parent(line));
            sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
        }
        if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
            sonNodes.push_back(res);
        }else {

            ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
//             cout<<mis_semicolon(line).what()<<endl;
            exceptions.push_back( new mis_semicolon(line));
            sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
        }
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    if(thisType==INTCON||thisType==SEMICN)
    {
        if(Exp::analyzing(&res,tmp)==0)
        {
            sonNodes.push_back(res);
        }
        if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
            sonNodes.push_back(res);
        }else {
            node *tmp= sonNodes[sonNodes.size()-1];
            while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
            int line=((tokenNode*)tmp)->line;
           // cout<<mis_semicolon(line).what()<<endl;
            exceptions.push_back( new mis_semicolon(line));
            sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
        };
        i=tmp;
        *ret=new Stmt(sonNodes);
        cout<<"Stmt success"<<endl;
        return 0;
    }
    else
    {
        auto tmp1=tmp;
        if(LVal::analyzing(&res,tmp)==0)
        {

            sonNodes.push_back(res);
            if(tokenNode::analyzing(&res,tmp,ASSIGN)==0)
            {
                sonNodes.push_back(res);//如果是lval，如果后面是等于
                if(tokenNode::analyzing(&res,tmp,GETINTTK)==0)
                {
                    ///getint语句
                    sonNodes.push_back(res);
                    if(tokenNode::analyzing(&res,tmp,LPARENT)==0){
                        sonNodes.push_back(res);
                    }else return -1;
                    if(tokenNode::analyzing(&res,tmp,RPARENT)==0){
                        sonNodes.push_back(res);
                    }else {

                        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
                        node *tmp= sonNodes[sonNodes.size()-1];
                        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
                        int line=((tokenNode*)tmp)->line;

                        exceptions.push_back( new mis_r_parent(line));
                        sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
                    }
                    if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
                        sonNodes.push_back(res);
                    }else {

                        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
//                        return -1;
                        node *tmp= sonNodes[sonNodes.size()-1];
                        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
                        int line=((tokenNode*)tmp)->line;
                        // cout<<mis_semicolon(line).what()<<endl;
                        exceptions.push_back( new mis_semicolon(line));
                        sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
                    }
                    i=tmp;
                    *ret=new Stmt(sonNodes);
                    cout<<"Stmt success"<<endl;
                    return 0;
                }
                else
                {
                    ///赋值语句
                    if(Exp::analyzing(&res,tmp)==0){
                        sonNodes.push_back(res);
                    }else return -1;
                    if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
                        sonNodes.push_back(res);
                    }else {

                        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
                        node *tmp= sonNodes[sonNodes.size()-1];
                        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
                        int line=((tokenNode*)tmp)->line;
                   //      cout<<mis_semicolon(line).what()<<endl;
                        exceptions.push_back( new mis_semicolon(line));
                        sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
                    }
                    i=tmp;
                    *ret=new Stmt(sonNodes);
                    cout<<"Stmt success"<<endl;
                    return 0;
                }

            }
            else{//如果val后面不是等于，就必须考虑exp
                sonNodes.pop_back();
                tmp=tmp1;
            }
        }//如果不是lval，或者是lval但是后面不紧接着等号，就对应exp
        {

            if(Exp::analyzing(&res,tmp)==0)
            {
                sonNodes.push_back(res);

            }
            if(tokenNode::analyzing(&res,tmp,SEMICN)==0){
                sonNodes.push_back(res);
            }else {

                ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
//error
if(sonNodes.empty())
{
    return -1;
}
                node *tmp2= sonNodes[sonNodes.size()-1];
                while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
                int line=((tokenNode*)tmp2)->line;
                // cout<<mis_semicolon(line).what()<<endl;
                exceptions.push_back( new mis_semicolon(line));
                sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
            }
            i=tmp;
            *ret=new Stmt(sonNodes);
            cout<<"Stmt success"<<endl;
            return 0;
        }


    }

    return-1;
}
int FuncType::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing FuncType "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(tokenNode::analyzing(&res,tmp,VOIDTK)==0)
    {
        sonNodes.push_back(res);
    }else if(tokenNode::analyzing(&res,tmp,INTTK)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    i=tmp;
    *ret=new FuncType (sonNodes);
    cout<<"FuncType  success"<<endl;
    return 0;
}
int FuncFParam::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing FuncFParam "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,INTTK)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    auto tmp1=tmp;
    vector<node*> sonNodes1;
    sonNodes1=sonNodes;
    //这之后就不能再返回-1了
    if(tokenNode::analyzing(&res,tmp1,LBRACK)==0)
        sonNodes1.push_back(res);
    else {
        goto suc1;
    }
    if(tokenNode::analyzing(&res,tmp1,RBRACK)==0)
        sonNodes1.push_back(res);
    else {
        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
        node *tmp2= sonNodes[sonNodes.size()-1];
        while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
        int line=((tokenNode*)tmp2)->line;
      //  cout<<mis_r_bracket(line).what()<<endl;
        exceptions.push_back( new mis_r_bracket(line));
        sonNodes.push_back(new tokenNode(new token("]",RBRACK,line),{}));
    }
    while(tokenNode::analyzing(&res,tmp1,LBRACK)==0)
    {
        auto tmp2=tmp1;
        sonNodes1.push_back(res);
        if(ConstExp::analyzing(&res,tmp1)==0)
            sonNodes1.push_back(res);
        else {
            tmp1--;
            sonNodes1.pop_back();
            goto suc2;
        }
        if(tokenNode::analyzing(&res,tmp1,RBRACK)==0)
            sonNodes1.push_back(res);
        else {
//            tmp1=tmp2;
//            tmp1--;
//            sonNodes1.pop_back();
//            sonNodes1.pop_back();
//            goto suc2;
            node *tmp2= sonNodes[sonNodes.size()-1];
            while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
            int line=((tokenNode*)tmp2)->line;
       //     cout<<mis_r_bracket(line).what()<<endl;
            exceptions.push_back( new mis_r_bracket(line));
            sonNodes.push_back(new tokenNode(new token("]",RBRACK,line),{}));
        }

    }
    goto suc2;

    suc1:
    i=tmp;
    *ret=new FuncFParam (sonNodes);
    cout<<"FuncFParam  success"<<endl;
    return 0;
    suc2:
    i=tmp1;
    *ret=new FuncFParam (sonNodes1);
    cout<<"FuncFParam  success"<<endl;
    return 0;
}
int FuncFParams::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing FuncFParams"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(FuncFParam::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;

//        vector<node*> sonNodes1;
//        sonNodes1=sonNodes;
    while(tokenNode::analyzing(&res,tmp,COMMA)==0){
        sonNodes.push_back(res);
        auto tmp1=tmp;
        if(FuncFParam::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }else{
            sonNodes.pop_back();
            tmp=tmp1;
            tmp--;
            break;
        }
    }
    i=tmp;
    *ret=new FuncFParams(sonNodes);
    cout<<"FuncFParams success"<<endl;
    return 0;
}
int FuncDef::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing FuncDef"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(FuncType::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,LPARENT)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(FuncFParams::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }
    if(tokenNode::analyzing(&res,tmp,RPARENT)==0)
    {
        sonNodes.push_back(res);
    }else{

        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
        node *tmp= sonNodes[sonNodes.size()-1];
        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
        int line=((tokenNode*)tmp)->line;

        exceptions.push_back( new mis_r_parent(line));
        sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
    }
    if(Block::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    i=tmp;
    *ret=new FuncDef(sonNodes);
    cout<<"FuncDef success"<<endl;
    return 0;
}
int InitVal::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing InitVal "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(tokenNode::analyzing(&res,tmp,LBRACE)==0)
    {
        sonNodes.push_back(res);
        if(InitVal::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
            while(tokenNode::analyzing(&res,tmp,COMMA)==0){
                sonNodes.push_back(res);
                if(InitVal::analyzing(&res,tmp)==0){
                    sonNodes.push_back(res);}
                else return -1;
            }
        }
        if(tokenNode::analyzing(&res,tmp,RBRACE)==0) {
            sonNodes.push_back(res);
        }
        else return -1;
    }else
    {
        if(Exp::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else return -1;
    }


    i=tmp;
    *ret=new InitVal (sonNodes);
    cout<<"InitVal  success"<<endl;
    return 0;
}
int VarDef::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing VarDef"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
        sonNodes.push_back(res);
    else return -1;
    while(tokenNode::analyzing(&res,tmp,LBRACK)==0){
        auto tmp1=tmp;
        sonNodes.push_back(res);//如果这轮失败了可以回滚
        if( ConstExp::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else
        {
            tmp--;
            sonNodes.pop_back();
            break;
        }

        if(tokenNode::analyzing(&res,tmp,RBRACK)==0)
            sonNodes.push_back(res);
        else
        {
//            tmp=tmp1;
//            tmp--;
//            sonNodes.pop_back();
//            sonNodes.pop_back();
//            break;
            node *tmp2= sonNodes[sonNodes.size()-1];
            while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
            int line=((tokenNode*)tmp2)->line;
          //  cout<<mis_r_bracket(line).what()<<endl;
            exceptions.push_back( new mis_r_bracket(line));
            sonNodes.push_back(new tokenNode(new token("]",RBRACK,line),{}));
        }
    }//已经满足短的条件了，备份一下迭代器，看看剩下的能否完全满足'=' InitVal
    auto tmp1=tmp;
    if(tokenNode::analyzing(&res,tmp1,ASSIGN)==0)
    {
        sonNodes.push_back(res);
        if( InitVal::analyzing(&res,tmp1)==0)
        {
            sonNodes.push_back(res);
            //到了这里就满足长的条件了，也唯一return0
            i=tmp1;
            *ret=new  VarDef (sonNodes);
            cout<<" VarDef success"<<endl;
            return 0;
        }
        else if(tokenNode::analyzing(&res,tmp1,GETINTTK)==0)
        {
            sonNodes.push_back(res);
            if(tokenNode::analyzing(&res,tmp1,LPARENT)==0){
                sonNodes.push_back(res);
            }else return -1;
            if(tokenNode::analyzing(&res,tmp1,RPARENT)==0){
                sonNodes.push_back(res);
            }else {

                ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
                node *tmp= sonNodes[sonNodes.size()-1];
                while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
                int line=((tokenNode*)tmp)->line;

                exceptions.push_back( new mis_r_parent(line));
                sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
            }
            i=tmp1;
            *ret=new VarDef(sonNodes);
            cout<<"VarDef success"<<endl;
            return 0;

        }
        else
        {
            sonNodes.pop_back();
        }
    }
    i=tmp;
    *ret=new VarDef(sonNodes);
    cout<<"VarDef success"<<endl;
    return 0;

}
int ConstInitVal::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing ConstInitVal "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(tokenNode::analyzing(&res,tmp,LBRACE)==0)
    {
        sonNodes.push_back(res);
        if(ConstInitVal::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
            while(tokenNode::analyzing(&res,tmp,COMMA)==0){
                sonNodes.push_back(res);
                if(ConstInitVal::analyzing(&res,tmp)==0){
                    sonNodes.push_back(res);}
                else {
                    tmp--;
                    sonNodes.pop_back();
                    break;
                }
            }
        }
        if(tokenNode::analyzing(&res,tmp,RBRACE)==0) {
            sonNodes.push_back(res);
        }
        else return -1;
    }else
    {
        if(ConstExp::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else return -1;
    }


    i=tmp;
    *ret=new ConstInitVal (sonNodes);
    cout<<"ConstInitVal  success"<<endl;
    return 0;
}
int ConstDef::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing ConstDef"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
        sonNodes.push_back(res);
    else return -1;
    while(tokenNode::analyzing(&res,tmp,LBRACK)==0){
        sonNodes.push_back(res);
        auto tmp1=tmp;
        if( ConstExp::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else {
            sonNodes.pop_back();
            tmp--;
            break;
        }

        if( tokenNode::analyzing(&res,tmp,RBRACK)==0)
            sonNodes.push_back(res);
        else {
//            sonNodes.pop_back();
//            sonNodes.pop_back();
//            tmp=tmp1;
//            tmp--;
//            break;
            node *tmp2= sonNodes[sonNodes.size()-1];
            while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
            int line=((tokenNode*)tmp2)->line;
          //  cout<<mis_r_bracket(line).what()<<endl;
            exceptions.push_back( new mis_r_bracket(line));
            sonNodes.push_back(new tokenNode(new token("]",RBRACK,line),{}));
        }
    }
    if(tokenNode::analyzing(&res,tmp,ASSIGN)==0)
        sonNodes.push_back(res);
    else return -1;
    if(ConstInitVal::analyzing(&res,tmp)==0)
        sonNodes.push_back(res);
    else return -1;
    i=tmp;
    *ret=new ConstDef(sonNodes);
    cout<<"ConstDef success"<<endl;
    return 0;
}
int VarDecl::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing VarDecl"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,INTTK)==0)
        sonNodes.push_back(res);
    else {
//        cout<<"return from vardecl"<<endl;
        return -1;
    }
    if( VarDef::analyzing(&res,tmp)==0)
        sonNodes.push_back(res);
    else return -1;
    while(tokenNode::analyzing(&res,tmp,COMMA)==0){
        sonNodes.push_back(res);
        if( VarDef::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else {
            sonNodes.pop_back();
            tmp--;
            break;
        }
    }
    if(tokenNode::analyzing(&res,tmp,SEMICN)==0)
        sonNodes.push_back(res);
    else {
        if(tokenNode::analyzing(&res,tmp,LPARENT)==0)
        {
            return -1;
        }
        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
        node *tmp= sonNodes[sonNodes.size()-1];
        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
        int line=((tokenNode*)tmp)->line;
       // cout<<mis_semicolon(line).what()<<endl;
        exceptions.push_back( new mis_semicolon(line));
        sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));

    }
    i=tmp;
    *ret=new VarDecl(sonNodes);
    cout<<"VarDecl success"<<endl;
    return 0;
}
int ConstDecl::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing ConstDecl"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,CONSTTK)==0)
        sonNodes.push_back(res);
    else return -1;
    if(tokenNode::analyzing(&res,tmp,INTTK)==0)
        sonNodes.push_back(res);
    else return -1;
    if(ConstDef::analyzing(&res,tmp)==0)
        sonNodes.push_back(res);
    else return -1;
    while(tokenNode::analyzing(&res,tmp,COMMA)==0){
        sonNodes.push_back(res);
        if(ConstDef::analyzing(&res,tmp)==0)
            sonNodes.push_back(res);
        else {
            tmp--;
            sonNodes.pop_back();
            break;
        }
    }
    if(tokenNode::analyzing(&res,tmp,SEMICN)==0)
        sonNodes.push_back(res);
    else {
        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
        node *tmp= sonNodes[sonNodes.size()-1];
        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
        int line=((tokenNode*)tmp)->line;
      //  cout<<mis_semicolon(line).what()<<endl;
        exceptions.push_back( new mis_semicolon(line));
        sonNodes.push_back(new tokenNode(new token(";",SEMICN,line),{}));
    }


    i=tmp;
    *ret=new ConstDecl(sonNodes);
    cout<<"ConstDecl success"<<endl;
    return 0;
}
int Decl::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing decl"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(ConstDecl::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else if(VarDecl::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;

    i=tmp;
    *ret=new Decl(sonNodes);
    cout<<"decl success"<<endl;
    return 0;
}
int MainFuncDef::analyzing(node** ret,vector<token*>::iterator &i){
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,INTTK)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,MAINTK)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,LPARENT)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,RPARENT)==0) {
        sonNodes.push_back(res);
    }else {
        ///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
        node *tmp= sonNodes[sonNodes.size()-1];
        while(tmp->sonNodes.size()!=0)tmp=tmp->sonNodes[tmp->sonNodes.size()-1];
        int line=((tokenNode*)tmp)->line;
        exceptions.push_back( new mis_r_parent(line));
        sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
    }
    if(Block::analyzing(&res,tmp)==0) {
        sonNodes.push_back(res);
    }else return -1;

    i=tmp;
    *ret=new MainFuncDef(sonNodes);
    cout<<"main success"<<endl;
    return 0;

}
int CompUnit::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing compunit"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    while(Decl::analyzing(&res,tmp)==0){
        sonNodes.push_back(res);
    }
    while(FuncDef::analyzing(&res,tmp)==0) {
        sonNodes.push_back(res);
    }
    if(MainFuncDef::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new CompUnit(sonNodes);
        cout<<"compunit success"<<endl;
        return 0;
    }
    return -1;
}
int Exp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing Exp"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(AddExp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new Exp(sonNodes);
        cout<<"Exp success"<<endl;
        return 0;
    }
    return -1;
}
int Cond::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing Cond"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(LOrExp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new Cond(sonNodes);
        cout<<"Cond success"<<endl;
        return 0;
    }
    return -1;
}
int LVal::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing LVal"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,LBRACK)==0)
        {
            auto tmp1=tmp;
            sonNodes.push_back(res);
            if(Exp::analyzing(&res,tmp)==0)
            {
                sonNodes.push_back(res);

            }else
            {
                sonNodes.pop_back();
                tmp--;
                break;
            }
            if(tokenNode::analyzing(&res,tmp,RBRACK)==0){
                sonNodes.push_back(res);
            }else
            {
//                sonNodes.pop_back();
//                sonNodes.pop_back();
//                tmp=tmp1;
//                tmp--;
//                break;
                node *tmp2= sonNodes[sonNodes.size()-1];
                while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
                int line=((tokenNode*)tmp2)->line;
               // cout<<mis_r_bracket(line).what()<<endl;
                exceptions.push_back( new mis_r_bracket(line));
                sonNodes.push_back(new tokenNode(new token("]",RBRACK,line),{}));
            }

        }
        i=tmp;
        *ret=new LVal(sonNodes);
        cout<<"LVal success"<<endl;
        return 0;
    }else return -1;


}
int IntConst::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing IntConst"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,INTCON)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new IntConst(sonNodes);
        cout<<"IntConst success"<<endl;
        return 0;
    }
    return -1;
}
int Number::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing Number"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(IntConst::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new Number(sonNodes);
        cout<<"Number success"<<endl;
        return 0;
    }
    return -1;
}
int PrimaryExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing PrimaryExp"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(Number::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        i=tmp;
        *ret=new PrimaryExp(sonNodes);
        cout<<"PrimaryExp success"<<endl;
        return 0;
    }else if(LVal::analyzing(&res,tmp)==0){
        sonNodes.push_back(res);
        i=tmp;
        *ret=new PrimaryExp(sonNodes);
        cout<<"PrimaryExp success"<<endl;
        return 0;
    }
    if(tokenNode::analyzing(&res,tmp,LPARENT)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(Exp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    if(tokenNode::analyzing(&res,tmp,RPARENT)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    i=tmp;
    *ret=new PrimaryExp(sonNodes);
    cout<<"PrimaryExp success"<<endl;
    return 0;

}
int FuncRParams::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing FuncRParams"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(Exp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;
    while(tokenNode::analyzing(&res,tmp,COMMA)==0){
        sonNodes.push_back(res);
        if(Exp::analyzing(&res,tmp)==0){
            sonNodes.push_back(res);
        }else{
            sonNodes.pop_back();
            tmp--;
            break;
        }
    }
    i=tmp;
    *ret=new FuncRParams(sonNodes);
    cout<<"FuncRParams success"<<endl;
    return 0;

}
int ConstExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing ConstExp"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(AddExp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
    }else return -1;

    i=tmp;
    *ret=new ConstExp(sonNodes);
    cout<<"ConstExp success"<<endl;
    return 0;

}
int UnaryOp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing UnaryOp"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    if(tokenNode::analyzing(&res,tmp,PLUS)==0){
        sonNodes.push_back(res);
    }else if(tokenNode::analyzing(&res,tmp,MINU)==0){
        sonNodes.push_back(res);
    }else if(tokenNode::analyzing(&res,tmp,NOT)==0){
        sonNodes.push_back(res);
    }else return -1;
    i=tmp;
    *ret=new UnaryOp(sonNodes);
    cout<<"UnaryOp success"<<endl;
    return 0;

}
int UnaryExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing UnaryExp"<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;
    auto tmp1=tmp;
    if(tokenNode::analyzing(&res,tmp,IDENFR)==0)
    {
        sonNodes.push_back(res);
        if(tokenNode::analyzing(&res,tmp,LPARENT)==0){
            sonNodes.push_back(res);//开始分析函数调用
            if(FuncRParams::analyzing(&res,tmp)==0)
            {
                sonNodes.push_back(res);
            }
            if(tokenNode::analyzing(&res,tmp,RPARENT)==0)
            {
                sonNodes.push_back(res);
                i=tmp;
                *ret=new UnaryExp(sonNodes);
                cout<<"UnaryExp success"<<endl;
                return 0;
            }
            else
            {
//                tmp=tmp1;
//                sonNodes.clear();
//                goto out;
///找到上一个非终结符的行号，一直找sonnodes[-1]直到最后的行号
                node *tmp2= sonNodes[sonNodes.size()-1];
                while(!tmp2->sonNodes.empty())tmp2=tmp2->sonNodes[tmp2->sonNodes.size()-1];
                int line=((tokenNode*)tmp2)->line;

                exceptions.push_back( new mis_r_parent(line));
                sonNodes.push_back(new tokenNode(new token(")",RPARENT,line),{}));
                i=tmp;
                *ret=new UnaryExp(sonNodes);
                cout<<"UnaryExp success"<<endl;
                return 0;
            }
        }else
        {
            sonNodes.clear();
            tmp=tmp1;
        }
    }
    out:
    if(PrimaryExp::analyzing(&res,tmp)==0){
        sonNodes.push_back(res);
    }else  {
        if(UnaryOp::analyzing(&res,tmp)==0)
        {
            sonNodes.push_back(res);
        }else return -1;
        if(UnaryExp::analyzing(&res,tmp)==0)
        {
            sonNodes.push_back(res);
        }else return -1;
    }
    i=tmp;
    *ret=new UnaryExp(sonNodes);
    cout<<"UnaryExp success"<<endl;
    return 0;

}
int MulExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing MulExp "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(UnaryExp::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,MULT)==0
        ||tokenNode::analyzing(&res,tmp,DIV)==0
          ||tokenNode::analyzing(&res,tmp,BITANDTK)==0
        ||tokenNode::analyzing(&res,tmp,MOD)==0){
            sonNodes.push_back(res);
            if(UnaryExp::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=MulExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new MulExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
//        if(sonNodes[0]->nodeTypeEnum()!=MulExp_)
//        {
//            vector<node*>sonSonNodes={};
//            sonSonNodes.push_back(sonNodes[0]);
//            node* imSb=new MulExp (sonSonNodes);
//            sonNodes.erase(sonNodes.begin());
//            sonNodes.insert(sonNodes.begin(),imSb);
//        }
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new MulExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new MulExp (sonNodes);
    cout<<"MulExp  success"<<endl;
    return 0;

}
int AddExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing AddExp "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(MulExp::analyzing(&res,tmp)==0)
    {
//        vector<node*>sonNodes1={};
//        sonNodes1.push_back(res);
//        sonNodes.push_back(new AddExp(sonNodes1));
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,PLUS)==0||tokenNode::analyzing(&res,tmp,MINU)==0){
            sonNodes.push_back(res);
            if(MulExp::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=AddExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new AddExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new AddExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new AddExp (sonNodes);
    cout<<"AddExp  success"<<endl;
    return 0;

}
int RelExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing RelExp  "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(AddExp ::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,LSS)==0
        ||tokenNode::analyzing(&res,tmp,GRE)==0
          ||tokenNode::analyzing(&res,tmp,GEQ)==0
            ||tokenNode::analyzing(&res,tmp,LEQ)==0){
            sonNodes.push_back(res);
            if(AddExp ::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=RelExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new RelExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new RelExp  (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new RelExp  (sonNodes);
    cout<<"RelExp   success"<<endl;
    return 0;

}
int EqExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing EqExp  "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(RelExp ::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,EQL)==0
        ||tokenNode::analyzing(&res,tmp,NEQ)==0){
            sonNodes.push_back(res);
            if(RelExp ::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=EqExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new EqExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new  EqExp  (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new EqExp  (sonNodes);
    cout<<"EqExp   success"<<endl;
    return 0;

}
int LAndExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing LAndExp  "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(EqExp ::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,AND)==0){
            sonNodes.push_back(res);
            if(EqExp ::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=LAndExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new LAndExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new  LAndExp  (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new LAndExp  (sonNodes);
    cout<<"LAndExp   success"<<endl;
    return 0;

}
int LOrExp::analyzing(node** ret,vector<token*>::iterator &i){
    cout<<"start analysing LOrExp  "<<endl;
    cout<<"current token is "<<(*i)->str<<endl;
    auto tmp=i;
    vector<node*>sonNodes={};
    node * res;

    if(LAndExp ::analyzing(&res,tmp)==0)
    {
        sonNodes.push_back(res);
        while(tokenNode::analyzing(&res,tmp,OR)==0){
            sonNodes.push_back(res);
            if(LAndExp ::analyzing(&res,tmp)==0){
                sonNodes.push_back(res);
            }else{
                sonNodes.pop_back();
                tmp--;
                break;
            }
        }
    }else return -1;

    i=tmp;
    ///开始打包
    if(sonNodes.size()>1&&sonNodes[0]->nodeTypeEnum()!=LOrExp_)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        node* imSb=new LOrExp (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    while(sonNodes.size()>3)
    {
        vector<node*>sonSonNodes={};
        sonSonNodes.push_back(sonNodes[0]);
        sonSonNodes.push_back(sonNodes[1]);
        sonSonNodes.push_back(sonNodes[2]);
        node* imSb=new  LOrExp  (sonSonNodes);
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.erase(sonNodes.begin());
        sonNodes.insert(sonNodes.begin(),imSb);
    }
    *ret=new LOrExp  (sonNodes);
    cout<<"LOrExp   success"<<endl;
    return 0;

}
void lastOrder(node * a,std::ofstream &outputStream){//引用还有个讲究来着
    if(a != nullptr){
        vector<node*>::iterator i;
        for(i = a->sonNodes.begin(); i < a->sonNodes.end (); ++i)
        lastOrder(*i,outputStream);
        a->output(outputStream);
    }
}
node * parsing()
{
    auto begin=token::tokens.begin();
    node *a= nullptr;
    CompUnit::analyzing(&a,begin);
    cout<<"finished parsing"<<endl;
    return a;
}
