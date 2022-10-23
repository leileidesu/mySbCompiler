#include "includes/io.h"
#include "includes/token.h"
#include "includes/parsing.h"
string read(string a)
{
    std::ifstream input(a);
    string raw = "", line;
    while (getline(input, line))
    {
//        cerr<<line<<endl;
        raw += line;
        raw += "\n";
    }
    input.close();

    string ans = "";
    for (int i = 0; i < raw.length();)
    {
        switch (raw[i])
        {
        case '\'':
            ans += raw[i];
            i++;
            while (raw[i] != '\'')
            {
                ans += raw[i];
                i++;
            }
            ans += raw[i];
            i++;

            break;
        case '\"':
            ans += raw[i];
            i++;
            while (raw[i] != '\"')
            {
                ans += raw[i];
                i++;
            }
            ans += raw[i];
            i++;
            break;
        case '/':
            if (raw[i + 1] == '*')
            {
                i += 2;
                while (i + 1 < raw.length() && !((raw[i] == '*') && (raw[i + 1] == '/')))
                {
                    if (raw[i] == '\n')
                    {
                        ans += raw[i];
                    }
                    i++;
                }
                i += 2;
            }
            else if (raw[i + 1] == '/')
            {
                while (raw[i] != '\n')
                {
                    i++;
                }
                ans += raw[i];
                i++;
            }
            else
            {
                ans += raw[i];
                i++;
            }
            break;
        case '\r':
            i++;
            break;
        default:
            ans += raw[i];
            i++;
            break;
        }
    }

    return ans;
}

void lexingOutput(string a)
{
   
    std::ofstream output(a);
    for (int i = 0; i < token::tokens.size(); i++)
    {
        output << numToEnum[token::tokens[i]->type] << " " << token::tokens[i]->str << endl;
    }
    output.close();
}
extern node * ASTRoot;
void parsingOutput(string a)
{

    std::ofstream output(a);
    lastOrder(ASTRoot,output);
    output.close();
}
extern vector<myException*> exceptions;
bool cmp(const myException* x,const myException *y)
{
    return (*x).line<(*y).line;
}
void errorOutput(string a)
{
    std::ofstream output(a);
    vector<myException*> tmpExceptions=exceptions;
    sort(tmpExceptions.begin(),tmpExceptions.end(),cmp);
    for(auto x:tmpExceptions)
    {
        output<<x->what()<<endl;

    }
    output.close();
}