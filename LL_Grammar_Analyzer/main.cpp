#include <QCoreApplication>
#include "moduleanalysis_llgram.h"

int main() {
    ModuleAnalysis_LLGram Ob;

    Ob.UploadGrammar("ExampleGrammar.ul");

    //Ob.CheckGrammar(); fail

    Ob.ParsingTable();

    Ob.ControlOutputVectorRules();

    return 0;
}
