#ifndef SYNTACTISANALYSIS_H
#define SYNTACTISANALYSIS_H
#include <iostream>
#include <string>
#include <list>
#include "projectbd.h"

using namespace std;

class SyntacticAnalysis {
private:
    ProjectBD DB;
    unsigned int number;
    list<unsigned int> Stack;

public:
    SyntacticAnalysis();

    QString Start();
};

#endif // SYNTACTISANALYSIS_H
