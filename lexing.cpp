#include "includes/lexing.h"

#include <ctype.h>
#include "includes/token.h"

#include <fstream>

char *tmp;
char *end_;
int currentLine = 1;
void DFAReserved()
{
    string tmpstr = "";
    while (*tmp == '_' || isalpha(*tmp) || isdigit(*tmp))
    {
        tmpstr += *tmp;
        tmp++;
    }
    if (reservedMap.find(tmpstr) != reservedMap.end())
    {
        token::tokens.push_back( new token(tmpstr, reservedMap[tmpstr], currentLine));
        return;
    }
    else
    {
        token::tokens.push_back(new  token(tmpstr, IDENFR, currentLine));
        return;
    }
}
void DFASymbols()
{
    string tmpstr = "";
    if (tmp + 1 < end_)
    {
        if (*tmp == '&' && *(tmp + 1) == '&')
        {
            token::tokens.push_back(new token("&&", AND, currentLine));
            tmp+=2;
            return;
        }

        else if (*tmp == '|' && *(tmp + 1) == '|')
        {
            token::tokens.push_back(new token("||", OR, currentLine));tmp+=2;
            return;
        }
        else if (*tmp == '<' && *(tmp + 1) == '=')
        {
            token::tokens.push_back(new token("<=", LEQ, currentLine));tmp+=2;
            return;
        }
        else if (*tmp == '>' && *(tmp + 1) == '=')
        {
            token::tokens.push_back(new token(">=", GEQ, currentLine));tmp+=2;
            return;
        }
        else if (*tmp == '=' && *(tmp + 1) == '=')
        {
            token::tokens.push_back(new token("==", EQL, currentLine));tmp+=2;
            return;
        }
        else if (*tmp == '!' && *(tmp + 1) == '=')
        {
            token::tokens.push_back(new token("!=", NEQ, currentLine));tmp+=2;
            return;
        }
    }
    tmpstr+=*tmp;
    token::tokens.push_back(new token(tmpstr, charMap[*tmp], currentLine));
    tmp+=1;
}
void DFAFormatString()
{
    string tmpstr = "";
    tmpstr += "\"";
    tmp++;
    while (*tmp != '"')
    {
        tmpstr += *tmp;
        tmp++;
    }
    tmpstr += *tmp;
    tmp++;
    token::tokens.push_back(new token(tmpstr, STRCON, currentLine));

}
void DFAIntConst()
{
    int ans = 0;
    ans += *tmp - '0';
    tmp++;
    while (isdigit(*tmp))
    {
        ans = ans * 10 + *tmp-'0';
        tmp++;
    }
    token::tokens.push_back(new token(to_string(ans), INTCON, currentLine)); //草 为什么不用new
}
void DFA(std::string raw)
{

    int len = raw.length();
    tmp = &(raw[0]);
    end_ = &(raw[0]) + len;
    while (tmp - &raw[0] < len)
    {
        if (*tmp == ' ' || *tmp == '\t')
            tmp++;
        else if (*tmp == '\n')
        {
            tmp++;
            currentLine++;
        }
        else if (isdigit(*tmp))
            DFAIntConst();
        else if (isalpha(*tmp) || *tmp == '_')
            DFAReserved();
        else if (*tmp == '"')
                DFAFormatString();
        else
            DFASymbols();
    }
}
