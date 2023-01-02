#ifndef TYPECODE_H
#define TYPECODE_H

#include <map>
#include <vector>
#include <string>
using namespace std;
enum typeCode
{
      IDENFR,
      INTCON,
      STRCON,
      MAINTK,
      CONSTTK,
      INTTK,
      BREAKTK,
      CONTINUETK,
      IFTK,
      ELSETK,
      NOT,
      AND,
      OR,
      WHILETK,
      GETINTTK,
      PRINTFTK,
      RETURNTK,
      PLUS,
      MINU,
      VOIDTK,
      MULT,
      DIV,
      MOD,
      LSS,
      LEQ,
      GRE,
      GEQ,
      EQL,
      NEQ,
      ASSIGN,
      SEMICN,
      COMMA,
      LPARENT,
      RPARENT,
      LBRACK,
      RBRACK,
      LBRACE,
      RBRACE,
    BITANDTK
};
static std::vector <std::string> numToEnum={
    "IDENFR",
      "INTCON",
      "STRCON",
      "MAINTK",
      "CONSTTK",
      "INTTK",
      "BREAKTK",
      "CONTINUETK",
      "IFTK",
      "ELSETK",
      "NOT",
      "AND",
      "OR",
      "WHILETK",
      "GETINTTK",
      "PRINTFTK",
      "RETURNTK",
      "PLUS",
      "MINU",
      "VOIDTK",
      "MULT",
      "DIV",
      "MOD",
      "LSS",
      "LEQ",
      "GRE",
      "GEQ",
      "EQL",
      "NEQ",
      "ASSIGN",
      "SEMICN",
      "COMMA",
      "LPARENT",
      "RPARENT",
      "LBRACK",
      "RBRACK",
      "LBRACE",
      "RBRACE",
      "BITANDTK"};
static std::map<std::string, typeCode> reservedMap{{"main", MAINTK}, {"const", CONSTTK}, {"int", INTTK}, {"break", BREAKTK}, {"continue", CONTINUETK}, {"if", IFTK}, {"else", ELSETK}, {"while", WHILETK}, {"getint", GETINTTK}, {"printf", PRINTFTK}, {"return", RETURNTK}, {"void", VOIDTK},{"bitand",BITANDTK}};
static std::map<std::string, typeCode> doubleCharMap{
    {"&&", AND}, {"||", OR}, {"<=", LEQ}, {">=", GEQ}, {"==",EQL,},
    {"!=", NEQ}};
static std::map<char, typeCode> charMap{
    {'!', NOT}, {'+', PLUS}, {'-', MINU}, {'*', MULT}, {'/', DIV}, {'%', MOD},
     {'<', LSS}, {'>', GRE}, {'=', ASSIGN}, {';', SEMICN}, {',', COMMA}, {'(', LPARENT}, {')', RPARENT},
     {'[',LBRACK},{']',RBRACK},{'{',LBRACE},{'}',RBRACE}};
#endif