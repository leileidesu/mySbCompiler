//
// Created by Administrator on 2022-10-25.
//
#include"IRVisitor.h"

#include"parsing.h"
#include"symbolTable.h"
#include"instruction.h"
#include <map>
#include<stack>
extern node *ASTRoot;
extern symbolTable *rootTable;
Module *IRRoot;
basicBlock *curBlock;
int Value::num = 0;
symbolTable *cur;
vector<Cond *> circu;
map<string, int> mymap;
int imsbbbb=0;// 1 i32 2 i32* 3 [  x i32 ]*
int visitConstExp(node *a) {
    synType type = a->nodeTypeEnum();
    auto sons = a->sonNodes;
    if (type == AddExp_) {
        if (sons.size() == 1) {
            cout<<"im add 1"<<endl;
            int ans=visitConstExp(sons[0]);
            cout<<ans<<endl;
            return ans;
        }
        int a1 = visitConstExp(sons[0]);
        int a2 = visitConstExp(sons[2]);
        if (((tokenNode *) sons[1])->type == PLUS)
            return a1 + a2;
        return a1 - a2;
    }
    if (type == MulExp_) {
        if (sons.size() == 1) return visitConstExp(sons[0]);
        int a1 = visitConstExp(sons[0]);
        int a2 = visitConstExp(sons[2]);
        cout<<"of mul: "<<a1<<" "<<a2<<endl;
        if (((tokenNode *) sons[1])->type == MULT)
            return a1 * a2;
        else if (((tokenNode *) sons[1])->type == DIV)
            return a1 / a2;
        else if (((tokenNode *) sons[1])->type == BITANDTK)
            return a1 & a2;
        return a1 % a2;
    }
    if (type == UnaryExp_) {
        if (sons.size() == 1)
        {
            int tmp=visitConstExp(sons[0]);
            cout<<"im unaru here"<<endl;
            return tmp;
        }
        if (((tokenNode *) (sons[0]->sonNodes[0]))->type == PLUS)
            return visitConstExp(sons[1]);
        return -1 * visitConstExp(sons[1]);
    }
    if (type == PrimaryExp_) {
        cout << "fially is a PrimaryExp_" << endl;
        if (sons.size() == 1 && sons[0]->nodeTypeEnum() == Number_) {
            cout << "fially is a num" << endl;
            node *number = sons[0];
            string num = ((tokenNode *) (number->sonNodes[0]->sonNodes[0]))->str;
            cout<<stoi(num)<<endl;;
            return stoi(num);
        }
        if (sons.size() == 1 && sons[0]->nodeTypeEnum() == LVal_) {
            node *lval = sons[0];
            auto sonsons=sons[0]->sonNodes;
            string name = ((tokenNode *) lval->sonNodes[0])->str;
            varSymbol *now = nullptr;
            symbolTable *nowtable = cur;
            while (now == nullptr) {
                cout << "one layer" << endl;
                now = nowtable->varSymbols[name];
                if (now == nullptr)
                    nowtable = nowtable->parent;
                else if (now->defined == 0) {
                    cout << "not defined" << endl;
                    now = nullptr;
                    nowtable = nowtable->parent;
                }
            }
            cout<<"now print value"<<endl;
            for(auto kk:now->values)
            {
                cout<<kk<<endl;
            }
            if(sonsons.size()==1)
            {
                return now->values[0];
            }else if(sonsons.size()==4)
            {
                return now->values[visitConstExp(sonsons[2]->sonNodes[0])];
            }
            else {
                int l1 = visitConstExp(sonsons[2]->sonNodes[0]);
                int l2 = visitConstExp(sonsons[5]->sonNodes[0]);
                cout<<"out "<<l1<<" "<<l2<<endl;
                return now->values[l1 * now->dims[1]+l2];
            }

        }
        return visitConstExp(sons[1]->sonNodes[0]);
    }
    return 0;
}

void outputModule(string a) {
    std::ofstream output(a);

        for (auto i: IRRoot->globalList) {
            //@a = global i32 5
            if(i->type==I32)
                output << "@" + i->getName() + " = global " << i->typestr() << " "<< i->initstr() << endl;
            else
            output << "@" + i->getName() + " = global " << i->typestr() << " "<< i->initstr() << endl;
        }
        for (auto i: IRRoot->funcList) {
            output << (i->isDecl == 1 ? "declare " : "define ") << (i->retType == 1 ? "i32 @" : "void @")
                   << i->getName()
                   << "(";
            int len = i->funcs->params.size();
            for (int j = 0; j < len; j++) {
                if(i->funcs->params[j].dim==0)
                output << " i32";
                else if(i->funcs->params[j].dim==1)
                    output << " i32*";
                else if(i->funcs->params[j].dim==2)
                    output << " ["<<i->funcs->params[j].last<<" x i32]*";
                if (j < len - 1)
                    output << ",";
            }
            output << ")" << endl;
            if (i->isDecl == 0) {
                string startnumber = i->BasicBlockList[0]->getName();
                //; <label>:
                output << "#" + startnumber << endl;
                output << "{" << endl;
                for (auto ii: i->BasicBlockList) {
                    if (ii->getName() != startnumber)
                        output << "\n; <label>:" + ii->getName() + ":" << endl;
                    for (auto iii: ii->Ins) {
                        cout<<iii->label<<endl;
                        output << iii->output() << endl;
                        if (iii->label == "brInstruction" && iii->operandList.empty())
                            break;
                    }
                }
                output << "}" << endl;
            }


        }

}

Value *calExp(node *a) {
    synType type = a->nodeTypeEnum();
    if (type == Exp_) {
        cout << "exp" << endl;
        return calExp(a->sonNodes[0]);
    }
    if (type == AddExp_) {
        cout << "addexp" << endl;
        if (a->sonNodes.size() == 1) {

            Value *ret = calExp(a->sonNodes[0]);//可以是ins，也可以是constantint
            if (ret->isConst == 1) {
                return ret;
            }
//            curBlock->addIns((Instruction*)ret);
            return ret;
        } else {
            binaryInstruction *a1 = (binaryInstruction *) calExp(a->sonNodes[0]);
            binaryInstruction *a2 = (binaryInstruction *) calExp(a->sonNodes[2]);
            binaryInstruction *ret;
            if (((tokenNode *) a->sonNodes[1])->type == PLUS) {
                ret = new binaryInstruction(ADD, a1, a2, I32);
            } else {
                ret = new binaryInstruction(SUB, a1, a2, I32);
            }
            curBlock->addIns(ret);
            return ret;
        }
    }
    if (type == MulExp_) {
        cout << "mul" << endl;
        if (a->sonNodes.size() == 1) {
            Value *ret = calExp(a->sonNodes[0]);//可以是ins，也可以是constantint
            if (ret->type == I1) {
                //%Y = zext i1 true to i32              ; yields i32:1
//                a1=new constantInt(1);
                auto zext = new zextInstruction(ret);
                curBlock->addIns(zext);
                return zext;
            }
//            if (ret->isConst == 1) {
//                return ret;
//            }
            //   curBlock->addIns((Instruction*)ret);
            return ret;
        } else {
                Instruction *a1 = (Instruction *) calExp(a->sonNodes[0]);
            if (a1->type == I1) {
                //%Y = zext i1 true to i32              ; yields i32:1
//                a1=new constantInt(1);
                auto zext = new zextInstruction(a1);
                a1 = zext;
                curBlock->addIns(zext);
            }
            Instruction *a2 = (Instruction *) calExp(a->sonNodes[2]);
            if (a2->type == I1) {
                //%Y = zext i1 true to i32              ; yields i32:1
//                a1=new constantInt(1);
                auto zext = new zextInstruction(a2);
                a2 = zext;
                curBlock->addIns(zext);
            }
            binaryInstruction *ret;
            if (((tokenNode *) a->sonNodes[1])->type == MULT) {
                ret = new binaryInstruction(MUL, a1, a2, I32);
            } else if (((tokenNode *) a->sonNodes[1])->type == BITANDTK) {
                ret = new binaryInstruction(AND_, a1, a2, I32);
            }
            else if (((tokenNode *) a->sonNodes[1])->type == DIV)
            {
                ret = new binaryInstruction(SDIV, a1, a2, I32);
            } else {
                ret = new binaryInstruction(SREM, a1, a2, I32);
            }
            curBlock->addIns(ret);
            return ret;
        }

    }
    if (type == UnaryExp_) {
        cout << "unary" << endl;
        if (a->sonNodes.size() == 2&&a->sonNodes[0]->nodeTypeEnum()==UnaryOp_){
            UnaryOp *sym = (UnaryOp *) a->sonNodes[0];
            constantInt *a1 = new constantInt(0);
            binaryInstruction *a2 = (binaryInstruction *) calExp(a->sonNodes[1]);
            Instruction *ret;
            if (((tokenNode *) sym->sonNodes[0])->type == PLUS) {
                ret = new binaryInstruction(ADD, a1, a2, I32);
            } else if (((tokenNode *) sym->sonNodes[0])->type == MINU) {
                ret = new binaryInstruction(SUB, a1, a2, I32);
            }
                //%10 = icmp ne i32 %9, 0
            else {
                ret = new icmpInstruction(a2, a1, EQ_);
            }
            curBlock->addIns(ret);
            return ret;
        } else if (a->sonNodes.size() == 1) {
            Value *ret = calExp(a->sonNodes[0]);//可以是ins，也可以是constantint
            if (ret->isConst == 1) {
                return ret;
            }
            //curBlock->addIns((Instruction*)ret);
            return ret;
        } else {
            vector<node *> sons = a->sonNodes;

            string name = ((tokenNode *) sons[0])->str;

            Function *fun;
            for (auto i: IRRoot->funcList)
                if (i->funcName == name)
                    fun = i;

            if (sons.size() == 3)//no params
            {
                auto ci = new callInstruction(fun, {});
                curBlock->addIns(ci);

                return ci;
            } else {

                cout << fun->funcName << endl;
                struct param p = {};
                auto funcparams = sons[2]->sonNodes;
                int len = funcparams.size();
                vector<Value *> wuwu = {};
                for (int i = 0; i < len; i += 2) {
                    if(fun->funcs->params[i/2].dim!=0)
                        imsbbbb=1;
                    imsbbbb=fun->funcs->params[i/2].dim;
                    Value *ans = calExp(funcparams[i]);//可能是loadins addins 可能是constantint
                    wuwu.push_back(ans);
                    imsbbbb=0;
                }
                auto call = new callInstruction(fun, wuwu);
                curBlock->addIns(call);

                return call;

            }
        }
    }
    if (type == PrimaryExp_) {
        cout << "pri" << endl;
        if (a->sonNodes.size() > 1)//(EXP)
        {
            Value *ret = calExp(a->sonNodes[1]);//可以是ins，也可以是constantint
            if (ret->isConst == 1) {
                return ret;
            }
            //curBlock->addIns((Instruction*)ret);
            return ret;
        }
        if (a->sonNodes[0]->nodeTypeEnum() == Number_) {
            IntConst *num1 = (IntConst *) (a->sonNodes[0]->sonNodes[0]);
//            cout<<((tokenNode*)num1->sonNodes[0])->str<<endl;
            int num2 = stoi(((tokenNode *) num1->sonNodes[0])->str);
            return new constantInt(num2);
        } else {
            node *lval = a->sonNodes[0];
            auto sons = lval->sonNodes;
            string name = ((tokenNode *) lval->sonNodes[0])->str;
            cout << name << endl;
            varSymbol *now = nullptr;
            symbolTable *nowtable = cur;
            while (now == nullptr) {
                cout << "one layer" << endl;
                now = nowtable->varSymbols[name];
                if (now == nullptr)
                    nowtable = nowtable->parent;
                else if (now->defined == 0) {
                    cout << "not defined" << endl;
                    now = nullptr;
                    nowtable = nowtable->parent;

                }
            }
            if(now->dim>0)//如果是一维数组或者二维数组
            {
                if(now->addr->numarray==1)//int a[]  int a[][2]
                {
                    localVar *lv = now->addr;
                    int dim = lv->sym->dim;
                    vector<Value *> tmp = {};
                    if(imsbbbb!=0)//这说明我在读取函数参数
                        tmp.push_back(new constantInt(0));
                    for (auto em: sons) {
                        if (em->nodeTypeEnum() == Exp_) {
                            tmp.push_back(calExp(em));
                        }
                    }
                    auto gpt = new getelementptrInstruction(lv, tmp);

                    curBlock->addIns(gpt);
                    if(imsbbbb==0) {
                        pointer *haha = new pointer(lv, gpt);
                        loadInstruction *ls = new loadInstruction(haha);
                        curBlock->addIns(ls);
                        return ls;
                    }
                    return gpt;

                }else//如果不是指针 就是普通的[3x [2 x i32] i32]
                {
                    localVar *lv = now->addr;
                    //知道虚拟寄存器了，要从内存里取出来load
                    //%10 = load i32, i32* %2
                    int dim = lv->sym->dim;
                    loadInstruction *ls;
//                    二维 -> i32  0 x1 x2 +load
//                    二维  -> i32*    0 x 0
//                    二维 -> [ 5 x i32]*    0  x
//
//                    一维 -> i32   0 x   + load
//                    一维 i32*     0 0
                    vector<Value *> tmp = {};
                    if(dim==2)
                    {
                        tmp.push_back(new constantInt(0));
                        for (auto em: sons) {
                            if (em->nodeTypeEnum() == Exp_) {
                                tmp.push_back(calExp(em));
                            }
                        }
                        if(imsbbbb==1)
                        {
                            tmp.push_back(new constantInt(0));
                        }
                        if(imsbbbb==2)
                        {
                            tmp.push_back(new constantInt(0));
                        }
                    }else if(dim==1)
                    {
                        tmp.push_back(new constantInt(0));
                        for (auto em: sons) {
                            if (em->nodeTypeEnum() == Exp_) {
                                tmp.push_back(calExp(em));
                            }
                        }
                        if(imsbbbb==1)
                            tmp.push_back(new constantInt(0));
                    }
                    if(imsbbbb==0)
                    {
                        auto gpt = new getelementptrInstruction(lv, tmp);
                        pointer* haha= new pointer(lv,gpt);
                        ls = new loadInstruction(haha);
                        curBlock->addIns(gpt);
                        curBlock->addIns(ls);
                        return ls;
                    }else
                    {
                        auto gpt = new getelementptrInstruction(lv, tmp);

                        curBlock->addIns(gpt);
                        return gpt;
                    }

                }

            }else
            //如果是0维 这种情况可能有i32和i32* 后者是函数参数int a[]
            {
                cout << "here1" << endl;
                localVar *lv = now->addr;
                //知道虚拟寄存器了，要从内存里取出来load
                //%10 = load i32, i32* %2
                //是指针
                int dim = lv->sym->dim;
                cout<<"check localvar "<<lv->name<<" "<<lv->sym->dim<<" "<<lv->sym->ispointer<<endl;
                loadInstruction *ls;
                if (dim == 0) {//想从一维数组取元素
                    ls = new loadInstruction(lv);
                    curBlock->addIns(ls);
                    return ls;


                } else {
                    vector<Value *> tmp = {};
                    tmp.push_back(new constantInt(0));
                    for (auto em: sons) {
                        if (em->nodeTypeEnum() == Exp_) {
                            tmp.push_back(calExp(em));
                        }

                    }
                    auto gpt = new getelementptrInstruction(lv, tmp);
                    curBlock->addIns(gpt);
                    if(imsbbbb!=1) {
                        pointer *haha = new pointer(lv, gpt);
                        loadInstruction *ls = new loadInstruction(haha);
                        curBlock->addIns(ls);
                        return ls;
                    }
                    return gpt;
                }

            }



        }
    }
    return nullptr;
}

Value *condHandler(node *a) {
    synType types = a->nodeTypeEnum();
    vector<node *> sons = a->sonNodes;
    if (types == EqExp_) {
        if (sons.size() == 1)
            return condHandler(sons[0]);
        else {
            Value *a1 = condHandler(sons[0]);
            Value *a2 = condHandler(sons[2]);
            if (a1->type == I1) {
                //%Y = zext i1 true to i32              ; yields i32:1
//                a1=new constantInt(1);
                auto zext = new zextInstruction(a1);
                a1 = zext;
                curBlock->addIns(zext);
            }
            if (a2->type == I1) {
                auto zext = new zextInstruction(a2);
                a2 = zext;
                curBlock->addIns(zext);
            }
            icmpInstruction *str;
            if (((tokenNode *) sons[1])->type == EQL) {
                str = new icmpInstruction(a1, a2, EQ_);
            } else str = new icmpInstruction(a1, a2, NE_);
            curBlock->addIns(str);
            return str;
        }
    }
    if (types == RelExp_) {
        if (sons.size() == 1)
            return condHandler(sons[0]);
        else {
            Value *a1 = condHandler(sons[0]);
            Value *a2 = condHandler(sons[2]);
            icmpInstruction *str;
            if (((tokenNode *) sons[1])->type == LSS) {
                str = new icmpInstruction(a1, a2, SLT_);
            } else if (((tokenNode *) sons[1])->type == GRE) {
                str = new icmpInstruction(a1, a2, SGT_);
            } else if (((tokenNode *) sons[1])->type == LEQ) {
                str = new icmpInstruction(a1, a2, SLE_);
            } else if (((tokenNode *) sons[1])->type == GEQ) {
                str = new icmpInstruction(a1, a2, SGE_);
            }
            curBlock->addIns(str);
            return str;
        }
    }
    if (types == AddExp_) {
        return calExp(a);

    }
    return nullptr;
}

void sad(node *a, Cond *cond) {
    auto sons = a->sonNodes;
    vector<node *>::iterator i;
    for (i = a->sonNodes.begin(); i < a->sonNodes.end(); ++i) {
        sad(*i, cond);
    }
    synType types = a->nodeTypeEnum();
    if (types == LOrExp_) {
        cout << "meet lor gonna push" << endl;
        cond->group.push_back(0);
    }
    if (types == LAndExp_) {
        cout << "meet land gonna plus" << endl;
        int now = *(cond->group.end() - 1);
        cond->group.pop_back();
        cond->group.push_back(now + 1);
        int len = a->sonNodes.size();
        for (int i = 0; i < len; i++) {
            if (a->sonNodes[i]->nodeTypeEnum() == EqExp_) {
                //原则上想开一个新的label，但是要是这个label还没有东西就用这个
                if (curBlock->Ins.size() == 0) {
                    if (cond->startBlock == -1) {
                        cond->startBlock = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size() - 1;
                    }
                } else {
                    if (cond->startBlock == -1) {
                        cond->startBlock = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size();
                        basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                        IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                        curBlock = newBlock;
                        auto bbs = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList;
                        bbs[bbs.size() - 2]->addIns(new brInstruction(bbs[bbs.size() - 1]));
                    } else {
                        basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                        IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                        curBlock = newBlock;
                    }

                }
                int pre = curBlock->Ins.size();
                Value *ret = condHandler(a->sonNodes[i]);
                int aft = curBlock->Ins.size();
                if (pre == aft || curBlock->Ins[curBlock->Ins.size() - 1]->label != "icmpInstruction") {
                    curBlock->addIns(new icmpInstruction(ret, new constantInt(0), NE_));
                }
            }
        }
    }
}

void handcond(Cond *a) {
    a->group.push_back(0);

    sad(a, a);
    a->group.pop_back();
    cout << "after handcond" << endl;
    cout << a->group.size() << endl;
//    basicBlock* newBlock=new basicBlock(IRRoot->funcList[IRRoot->funcList.size()-1]);
//    IRRoot->funcList[IRRoot->funcList.size()-1]->BasicBlockList.push_back(newBlock);
//    curBlock= newBlock;

}

void visitorIterator(node *a) {
    if (a != nullptr) {
        auto curbackup = cur;
        vector<node *>::iterator i;
        if (a->nodeTypeEnum() == Block_) {
            cur = ((Block *) a)->table;
        }
        a->IRVisit();
        auto sons = a->sonNodes;
        for (i = a->sonNodes.begin(); i < a->sonNodes.end(); ++i) {
            if (a->nodeTypeEnum() == Stmt_ &&
                (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == IFTK)) {
                if (i == a->sonNodes.begin() + 4 || i == a->sonNodes.begin() + 6) {
                    if (i == a->sonNodes.begin() + 4)
                        ((Cond *) a->sonNodes[2])->ifblock = IRRoot->funcList[IRRoot->funcList.size() -
                                                                              1]->BasicBlockList.size();
                    else {
//                        basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
//                        IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
//                        curBlock = newBlock;
                        ((Cond *) a->sonNodes[2])->elseblock = IRRoot->funcList[IRRoot->funcList.size() -
                                                                                1]->BasicBlockList.size();
                    }

                    basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                    curBlock = newBlock;
                }
                visitorIterator(*i);
                if (i == a->sonNodes.end() - 1) {
                    ((Cond *) a->sonNodes[2])->final = IRRoot->funcList[IRRoot->funcList.size() -
                                                                        1]->BasicBlockList.size();
                    basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                    curBlock = newBlock;
                    int start = ((Cond *) a->sonNodes[2])->startBlock;
                    int ifif = ((Cond *) a->sonNodes[2])->ifblock;
                    int elel = ((Cond *) a->sonNodes[2])->elseblock;
                    int fifi = ((Cond *) a->sonNodes[2])->final;
                    //elel-1 if的收束
                    auto datas = ((Cond *) a->sonNodes[2])->group;
                    auto bbs = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList;
                    cout << "in the last if stmt" << endl;
                    //没有else：
                    int bias = 0;
                    for (int jj = 0; jj < datas.size(); jj++)
//                    for (auto j: datas)
                    {
                        auto j = datas[jj];
                        cout << j << endl;
                        for (int k = 0; k < j; k++) {
                            int suc, fail;
                            if (jj == datas.size() - 1)//最后一组or
                            {
//                                cout << "its last group " << endl;
                                if (k < j - 1) {
                                    cout << "1" << endl;
                                    suc = start + bias + 1;
                                    if (elel == -1)
                                        fail = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size() - 1;
                                    else fail = elel;
                                } else {
                                    cout << "2" << endl;
                                    suc = ifif;
                                    if (elel == -1)
                                        fail = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size() - 1;
                                    else fail = elel;
                                }
                            } else {
                                if (k < j - 1)//不是最后i一组or
                                {
                                    cout << "3" << endl;
                                    suc = start + bias + 1;
                                    fail = bias - k + j + start;
                                } else {
                                    cout << "4" << endl;
                                    suc = ifif;
                                    fail = bias - k + j + start;
                                }
                            }
                            cout << "sur 1" << endl;
                            //else stmt刚建立好
                            cout << "now=" << start + bias << "and j==" << j << " and k==" << k << " and suc fail "
                                 << suc << " " << fail << endl;
                            brInstruction *br = new brInstruction(
                                    bbs[start + bias]->Ins[bbs[start + bias]->Ins.size() - 1], bbs[suc], bbs[fail]);
                            bbs[start + bias]->addIns(br);
                            bias++;
                            cout << "sur 2" << endl;


                        }
                    }
                    cout << "sur 2.5" << endl;
                    brInstruction *br = new brInstruction(bbs[fifi]);
                    //if (elel != -1&&bbs[elel - 1]->Ins[bbs[elel - 1]->Ins.size()-1]->label!="brInstruction"&&bbs[elel - 1]->Ins[bbs[elel - 1]->Ins.size()-1]->label!="break")
                    if (elel != -1) {
                        if (bbs[elel - 1]->Ins.empty() ||
                            bbs[elel - 1]->Ins[bbs[elel - 1]->Ins.size() - 1]->label != "retInstruction")
                        bbs[elel - 1]->addIns(br);
                        if (bbs[fifi - 1]->Ins.empty() ||
                            bbs[fifi - 1]->Ins[bbs[fifi - 1]->Ins.size() - 1]->label != "retInstruction")
                        bbs[fifi - 1]->addIns(br);
                    }

                        //has else if to final
                    else {
                        cout << bbs[fifi - 1]->Ins.size() << endl;
                        //if体的最后一句要跳到final块
                        if (bbs[fifi - 1]->Ins.empty() ||
                            bbs[fifi - 1]->Ins[bbs[fifi - 1]->Ins.size() - 1]->label != "retInstruction")
                            bbs[fifi - 1]->addIns(br);
                    }
                    cout << "sur 3" << endl;
//                    if(bbs.size()>=2&&!bbs[bbs.size()-2]->Ins.empty()&&bbs[bbs.size()-2]->Ins[bbs[bbs.size()-2]->Ins.size()-1]->label!="brInstruction"&&bbs[bbs.size()-2]->Ins[bbs[bbs.size()-2]->Ins.size()-1]->label!="break")
//                    bbs[bbs.size()-2]->addIns(new brInstruction(bbs[bbs.size()-1]));


                    cout << "survive" << endl;

                }
            } else if (a->nodeTypeEnum() == Stmt_ &&
                       (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == WHILETK)) {
                if (i == a->sonNodes.begin() + 4) {
                    circu.push_back((Cond *) a->sonNodes[2]);
                    ((Cond *) a->sonNodes[2])->ifblock = IRRoot->funcList[IRRoot->funcList.size() -
                                                                          1]->BasicBlockList.size();

                    basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                    curBlock = newBlock;
                }
                visitorIterator(*i);
                if (i == a->sonNodes.begin() + 4) {
                    circu.pop_back();
                    basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
                    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
                    curBlock = newBlock;
                    int start = ((Cond *) a->sonNodes[2])->startBlock;
                    int ifif = ((Cond *) a->sonNodes[2])->ifblock;
//                        int elel = ((Cond *) a->sonNodes[2])->elseblock;
                    auto datas = ((Cond *) a->sonNodes[2])->group;
                    auto bbs = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList;
                    for (int ii = ifif; ii < bbs.size(); ii++) {
                        for (int kk = 0; kk < bbs[ii]->Ins.size(); kk++) {
                            auto sb = bbs[ii]->Ins[kk];
                            cout << sb->output() << endl;
                            if (sb->label == "break") {
                                cout << "hhhhimfw" << endl;
                                bbs[ii]->Ins[kk] = new brInstruction(bbs[bbs.size() - 1]);
                            }
                        }
                    }


                    int bias = 0;
                    for (auto j: datas) {
                        for (int k = 0; k < j; k++) {
                            int suc, fail;
                            if (j == *(datas.end() - 1))//最后一组or
                            {
                                if (k < j - 1) {
                                    suc = start + bias + 1;

                                    fail = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size() - 1;

                                } else {
                                    suc = ifif;

                                    fail = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.size() - 1;

                                }
                            } else {
                                if (k < j - 1)//不是最后i一组or
                                {
                                    suc = start + bias + 1;
                                    fail = bias - k + j + start;
                                } else {
                                    suc = ifif;
                                    fail = bias - k + j + start;
                                }
                            }
                            brInstruction *br = new brInstruction(
                                    bbs[start + bias]->Ins[bbs[start + bias]->Ins.size() - 1], bbs[suc], bbs[fail]);
                            bbs[start + bias]->addIns(br);
                            bias++;

                        }
                    }
                    brInstruction *br = new brInstruction(bbs[start]);
                    bbs[bbs.size() - 2]->addIns(br);
                    if (bbs[bbs.size() - 2]->Ins[bbs[bbs.size() - 2]->Ins.size() - 1]->label != "brInstruction" &&
                        bbs[bbs.size() - 2]->Ins[bbs[bbs.size() - 2]->Ins.size() - 1]->label != "break")
                        bbs[bbs.size() - 2]->addIns(new brInstruction(bbs[bbs.size() - 1]));

                }
            } else if (a->nodeTypeEnum() == FuncDef_ && i == (a->sonNodes.end() - 1)) {
                visitorIterator(*i);
                cout << "here" << endl;
                if (curBlock->Ins.empty() || curBlock->Ins[curBlock->Ins.size() - 1]->label != "retInstruction") {
                    curBlock->addIns(new retInstruction(VOID, 0));
                }
                cout << "here" << endl;
            } else visitorIterator(*i);
        }


        if (a->nodeTypeEnum() == Block_) {
            cur = curbackup;
        }
    }
}

void visitor(string a) {

    cur = rootTable;
    try {
        visitorIterator(ASTRoot);
    }
    catch (exception e) {
        cout << e.what() << endl; // 打印异常信息
    }
    cout << "===================" << endl;

    outputModule(a);
}

void CompUnit::IRVisit() {
    cout << "im comp" << endl;
    IRRoot = new Module();
    IRRoot->funcList = {};
    IRRoot->globalList = {};
    auto *tmp1 = new funcSymbol("getint", 1, {});
    rootTable->funcSymbols.insert(pair<string, funcSymbol *>("getint", tmp1));
    IRRoot->funcList.push_back(new Function(tmp1, 1, IRRoot));
    struct param t2 = {"a", 0};
    auto *tmp2 = new funcSymbol("putint", 0, {});
    tmp2->params.push_back(t2);
    rootTable->funcSymbols.insert(pair<string, funcSymbol *>("putint", tmp2));
    IRRoot->funcList.push_back(new Function(tmp2, 1, IRRoot));
    auto *tmp3 = new funcSymbol("putch", 0, {});
    tmp3->params.push_back(t2);
    rootTable->funcSymbols.insert(pair<string, funcSymbol *>("putch", tmp3));
    IRRoot->funcList.push_back(new Function(tmp3, 1, IRRoot));
//    Function * getint=new Function(tmp1,1,IRRoot);
}

void VarDef::IRVisit() {
    vector<node *> sons = sonNodes;
    string name = ((tokenNode *) sons[0])->str;//变量的名字
    printf("cur->%p\n", cur);
    varSymbol *sym = cur->varSymbols[name];//这个变量的符号表项 todo：这里没错，就是读这一层
    sym->defined = 1;
    Value *newone;
    if (cur != rootTable) {
        newone = new localVar(sym, 0);
        for (auto emm: sons) {
            if (emm->nodeTypeEnum() == ConstExp_) {
                int ai = visitConstExp(emm->sonNodes[0]);
                cout << ai << endl;
                sym->dims.push_back(ai);

            }
        }
        allocaInstruction *newins = new allocaInstruction((localVar *) newone, sym->dims);
        curBlock->addIns(newins);
        cout << newins->output() << endl;
    } else {
        newone = new GlobalVariable(sym, IRRoot);
        for (auto emm: sons) {
            if (emm->nodeTypeEnum() == ConstExp_) {
                int ai = visitConstExp(emm->sonNodes[0]);
                sym->dims.push_back(ai);

            }
        }
        IRRoot->globalList.push_back((GlobalVariable *) newone);
    }
    int globalinitval = 0;
    if(sons.size()==5&&sons[2]->nodeTypeEnum()==TokenNode_&&((tokenNode*)sons[2])->type==GETINTTK)
    {
        callInstruction *ci = new callInstruction(IRRoot->funcList[0], {});
        curBlock->addIns(ci);
        storeInstruction *si = new storeInstruction(ci, newone);
        curBlock->addIns(si);
    }else
    {
        if (sons[sons.size() - 1]->nodeTypeEnum() == InitVal_)//如果有初始化
        {
            node *init = sons[sons.size() - 1];
            if (init->sonNodes.size() == 1) {
                Exp *exp = (Exp *) sons[sons.size() - 1]->sonNodes[0];
                if (cur != rootTable) {
                    Value *imsb = calExp((node *) exp);
                    storeInstruction *sto = new storeInstruction(imsb, newone);
                    curBlock->addIns(sto);
                }
                if (cur == rootTable) {
                    int ret = visitConstExp((node *) exp->sonNodes[0]);

                    vector<vector<int> > a1 = {};
                    vector<int> a2 = {};
                    a2.push_back(ret);
                    a1.push_back(a2);
                    ((GlobalVariable *) newone)->initval = a1;
                }
            } else {
                //是数组
                int dim = sym->dim;
                if (dim == 1) {
                    cout<<"yiweishuzu"<<endl;
                    auto sonsons = init->sonNodes;
                    int len = sym->dims[0];//应该有这么多个
                    int cnt = 0;
                    if (cur != rootTable) {
                        for (int i = 0; i < sonsons.size(); i++) {
                            if (sonsons[i]->nodeTypeEnum() == InitVal_) {
                                cnt++;
                                Value *ans = calExp(sonsons[i]->sonNodes[0]);
                                localVar *meow = (localVar *) newone;
                                vector<Value *> tmp = {};
                                tmp.push_back(new constantInt(0));
                                tmp.push_back(new constantInt(cnt - 1));
                                auto gpr = new getelementptrInstruction(meow, tmp);
                                pointer* haha= new pointer(meow,gpr);
                                auto str = new storeInstruction(ans, haha);
                                curBlock->addIns(gpr);
                                curBlock->addIns(str);
                            }
                        }
                    }else
                    {
                        GlobalVariable *meow = (GlobalVariable *) newone;
                        vector<int>a={};

                        for (int i = 0; i < sonsons.size(); i++) {

                            if (sonsons[i]->nodeTypeEnum() == InitVal_) {
                                cout<<numToSyn[sonsons[i]->sonNodes[0]->sonNodes[0]->nodeTypeEnum()]<<endl;
                                int ans= visitConstExp(sonsons[i]->sonNodes[0]->sonNodes[0]);
                                cout<<"the ret is "<<ans;
                                a.push_back(ans);
                                cout<<"pushed"<<endl;
                            }
                        }
                        cout<<meow->sym->dims.size()<<endl;
                        int em=meow->sym->dims[0]-a.size();
                        cout<<em<<endl;
                        while(em--)
                        {
                            a.push_back(0);
                        }
                        meow->initval.push_back(a);

                    }

                } else {
                    cout << "erweishuzu" << endl;
                    auto sonsons = init->sonNodes;

                    if (cur != rootTable)
                    {
                        int cnt1 = 0, cnt2 = 0;
                        for (int i = 0; i < sonsons.size(); i++)//外层循环{init.init,init...}
                        {
                            if (sonsons[i]->nodeTypeEnum() == InitVal_) {
                                cnt2 = 0;
                                int len = sym->dims[cnt1];//应该有这么多个init
                                cout << "should have " << len << endl;
                                auto sonsonsons = sonsons[i]->sonNodes;
                                cnt1++;
                                for (int j = 0; j < sonsonsons.size(); j++) {
                                    if (sonsonsons[j]->nodeTypeEnum() == InitVal_) {
                                        cnt2++;
                                        Value *ans = calExp(sonsonsons[j]->sonNodes[0]);
                                        localVar *meow = (localVar *) newone;
                                        vector<Value *> tmp = {};
                                        tmp.push_back(new constantInt(0));
                                        tmp.push_back(new constantInt(cnt1 - 1));
                                        tmp.push_back(new constantInt(cnt2 - 1));
                                        auto gpr = new getelementptrInstruction(meow, tmp);
                                        pointer* haha= new pointer(meow,gpr);
                                        auto str = new storeInstruction(ans, haha);

                                        curBlock->addIns(gpr);
                                        curBlock->addIns(str);
                                    }
                                }
                            }
                        }
                    }else
                    {
                        int cnt1 = 0, cnt2 = 0;
                        GlobalVariable *meow = (GlobalVariable *) newone;
                        for(auto k : meow->sym->dims)
                        {
                            cout<<"dims "<<k<<endl;
                        }
                        for (int i = 0; i < sonsons.size(); i++)//外层循环{init.init,init...}
                        {

                            if (sonsons[i]->nodeTypeEnum() == InitVal_) {
                                vector<int>a={};
                                cnt1++;cnt2 = 0;
                                int len = meow->sym->dims[1];//应该有这么多个init
                                auto sonsonsons = sonsons[i]->sonNodes;
                                for (int j = 0; j < sonsonsons.size(); j++) {
                                    if (sonsonsons[j]->nodeTypeEnum() == InitVal_) {
                                        cnt2++;
                                        int ans = visitConstExp(sonsonsons[j]->sonNodes[0]->sonNodes[0]);
                                        cout<<"the ret is "<<ans;
                                        a.push_back(ans);
                                    }
                                }
                                int em=len-a.size();
                                cout<<"len "<<len<<endl;
                                while(em--)
                                {
                                    a.push_back(0);
                                }
                                meow->initval.push_back(a);
                            }

                        }
                        int len=sym->dims[0];
                        int em=len-meow->initval.size();
                        while(em--)
                        {
                            vector<int> aa={};
                            meow->initval.push_back(aa);
                        }

                    }

                }
            }


        }
        else
        {
            if(cur==rootTable)
            {
                if(sym->dim==0)
                {
                    vector<vector<int> > a1 = {};
                    vector<int> a2 = {};
                    a2.push_back(0);
                    a1.push_back(a2);
                    ((GlobalVariable *) newone)->initval = a1;
                }else if(sym->dim==1)
                {
                    int len=sym->dims[0];
                    vector<vector<int> > a1 = {};
                    vector<int> a2 = {};
                    a1.push_back(a2);
                    ((GlobalVariable *) newone)->initval = a1;
                }
                else
                {
                    int len=sym->dims[0];
                    vector<vector<int> > a1 = {};
                    vector<int> a2 = {};
                    while(len--)
                    {
                        a1.push_back(a2);
                    }
                    ((GlobalVariable *) newone)->initval = a1;
                }
            }
        }
    }

}

void ConstDef::IRVisit() {//先当成变量存取内存 todo
    vector<node *> sons = sonNodes;
    string name = ((tokenNode *) sons[0])->str;//变量的名字
    printf("cur->%p\n", cur);
    varSymbol *sym = cur->varSymbols[name];//这个变量的符号表项 todo：这里没错，就是读这一层
    sym->defined = 1;
    Value *newone;
    int sum=1;
    if (cur != rootTable) {
        newone = new localVar(sym, 0);
        for (auto emm: sons) {
            if (emm->nodeTypeEnum() == ConstExp_) {
                int ai = visitConstExp(emm->sonNodes[0]);
                cout << ai << endl;
                sym->dims.push_back(ai);
sum*=ai;
            }
        }
        cout<<"putsh bask "<<sum<<" 0"<<endl;
        for(int emm=1;emm<=sum;emm++) sym->values.push_back(0);
        allocaInstruction *newins = new allocaInstruction((localVar *) newone, sym->dims);
        curBlock->addIns(newins);
    } else {

        newone = new GlobalVariable(sym, IRRoot);
        for (auto emm: sons) {
            if (emm->nodeTypeEnum() == ConstExp_) {
                int ai = visitConstExp(emm->sonNodes[0]);
                sym->dims.push_back(ai);
                sum*=ai;
            }
        }
        cout<<"putsh bask "<<sum<<" 0"<<endl;
        for(int emm=1;emm<=sum;emm++) sym->values.push_back(0);
        IRRoot->globalList.push_back((GlobalVariable *) newone);
        cout<<"1048"<<endl;
    }
    int globalinitval = 0;
    if (sons[sons.size() - 1]->nodeTypeEnum() == ConstInitVal_)//如果有初始化
    {
        node *init = sons[sons.size() - 1];          // constinitval
        if (init->sonNodes.size() == 1) {
            ConstExp *exp = (ConstExp *) init->sonNodes[0];
            if (cur != rootTable) {
                int imsb= visitConstExp(exp->sonNodes[0]);
                storeInstruction *sto = new storeInstruction(new constantInt(imsb), newone);
                curBlock->addIns(sto);
                sym->values[0]=imsb;
            }
            if (cur == rootTable) {
                int ret= visitConstExp(exp->sonNodes[0]);

                vector<vector<int> > a1 = {};
                vector<int> a2 = {};
                a2.push_back(ret);
                a1.push_back(a2);
                ((GlobalVariable *) newone)->initval = a1;
                sym->values[0]=ret;
            }
        } else {
            //是数组
            int dim = sym->dim;
            if (dim == 1) {
                cout<<"yiweishuzu"<<endl;
                auto sonsons = init->sonNodes;
                int len = sym->dims[0];//应该有这么多个
                int cnt = 0;
                if (cur != rootTable) {
                    for (int i = 0; i < sonsons.size(); i++) {
                        if (sonsons[i]->nodeTypeEnum() == ConstInitVal_) {

                            cnt++;
                            int ans = visitConstExp(sonsons[i]->sonNodes[0]->sonNodes[0]);
                            sym->values[cnt-1]=ans;
                            localVar *meow = (localVar *) newone;
                            vector<Value *> tmp = {};
                            tmp.push_back(new constantInt(0));////////////
                            tmp.push_back(new constantInt(cnt - 1));
                            auto gpr = new getelementptrInstruction(meow, tmp);
                            pointer* haha= new pointer(meow,gpr);
                            auto str = new storeInstruction(new constantInt(ans), haha);
                            curBlock->addIns(gpr);
                            curBlock->addIns(str);
                        }
                    }
                }else
                {
                    GlobalVariable *meow = (GlobalVariable *) newone;
                    vector<int>a={};

                    for (int i = 0; i < sonsons.size(); i++) {

                        if (sonsons[i]->nodeTypeEnum() == ConstInitVal_) {
                            cout<<numToSyn[sonsons[i]->sonNodes[0]->sonNodes[0]->nodeTypeEnum()]<<endl;
                            int ans= visitConstExp(sonsons[i]->sonNodes[0]->sonNodes[0]);
                            sym->values[cnt-1]=ans;
                            cout<<"the ret is "<<ans;
                            a.push_back(ans);
                            cout<<"pushed"<<endl;
                        }
                    }
                    cout<<meow->sym->dims.size()<<endl;
                    int em=meow->sym->dims[0]-a.size();
                    cout<<em<<endl;
                    while(em--)
                    {
                        a.push_back(0);
                    }
                    meow->initval.push_back(a);

                }

            } else {
                cout << "erweishuzu" << endl;
                auto sonsons = init->sonNodes;

                if (cur != rootTable)
                {
                    int cnt1 = 0, cnt2 = 0;
                    for (int i = 0; i < sonsons.size(); i++)//外层循环{init.init,init...}
                    {

                        if (sonsons[i]->nodeTypeEnum() == ConstInitVal_) {//第cnt1个一维
                            cnt2 = 0;
                            int len = sym->dims[1];//应该有这么多个init
                            cout << "should have " << len << endl;
                            auto sonsonsons = sonsons[i]->sonNodes;
                            cnt1++;
                            for (int j = 0; j < sonsonsons.size(); j++) {
                                if (sonsonsons[j]->nodeTypeEnum() == ConstInitVal_) {
//
                                    cnt2++;//cnt1个一维里的第cnt2个

                                    int ans = visitConstExp(sonsonsons[j]->sonNodes[0]->sonNodes[0]);
                                    sym->values[(cnt1-1)*(sym->dims[1])+cnt2-1]=ans;
                                    localVar *meow = (localVar *) newone;
                                    vector<Value *> tmp = {};
                                    tmp.push_back(new constantInt(0));
                                    tmp.push_back(new constantInt(cnt1 - 1));
                                    tmp.push_back(new constantInt(cnt2 - 1));
                                    auto gpr = new getelementptrInstruction(meow, tmp);
                                    pointer* haha= new pointer(meow,gpr);
                                    auto str = new storeInstruction(new constantInt(ans), haha);

                                    curBlock->addIns(gpr);
                                    curBlock->addIns(str);
                                }
                            }
                        }
                    }
                }else
                {
                    int cnt1 = 0, cnt2 = 0;
                    GlobalVariable *meow = (GlobalVariable *) newone;
                    cout<<"size"<<sym->dims.size()<<endl;
                    for (auto asd : sym->dims)
                        cout<<asd<<" "<<endl;
                    for (int i = 0; i < sonsons.size(); i++)//外层循环{init.init,init...}
                    {
                        cout<<"while 2"<<endl;
                        if (sonsons[i]->nodeTypeEnum() == ConstInitVal_) {
                            vector<int>a={};
                            cnt1++;cnt2 = 0;
                            int len = sym->dims[1];//应该有这么多个init
                            auto sonsonsons = sonsons[i]->sonNodes;
                            for (int j = 0; j < sonsonsons.size(); j++) {
                                cout<<"hereherer"<<endl;
                                if (sonsonsons[j]->nodeTypeEnum() == ConstInitVal_) {
                                    cnt2++;
                                    cout<<"first "<<cnt1-1<<" second "<<cnt2-1<<endl;
                                    int ans = visitConstExp(sonsonsons[j]->sonNodes[0]->sonNodes[0]);
                                    cout<<"the ret is "<<ans;
                                    cout<<"its for "<<cnt1-1<<" "<<cnt2-1<<endl;//下标 2 0

                                    sym->values[(cnt1-1)*len+cnt2-1]=ans;
                                    cout<<"the ret is "<<ans;
                                    a.push_back(ans);
                                }
                            }
                            cout<<"here"<<endl;
                            int em=len-a.size();
                            cout<<len<<" "<<a.size()<<endl;
                            while(em--)
                            {
                                a.push_back(0);
                            }
                            meow->initval.push_back(a);
                        }

                    }
                    int len=sym->dims[0];
                    int em=len-meow->initval.size();
                    while(em--)
                    {
                        cout<<"while 1"<<endl;
                        vector<int> aa={};
                        meow->initval.push_back(aa);
                    }

                }

            }
        }


    }

}

extern symbolTable *rootTable;

void FuncDef::IRVisit() {
    Value::num = 0;
    vector<node *> sons = this->sonNodes;
    string name = ((tokenNode *) sons[1])->str;
    Function *main = new Function(rootTable->funcSymbols[name], 0, IRRoot);
    IRRoot->funcList.push_back(main);

    symbolTable *foresee = ((Block *) sons[sons.size() - 1])->table;
    vector<Value *> mm = {};
    int index=0;
    node * ff=rootTable->funcSymbols[name]->ff;
    for (int iii=0;iii<main->funcs->params.size();iii++) {

        cout<<main->funcs->params[iii].name<<" "<<main->funcs->params[iii].dim<<endl;
        foresee->varSymbols[main->funcs->params[iii].name]->defined = 1;
        foresee->varSymbols[main->funcs->params[iii].name]->dims={};
        auto lv=new localVar(foresee->varSymbols[main->funcs->params[iii].name], 0);

            node * fpar=ff->sonNodes[2*index];
            if(fpar->sonNodes.size()==2)//int
            {
                lv->dims={};
                main->funcs->params[iii].last=0;
            }else if(fpar->sonNodes.size()==4)//yiwei
            {
                lv->type=I32;
             lv->numarray++;
lv->dims.push_back(0);
                foresee->varSymbols[main->funcs->params[iii].name]->ispointer=1;
                foresee->varSymbols[main->funcs->params[iii].name]->dims.push_back(0);
                main->funcs->params[iii].last=0;
            }else
            {
                lv->type=ARRAY;
                lv->numarray++;
                foresee->varSymbols[main->funcs->params[iii].name]->ispointer=1;
                node * conex=fpar->sonNodes[fpar->sonNodes.size()-2];
                int ans= visitConstExp(conex->sonNodes[0]);
                cout<<"last "<<ans<<endl;
                main->funcs->params[iii].last=ans;
                lv->dims.push_back(0);
                lv->dims.push_back(ans);
                foresee->varSymbols[main->funcs->params[iii].name]->dims.push_back(0);
                foresee->varSymbols[main->funcs->params[iii].name]->dims.push_back(ans);
            }
        mm.push_back(lv);
            index++;
    }
    basicBlock *newBlock = new basicBlock(main);
    curBlock = newBlock;
    ;
    //为了让标签不是0，要在前面处理参数
    cout<<"imgonna alloc for prm"<<endl;
    for (auto i: mm) {
        if(((localVar *) i)->sym->ispointer==1)
        {
            cout<<"prm is pointer"<<endl;
            cout<<" thereris a fun"<<endl;
            cout<<((localVar *) i)->getName()<<" "<<((localVar *) i)->sym->dim<<" "<<((localVar *) i)->numarray<<endl;
            for (auto ii:((localVar *) i)->sym->dims) cout<<ii<<endl;
            continue;
        }
        cout<<"a int param"<<endl;
        auto backup = i;
        auto imsb = new localVar(((localVar *) i)->sym, 0);
        allocaInstruction *newins = new allocaInstruction((localVar *) imsb, imsb->sym->dims);
        curBlock->addIns(newins);
        cout<<"a int param"<<endl;
        storeInstruction *sto = new storeInstruction(i, imsb);
        cout<<"a int param"<<endl;
        curBlock->addIns(sto);

    }

    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
}

void MainFuncDef::IRVisit() {
    Value::num = 0;
    cout << "im main" << endl;
    Function *main = new Function(rootTable->funcSymbols["main"], 0, IRRoot);
    IRRoot->funcList.push_back(main);
    basicBlock *newBlock = new basicBlock(main);
    curBlock = newBlock;
    IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
}


void Stmt::IRVisit() {
    cout << "im in stmt" << endl;


    vector<node *> sons = this->sonNodes;
    if (((tokenNode *) sons[0])->type == RETURNTK) {
        if (sons.size() > 2) {
            Value *ans = calExp(sons[1]);
            curBlock->addIns(new retInstruction(I32, ans));
        } else {
            curBlock->addIns(new retInstruction(VOID, 0));
        }
    }
    else if (sons[0]->nodeTypeEnum() == LVal_ && sons[sons.size() - 2]->nodeTypeEnum() == Exp_)
        //这是给左值赋值 a=b; a=123;
    {
        cout << "im in a=b" << endl;
        node *lval = sons[0];
        auto sonsons = lval->sonNodes;
        string name = ((tokenNode *) lval->sonNodes[0])->str;
        cout << name << endl;
        varSymbol *now = nullptr;
        symbolTable *nowtable = cur;
        while (now == nullptr) {
            cout << "one layer" << endl;
            now = nowtable->varSymbols[name];
            if (now == nullptr)
                nowtable = nowtable->parent;
            else if (now->defined == 0) {
                cout << "not defined" << endl;
                now = nullptr;
                nowtable = nowtable->parent;

            }
        }
        cout << "here" << endl;
        localVar *lv = now->addr;
        cout<<"announce "<<lv->getName()<<endl;
        //知道虚拟寄存器了，要从内存里取出来load
        //%10 = load i32, i32* %2
        int dim = lv->sym->dim;

        if (dim == 0&&lv->numarray==0) {

            node *exp = sons[sons.size() - 2];
            Value *ret = calExp((node *) exp);
            storeInstruction *sto = new storeInstruction(ret, lv);
            curBlock->addIns(sto);

        } else {
            vector<Value *> tmp = {};
            if(lv->numarray==0)
            tmp.push_back(new constantInt(0));
            for (auto em: sonsons) {
                if (em->nodeTypeEnum() == Exp_) {
                    tmp.push_back(calExp(em));
                }

            }
            auto gpt = new getelementptrInstruction(lv, tmp);
            pointer* haha= new pointer(lv,gpt);
            curBlock->addIns(gpt);
            node *exp = sons[sons.size() - 2];
            Value *ret = calExp((node *) exp);
            storeInstruction *sto = new storeInstruction(ret, haha);
            curBlock->addIns(sto);

        }


    } else if (sons[0]->nodeTypeEnum() == LVal_) {
        node *lval = sons[0];
        auto sonsons = lval->sonNodes;
        string name = ((tokenNode *) lval->sonNodes[0])->str;
        cout << name << endl;
        varSymbol *now = nullptr;
        symbolTable *nowtable = cur;
        while (now == nullptr) {
            cout << "one layer" << endl;
            now = nowtable->varSymbols[name];
            if (now == nullptr)
                nowtable = nowtable->parent;
            else if (now->defined == 0) {
                cout << "not defined" << endl;
                now = nullptr;
                nowtable = nowtable->parent;

            }
        }

        localVar *lv = now->addr;

        //知道虚拟寄存器了，要从内存里取出来load
        //%10 = load i32, i32* %2
        int dim = lv->sym->dim;
        loadInstruction *ls;
        if (dim == 0) {
            callInstruction *ci = new callInstruction(IRRoot->funcList[0], {});
            curBlock->addIns(ci);
            storeInstruction *si = new storeInstruction(ci, lv);
            curBlock->addIns(si);

        } else {
            vector<Value *> tmp = {};
            if(lv->numarray==0)
                tmp.push_back(new constantInt(0));

            for (auto em: sonsons) {
                if (em->nodeTypeEnum() == Exp_) {
                    tmp.push_back(calExp(em));
                }

            }
            auto gpt = new getelementptrInstruction(lv, tmp);

            curBlock->addIns(gpt);

            callInstruction *ci = new callInstruction(IRRoot->funcList[0], {});
            curBlock->addIns(ci);
            pointer* pr=new pointer(lv,gpt);
            storeInstruction *si = new storeInstruction(ci, pr);
            curBlock->addIns(si);
        }


    }
    else if (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == PRINTFTK) {
        string format = ((tokenNode *) sons[2])->str;
        int len = format.size();
        int order = 0;
        vector<Value *> ans;
        for (auto j: sons) {
            if (j->nodeTypeEnum() == Exp_) {
                ans.push_back(calExp(j));
            }
        }

        for (int i = 1; i < len - 1; i++) {
            if (format[i] != '%' && format[i] != '\\') {
                constantInt *num = new constantInt(format[i]);
                putchar(format[i]);
                vector<Value *> tmp = {};
                tmp.push_back(num);
                callInstruction *ci = new callInstruction(IRRoot->funcList[2], tmp);
                curBlock->addIns(ci);
            } else if (format[i] == '%') {
//                Value *ans = calExp(sons[4 + order * 2]);
                vector<Value *> tmp = {};
                tmp.push_back(ans[0]);
                cout << ans[0]->getName() << endl;
                ans.erase(ans.begin());
                cout << "rest:" << endl;
                for (auto haha: ans) {
                    cout << haha->getName() << endl;
                }
                callInstruction *ci = new callInstruction(IRRoot->funcList[1], tmp);
                curBlock->addIns(ci);
                i++;
                order++;


            } else {
                constantInt *num = new constantInt(10);
                vector<Value *> tmp = {};
                tmp.push_back(num);
                callInstruction *ci = new callInstruction(IRRoot->funcList[2], tmp);
                curBlock->addIns(ci);
                i++;
            }
        }
    }
    else if (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == IFTK) {
        cout << "im in if" << endl;
        node *cond = sons[2];
        handcond((Cond *) cond);
    }
    else if (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == WHILETK) {
        cout << "im in while" << endl;
        node *cond = sons[2];
        handcond((Cond *) cond);
    }
    else if (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == CONTINUETK) {
        Cond *now = *(circu.end() - 1);
        int start = now->startBlock;
        auto bbs = IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList;
        //auto cmp=new icmpInstruction(new constantInt(1),new constantInt(1),EQ_);
        basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
        IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
        //auto brbr = new brInstruction(cmp,bbs[start],bbs[bbs.size()-1]);
        auto brbr1 = new brInstruction(bbs[start]);
//        curBlock->addIns(cmp);
        curBlock->addIns(brbr1);
        curBlock = newBlock;
    }
    else if (sons[0]->nodeTypeEnum() == TokenNode_ && ((tokenNode *) sons[0])->type == BREAKTK) {
        auto br = new brInstruction();
        curBlock->addIns(br);
        basicBlock *newBlock = new basicBlock(IRRoot->funcList[IRRoot->funcList.size() - 1]);
        IRRoot->funcList[IRRoot->funcList.size() - 1]->BasicBlockList.push_back(newBlock);
        curBlock=newBlock;
    }
    else if (sons[0]->nodeTypeEnum() == Exp_) {
        calExp(sons[0]);
    }
}

