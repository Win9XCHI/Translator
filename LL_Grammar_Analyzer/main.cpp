#include <QCoreApplication>
#include "moduleanalysis_llgram.h"

int main() {
    ModuleAnalysis_LLGram Ob;

    Ob.UploadGrammar("D:\\DIIT KIT\\Translation system\\Translator\\Sourse\\ExampleGrammar.ul");

    Ob.ParsingTable();

    return 0;
}
