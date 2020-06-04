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
};

class ModuleAnalysis_LLGram {
    map<string, vector<string>> Grammar;
    map<string, vector<vector<Part>>> Rules;
    vector<Part> VectorRules;
    ProjectBD DB;

    void ConstructRules();
    unsigned int Numbering(string str, unsigned int number = 1);
    void SortRules();

    template <class Container>
    static void split3(const std::string& str, Container& cont, char delim = ' ');

public:
    ModuleAnalysis_LLGram();
    ~ModuleAnalysis_LLGram();

    void UploadGrammar(string path);

    bool CheckGrammar();

    void ParsingTable();
};



#endif // MODULEANALYSIS_LLGRAM_H
