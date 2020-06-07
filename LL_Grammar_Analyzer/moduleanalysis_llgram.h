#ifndef MODULEANALYSIS_LLGRAM_H
#define MODULEANALYSIS_LLGRAM_H
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include "projectbd.h"

using namespace std;

struct Part {
    string Name;
    unsigned int number;

    Part() : Name(""), number(0) {}
};

class ModuleAnalysis_LLGram{
    ProjectBD DB;
    map<string, vector<string>> Grammar;
    map<string, vector<vector<Part>>> Rules;
    unsigned int num;
    vector<Part> VectorRules;

    void ConstructRules();
    unsigned int Numbering(string str, unsigned int number = 1);
    void FillingTable(Part, unsigned int &);
    bool CheckTerminal(string);
    bool CheckLocationNo_Term(string, unsigned int);
    void SortRules();
    Part SearchTerm(string, unsigned int, int);

    void ModuleLeft(Part, unsigned int &);
    void ModuleE(Part, unsigned int &);
    void ModuleTerminal(Part);
    void ModuleNoTerminal(Part, unsigned int &);
    QString SearchTerminalE(unsigned int);
    Part GetNext(unsigned int);
    void InsertRecord(unsigned int, QString, unsigned int, bool, unsigned int, bool, bool);

    string PartFollowing(unsigned int, vector<Part>);
    void Following(string, map<string, map<string, bool>> &, map<string, map<string, bool>>);
    map<string, bool> SearchPredecessors(string, map<string, bool>);
    bool SearchEmpty(string, string);

    Part NextTerm(string, unsigned int);
    string CheckNext(unsigned int);
    unsigned int CountVariants(string);
    Part e(vector<unsigned int>);

    template <class Container>
    static void split3(const std::string& str, Container& cont, char delim = ' ');

public:
    ModuleAnalysis_LLGram();
    ~ModuleAnalysis_LLGram();

    void UploadGrammar(string path);

    bool CheckGrammar();

    void ParsingTable();

    void ControlOutputVectorRules();
};



#endif // MODULEANALYSIS_LLGRAM_H
