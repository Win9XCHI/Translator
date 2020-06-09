#ifndef SYNTACTISANALYSIS_H
#define SYNTACTISANALYSIS_H
#include <iostream>
#include <string>
#include <list>
#include "projectdb.h"
#include "functions.h"

using namespace std;

class SyntacticAnalysis {
private:
    ProjectDB *DB;
    unsigned int number;
    list<unsigned int> Stack;

public:
    SyntacticAnalysis();
    explicit SyntacticAnalysis(ProjectDB *);

    QString Start();
};

#endif // SYNTACTISANALYSIS_H
