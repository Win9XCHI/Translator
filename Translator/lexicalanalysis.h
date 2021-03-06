#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H
#include <string>
#include <fstream>
#include <list>
#include "projectdb.h"
#include <cstdlib>

class LexicalAnalysis {
private:
    std::ifstream file;
    ProjectDB *DB;
    std::map<std::string, std::string> Key_Words;
    std::map<std::string, std::string> Operations;
    std::map<std::string, std::string> Delimiters;
    std::list<std::string> error;
    int countForArray;  //[]
    int countForFunc;   //()

    unsigned int LastNumberId;
    unsigned int LastNumberOp;
    unsigned int LastNumberLit;
    unsigned int LastNumberKey;
    unsigned int LastNumberDel;

    //Function switch functions for processing chars
    //Input: -
    //Output: -
    void Controller();

    //Function for processing ID chars
    //Input: char
    //Output: next char after ID
    char ID(char);

    //Function for processing Operation chars
    //Input: char
    //Output: next char after Operation
    char Operation(char);

    //Function for processing Literals chars
    //Input: char
    //Output: next char after Literals
    char Literals(char);

    //Function for processing comments chars
    //Input: char
    //Output: next char after Literals
    char Comment(char);

    //Function for initialization insert in database
    //Input: cade, name, type (lexems)
    //Output: -
    void InitialInsert(QString, QString, QString);

    //Function for check ID on keywords
    //Input: ID
    //Output: there any matches
    bool Check_key_words(const std::string);

    //Function for check end lexems
    //Input: char, do parentheses count
    //Output: end or not
    bool Check_endLex(char, bool = true);

    //Function for check ID on hexadecimal digit
    //Input: ID
    //Output: there any matches
    bool Check16Digit(const std::string);

    void Initializer();

public:
    LexicalAnalysis();
    explicit LexicalAnalysis(ProjectDB *);
    explicit LexicalAnalysis(std::string filename);
    ~LexicalAnalysis();

    //Function initialization lexical analysis
    //Input: file name or not if file name specified during construction
    //Output: -
    void Start(std::string filename = "");

    //Function returned errors list which created at work
    //Input: -
    //Output: errors list
    std::list<std::string> GetError();

    //Return lexems
    //Input: -
    //Output: lexems list
    std::list<Lex> GetLexical();

    //Return id
    //Input: -
    //Output: id list
    std::list<Lex> GetID();

    //Clear class data
    //Input: -
    //Output: -
    void Clear();
};

#endif // LEXICALANALYSIS_H
