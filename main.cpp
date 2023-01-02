#include "includes/token.h"
#include "includes/io.h"
#include "includes/lexing.h"
#include "includes/parsing.h"
#include "includes/exception.h"
#include "includes/IRVisitor.h"
#include "IRNode.h"

string rawString;
int autoMode = 0;
extern node *  ASTRoot;
extern symbolTable* rootTable;
extern Module *IRRoot;
void autoTest()
{
    cout << "============autoTest Mode================" << endl;
    string instr = "";
    string outstr = "";
    for (int i = 1; i <= 30; i++)
    {
        token::tokens.clear();
        ASTRoot= nullptr;
        rootTable= nullptr;
        IRRoot= nullptr;
        instr = "A/testfile" + to_string(i) + ".txt";
        outstr = "A/myoutput" + to_string(i) + ".ll";
        rawString = read(instr);
        DFA(rawString);
        lexingOutput(outstr);
        ASTRoot=parsing();
        parsingOutput(outstr);
        exceptionVisitor();
        errorOutput("error.txt");
        visitor(outstr);
    }
    for (int i = 1; i <= 30; i++)
    {
        token::tokens.clear();
        ASTRoot= nullptr;
        rootTable= nullptr;
        IRRoot= nullptr;
        instr = "B/testfile" + to_string(i) + ".txt";
        outstr = "B/myoutput" + to_string(i) + ".ll";
        rawString = read(instr);
        DFA(rawString);
        lexingOutput(outstr);
        ASTRoot=parsing();
        parsingOutput(outstr);
        exceptionVisitor();
        errorOutput("error.txt");
        visitor(outstr);
    }
    for (int i = 1; i <= 30; i++)
    {
        token::tokens.clear();
        ASTRoot= nullptr;
        rootTable= nullptr;
        IRRoot= nullptr;
        instr = "C/testfile" + to_string(i) + ".txt";
        outstr = "C/myoutput" + to_string(i) + ".ll";
        rawString = read(instr);
        DFA(rawString);
        lexingOutput(outstr);
        ASTRoot=parsing();
        parsingOutput(outstr);
        exceptionVisitor();
        errorOutput("error.txt");
        visitor(outstr);
    }
}
int main(int args, char *argv[])
{
    if (args > 1 && strcmp(argv[1], "at") == 0)
        autoMode = 1;
    if (autoMode == 1)
        autoTest();
    else
    {
        rawString = read("testfile.txt");
        DFA(rawString);
        lexingOutput("output.txt");
        ASTRoot=parsing();
        parsingOutput("output.txt");
        exceptionVisitor();
        errorOutput("error.txt");
        visitor("llvm_ir.txt");
    }
}
