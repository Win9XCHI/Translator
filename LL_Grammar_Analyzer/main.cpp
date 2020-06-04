#include <QCoreApplication>
#include "moduleanalysis_llgram.h"

int main() {
    ModuleAnalysis_LLGram Ob;

    Ob.UploadGrammar("ExampleGrammar.ul");

    Ob.ParsingTable();

    Ob.ControlOutputVectorRules();

    Ob.ParsingTable();

    return 0;
}
