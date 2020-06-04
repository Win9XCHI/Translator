#include <QCoreApplication>
#include "moduleanalysis_llgram.h"

int main() {
    ModuleAnalysis_LLGram Ob;

    Ob.UploadGrammar("ExampleGrammar.ul");

    Ob.ParsingTable();


    //example create table

    ProjectBD ObDB;

    std::vector<std::vector<QString>> columns;
    std::vector<QString> str;

    str.push_back("contact_id");
    str.push_back("INTEGER");
    str.push_back("PRIMARY KEY");

    columns.push_back(str);
    str.clear();

    str.push_back("first_name");
    str.push_back("TEXT");
    str.push_back("NOT NULL");

    columns.push_back(str);
    str.clear();

    str.push_back("last_name");
    str.push_back("TEXT");
    str.push_back("NOT NULL");

    columns.push_back(str);
    str.clear();

    ObDB.CREATE_TABLE("contacts", columns);

    return 0;
}
