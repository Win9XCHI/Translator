#include "moduleanalysis_llgram.h"
#include <iostream>

ModuleAnalysis_LLGram::ModuleAnalysis_LLGram() {
    DB.createConnection();
}

ModuleAnalysis_LLGram::~ModuleAnalysis_LLGram() {}

void ModuleAnalysis_LLGram::ConstructRules() {
    vector<string> words;

    for (unsigned int i = 0; i < Grammar["Rule"].size(); i++) {
        ModuleAnalysis_LLGram::split3(Grammar["Rule"][i], words);
        vector<Part> parts;

        for (unsigned int n = 0; n < words.size(); n++) {

            if (n != 1) {
                Part Ob;
                Ob.number = 0;
                Ob.Name = words[n];

                parts.push_back(Ob);
            }
        }

        if (Rules.find(words[0]) == Rules.cend()) {
            Rules[words[0]] = vector<vector<Part>>();
        }

        Rules[words[0]].push_back(parts);
        words.clear();
    }
}

void ModuleAnalysis_LLGram::UploadGrammar(string path) {
    string str;
    char buff[255];

    ifstream file(path);

    while (!file.eof()) {
        file.getline(buff, 255);

        str += buff;
    }

    string terminal = str.substr(5, str.find("}") - 6);
    string no_terminal = str.substr(str.find("}") + 5);
    string rules = no_terminal.substr(no_terminal.find("}") + 5);
    no_terminal = no_terminal.substr(0, no_terminal.find("}") - 1);
    rules = rules.substr(0, rules.find("}") - 1);
    string entry = str.substr(str.find_last_of(",") + 2, str.find_last_of(")") - str.find_last_of(",") - 3);

    vector<string> Terminal;
    vector<string> No_Terminal;
    vector<string> Rule;
    vector<string> Entry;
    Entry.push_back(entry);

    ModuleAnalysis_LLGram::split3(terminal, Terminal, ',');
    ModuleAnalysis_LLGram::split3(no_terminal, No_Terminal, ',');
    ModuleAnalysis_LLGram::split3(rules, Rule, ',');

    Grammar["Terminal"] = Terminal;
    Grammar["No_Terminal"] = No_Terminal;
    Grammar["Rule"] = Rule;
    Grammar["Entry"] = Entry;

    ConstructRules();

    file.close();
}

/*bool ModuleAnalysis_LLGram::SearchArrayEmpty(vector<bool> &SearchEmpty) {

    for (pair<string, vector<vector<Part>>> item : Rules) {

        for (unsigned int n = 0; n < item.second.size(); n++) {

            for (unsigned int t = 0; t < item.second[n].size(); t++) {

                if (!CheckTerminal(item.second[n][t].Name) && item.second[n][t].Name != "e") {
                    SearchEmpty.push_back(SearchArrayEmpty(SearchEmpty, ));
                }

                if (item.second[n][t].Name == "e" && t == item.second[n].size() - 1) {

                    SearchEmpty.push_back(true);
                }
            }
        }


    }

    return false;
}*/

bool ModuleAnalysis_LLGram::CheckGrammar() {
    DB.DROP("DataVector");
    DB.DROP("MatrixPredecessors");
    DB.DROP("MatrixFollowing");

    vector<std::vector<QString>> columns;


    for (pair<string, vector<vector<Part>>> item : Rules) {
        vector<QString> str;
        str.push_back(QString::fromStdString(item.first));
        str.push_back("BOOLEAN");
        str.push_back("");

        columns.push_back(str);
    }

    DB.CREATE_TABLE("DataVector", columns);
    columns.clear();

    vector<bool> SearchEmpty;
}

void ModuleAnalysis_LLGram::SortRules() {
    VectorRules.clear();
    unsigned int number = 1;

    for (unsigned int i = 0; i < num; i++) {

        for (pair<string, vector<vector<Part>>> item : Rules) {

            for (unsigned int n = 0; n < item.second.size(); n++) {

                for (unsigned int t = 0; t < item.second[n].size(); t++) {

                    if (item.second[n][t].number == number) {
                        VectorRules.push_back(item.second[n][t]);
                        number++;
                        break;
                    }
                }
            }
        }
    }
}

unsigned int ModuleAnalysis_LLGram::Numbering(string str, unsigned int number) {
    vector<string> Ob;

    try {
        auto item = Rules.find(str);
        if (item == Rules.cend()) {
            throw "Not found";
        }

        if (item->second[0][0].number == 0) {
            for (unsigned int i = 0; i < item->second.size(); i++) {
                item->second[i][0].number = number;
                number++;
            }

            for (unsigned int i = 0; i < item->second.size(); i++) {

                for (unsigned int n = 1; n < item->second[i].size(); n++) {
                    item->second[i][n].number = number;
                    number++;
                }
            }

            for (unsigned int i = 0; i < item->second.size(); i++) {
                for (unsigned int n = 1; n < item->second[i].size(); n++) {

                   if (CheckTerminal(item->second[i][n].Name)  || item->second[i][n].Name == "e") {
                        continue;
                   }

                    if (!CheckTerminal(item->second[i][n].Name)) {
                        Ob.push_back(item->second[i][n].Name);
                        continue;
                    }

                    throw "Not found rule name";

                }
            }
        }

        for (unsigned int i = 0; i < Ob.size(); i++) {
            number = Numbering(Ob[i], number);
        }

    } catch (string str) {

        cout << endl << str;
        throw "Error";
    }

    return number;
}

//true -> lest, false -> right
bool ModuleAnalysis_LLGram::CheckLocationNo_Term(string str, unsigned int number) {
    auto item = Rules.find(str);

    for (unsigned int n = 0; n < item->second.size(); n++) {

        if (item->second[n][0].number == number) {
            return true;
        }
    }

    return false;
}

string ModuleAnalysis_LLGram::CheckNext(unsigned int number) {

    for (unsigned int i = 0; i < num; i++) {

        for (pair<string, vector<vector<Part>>> item : Rules) {

            for (unsigned int n = 0; n < item.second.size(); n++) {

                for (unsigned int t = 0; t < item.second[n].size(); t++) {

                    if (item.second[n][t].number == number) {

                        if (t == item.second[n].size() - 1) {
                            return "End";
                        }

                        if (CheckTerminal(item.second[n][t].Name)) {
                            return "Terminal";
                        }

                        return "No_Terminal";

                    }
                }
            }
        }
    }

    throw "CheckNext";
}

Part ModuleAnalysis_LLGram::GetNext(unsigned int number) {

    for (unsigned int i = 0; i < num; i++) {

        for (pair<string, vector<vector<Part>>> item : Rules) {

            for (unsigned int n = 0; n < item.second.size(); n++) {

                for (unsigned int t = 0; t < item.second[n].size(); t++) {

                    if (item.second[n][t].number == number) {

                        return item.second[n][t];
                    }
                }
            }
        }
    }

    throw "CheckNext";
}

unsigned int ModuleAnalysis_LLGram::CountVariants(string str) {
    auto item = Rules.find(str);

    if (item != Rules.cend()) {
        return item->second.size();
    }

    return 0;
}

Part ModuleAnalysis_LLGram::SearchTerm(string str, unsigned int number, unsigned int i) {
    auto item = Rules.find(str);

    for (unsigned int n = 0; n < item->second.size(); n++) {

        if (item->second[n][0].number == number) {

            if (CheckTerminal(item->second[n][i + 1].Name) || item->second[n][i + 1].Name == "e") {
                 return item->second[n][i + 1];
             }

            return SearchTerm(item->second[n][i + 1].Name, item->second[n][i + 1].number, i);

        }
    }

    return Part();
}

QString ModuleAnalysis_LLGram::SearchTerminalE(unsigned int Stack_pointer) {
    if (!DB.SELECT("Stack_In", "ParsingTable")) {
        throw "Error DB";
    }

    vector<int> cont;
    DB.GetStackNumber(cont);
    QString definition = "Number = " + QString::number(cont[Stack_pointer]);

    if (!DB.SELECT("Terminal", "ParsingTable", definition)) {
        throw "Error DB";
    }

    return DB.GetTerminal();
}

void ModuleAnalysis_LLGram::InsertRecord(unsigned int number, QString Terminal, unsigned int Route, bool Accept, unsigned int Stack_In, bool Stack_Out, bool Error) {
    std::vector<QString> listColumns;
    std::vector<QString> listValue;
    listColumns.push_back("Number");
    listColumns.push_back("Terminal");
    listColumns.push_back("Route");
    listColumns.push_back("Accept");
    listColumns.push_back("Stack_In");
    listColumns.push_back("Stack_Out");
    listColumns.push_back("Error");

    listValue.push_back(QString::number(number));
    listValue.push_back(Terminal);
    listValue.push_back(QString::number(Route));
    listValue.push_back(QString::number(Accept));
    listValue.push_back(QString::number(Stack_In));
    listValue.push_back(QString::number(Stack_Out));
    listValue.push_back(QString::number(Error));

    DB.Insert("ParsingTable", listColumns, listValue);
}

void ModuleAnalysis_LLGram::ModuleLeft(Part Ob, unsigned int &Stack_pointer) {
    bool Accept = false, Stack_Out = false, Error = true;
    QString Terminal;
    unsigned int Route, Stack_In = 0;

    Part next = GetNext(Ob.number); //not next

    if (CheckTerminal(next.Name)) {

        Terminal = QString::fromStdString(next.Name);
        Route = next.number;

        if (CountVariants(Ob.Name) > 1) {
            Error = false;
        }
    } else {

        if (next.Name == "e") {
            Terminal = SearchTerminalE(Stack_pointer);
            Route = next.number;

            if (CountVariants(Ob.Name) > 1) {
                Error = false;
            }
        }
    }

    InsertRecord(Ob.number, Terminal, Route, Accept, Stack_In, Stack_Out, Error);
}

void ModuleAnalysis_LLGram::ModuleE(Part Ob, unsigned int &Stack_pointer) {
    bool Accept = false, Stack_Out = false, Error = true;
    QString Terminal;
    unsigned int Route, Stack_In = 0;

    Terminal = SearchTerminalE(Stack_pointer);
    Route = 0;
    Accept = true;
    Stack_Out = true;

    InsertRecord(Ob.number, Terminal, Route, Accept, Stack_In, Stack_Out, Error);

    Stack_pointer++;
}

void ModuleAnalysis_LLGram::ModuleTerminal(Part Ob) {
    bool Accept = false, Stack_Out = false, Error = true;
    QString Terminal;
    unsigned int Route, Stack_In = 0;

    Terminal = QString::fromStdString(Ob.Name);
    Route = CheckNext(Ob.number) == "End" ? 0 : Ob.number + 1;
    Accept = true;

    InsertRecord(Ob.number, Terminal, Route, Accept, Stack_In, Stack_Out, Error);
}

void ModuleAnalysis_LLGram::ModuleNoTerminal(Part Ob, unsigned int &Stack_pointer) {
    bool Accept = false, Stack_Out = false, Error = true;
    QString Terminal;
    unsigned int Route, Stack_In = 0;

    unsigned int count = CountVariants(Ob.Name);
    if (count == 0) {
        throw "Zero";
    }

    Route = SearchTerm(Ob.Name, Ob.number, 0).number;

    for (unsigned int i = 0; i < count; i++) {
        QString str = QString::fromStdString(SearchTerm(Ob.Name, Ob.number, i).Name);

        if (str.indexOf("e") != -1) {
            Terminal += SearchTerminalE(Stack_pointer) + " ";
        } else {
            Terminal += str + " ";
        }
    }

    if (CheckNext(Ob.number) != "End") {
       Stack_In = Ob.number + 1;
    }

    InsertRecord(Ob.number, Terminal, Route, Accept, Stack_In, Stack_Out, Error);
}

void ModuleAnalysis_LLGram::FillingTable(Part Ob, unsigned int &Stack_pointer) {

    if (CheckLocationNo_Term(Ob.Name, Ob.number)) { //left (terminal and right)
        ModuleLeft(Ob, Stack_pointer);
        return;
    }

    if (Ob.Name == "e") {
        ModuleE(Ob, Stack_pointer);
        return;
    }

    if (CheckTerminal(Ob.Name)) {
        ModuleTerminal(Ob);
        return;
    }

    ModuleNoTerminal(Ob, Stack_pointer);
}

void ModuleAnalysis_LLGram::ParsingTable() {

    try {
        DB.DeleteRecord("ParsingTable");
        num = Numbering(Grammar["Entry"][0]);

        SortRules();

        unsigned int Stack_pointer = 0;
        for (unsigned int i = 0; i < VectorRules.size(); i++) {
            FillingTable(VectorRules[i], Stack_pointer);
        }

    } catch (...) {
        cout << endl << "End. Error!";
    }

}

bool ModuleAnalysis_LLGram::CheckTerminal(string str) {
    for (unsigned int q = 0; q < Grammar["Terminal"].size(); q++) {

         if (str == Grammar["Terminal"][q]) {

             return true;
         }
    }
    return false;
}

template <class Container>
void ModuleAnalysis_LLGram::split3(const std::string& str, Container& cont, char delim)
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

void ModuleAnalysis_LLGram::ControlOutputVectorRules() {

    cout << endl << "Number         Term\No_term";

    for (unsigned int i = 0; i < VectorRules.size(); i++) {
        cout << endl;
        cout << VectorRules[i].number << "          " << VectorRules[i].Name;
    }
}
