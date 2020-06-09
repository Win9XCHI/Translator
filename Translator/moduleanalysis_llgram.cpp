#include "moduleanalysis_llgram.h"
#include <iostream>

ModuleAnalysis_LLGram::ModuleAnalysis_LLGram() {
    DB = new ProjectDB("DB.db");
    if (!DB->CheckConnection()) {
        DB->createConnection();
    }
}

ModuleAnalysis_LLGram::ModuleAnalysis_LLGram(ProjectDB *db) {
    DB = db;
}

ModuleAnalysis_LLGram::~ModuleAnalysis_LLGram() {}

void ModuleAnalysis_LLGram::ConstructRules() {
    vector<string> words;

    for (unsigned int i = 0; i < Grammar["Rule"].size(); i++) {
        split3(Grammar["Rule"][i], words);
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

    split3(terminal, Terminal, ',');
    split3(no_terminal, No_Terminal, ',');
    split3(rules, Rule, ',');

    Grammar["Terminal"] = Terminal;
    Grammar["No_Terminal"] = No_Terminal;
    Grammar["Rule"] = Rule;
    Grammar["Entry"] = Entry;

    ConstructRules();
    num = Numbering(Grammar["Entry"][0]);

    file.close();
}

//very bad, it created only two tables
bool ModuleAnalysis_LLGram::CheckGrammar() {
    DB->DROP("DataVector");
    DB->DROP("MatrixPredecessors");
    DB->DROP("MatrixFollowing");

    map<string, bool> columns;
    map<string, map<string, bool>> MatrixPredecessors, MatrixFollowing;
    //vector<QString> columnsName;
    //vector<QString> SEmpty;

    map<string, bool> pattern;
    for (unsigned int i = 0; i < Grammar["No_Terminal"].size(); i++) {
        pattern[Grammar["No_Terminal"][i]] = false;
    }
    for (unsigned int i = 0; i < Grammar["Terminal"].size(); i++) {
        pattern[Grammar["Terminal"][i]] = false;
    }

    for (pair<string, vector<vector<Part>>> item : Rules) {
        //vector<QString> str;
        //SEmpty.push_back();
        //str.push_back(QString::fromStdString(item.first));
        //columnsName.push_back(QString::fromStdString(item.first));
        //str.push_back("BOOLEAN");
        //str.push_back("");

        //columns.push_back(str);
        columns[item.first] = SearchEmpty(item.first, "0");
        MatrixPredecessors[item.first] = SearchPredecessors(item.first, pattern); //Search connect
    }
    cout << endl;
    for (pair<string, bool> item : columns) {
        cout << item.first << " -> " << item.second;
        cout << endl;
    }

    columns.clear();

    for (pair<string, map<string, bool>> item : MatrixPredecessors) {

        for (pair<string, bool> item2Level : item.second) {

            if (item2Level.second == true) {
                auto it = MatrixPredecessors.find(item2Level.first);

                for (pair<string, bool> i : it->second) {

                    if (i.second == true) {
                        auto place = item.second.find(i.first);
                        place->second = true;
                    }
                }
            }
        }
    }

    //Following("PROGRAM", MatrixFollowing, MatrixPredecessors);

    for (pair<string, bool> item : pattern) {
        cout << item.first << " | ";
    }
    cout << endl;
    for (pair<string, map<string, bool>> item : MatrixPredecessors) {
        cout << item.first << "   | ";
        for (pair<string, bool> item2Level : item.second) {
            cout << item2Level.second << " | ";
        }
        cout << endl;
    }

    cout << endl;

    //DB->CREATE_TABLE("DataVector", columns);
    /*columns.clear();

    cout << endl;
    for (unsigned int i = 0; i < columnsName.size(); i++) {
        qDebug() << i+1 << ". " << columnsName[i] << " -> " << SEmpty[i];
    }*/
    //DB->Insert("DataVector", columnsName, SEmpty);

    return false;
}

//n, item->second[i],
string ModuleAnalysis_LLGram::PartFollowing(unsigned int n, vector<Part> item) {
    for (unsigned int t = n; t < item.size(); t++) {

        if (CheckTerminal(item[t].Name)) {
            return "";
        }
        if (!SearchEmpty(item[t].Name, "0")) {
           return item[t].Name;
        }
    }

    return "";
}

void ModuleAnalysis_LLGram::Following(string str, map<string, map<string, bool>> &matrix, map<string, map<string, bool>> PrevMatrix) {
    auto item = Rules.find(str);

    for (unsigned int i = 0; i < item->second.size(); i++) {

        for (unsigned int n = 1; n < item->second[i].size(); n++) {

            if (!CheckTerminal(item->second[i][n].Name)) {
                string next = PartFollowing(n, item->second[i]);

                if (next != "") {
                    matrix[item->second[i][n].Name][next] = true;

                    auto itemFromPrevMatrix = PrevMatrix.find(next);

                    for (pair<string, bool> item2Level : itemFromPrevMatrix->second) {

                        if (item2Level.second) {
                            matrix[item->second[i][n].Name][item2Level.first] = true;
                        }
                    }
                }
            }
            /*if (n != item->second[i].size() - 1) {

                if (!CheckTerminal(item->second[i][n].Name) && !SearchEmpty(item->second[i][n + 1].Name, "0")) {
                    matrix[item->second[i][n].Name][item->second[i][n + 1].Name] = true;

                    auto itemFromPrevMatrix = PrevMatrix.find(item->second[i][n + 1].Name);

                    for (pair<string, bool> item2Level : itemFromPrevMatrix->second) {

                        if (item2Level.second) {
                            matrix[item->second[i][n].Name][item2Level.first] = true;
                        }
                    }
                }

                if (!CheckTerminal(item->second[i][n].Name) && SearchEmpty(item->second[i][n + 1].Name, "0")) {

                }


            } else {

            }*/
        }
    }
}

map<string, bool> ModuleAnalysis_LLGram::SearchPredecessors(string str, map<string, bool> pattern) {
    auto item = Rules.find(str);

    if (item != Rules.cend()) {
        for (unsigned int i = 0; i < item->second.size(); i++) {

            for (unsigned int n = 1; n < item->second[i].size(); n++) {

                pattern[item->second[i][n].Name] = true;
                if (CheckTerminal(item->second[i][n].Name) || !SearchEmpty(item->second[i][n].Name, "0")) {
                    break;
                }
            }
        }
    }

    return pattern;
}

bool ModuleAnalysis_LLGram::SearchEmpty(string str, string prevStr) {
    auto item = Rules.find(str);
    bool flag = false;

    if (item != Rules.cend()) {
        for (unsigned int i = 0; i < item->second.size(); i++) {

            if (CheckTerminal(item->second[i][item->second[i].size() - 1].Name)) {
                continue;
            }

            if (item->second[i][item->second[i].size() - 1].Name == "e") {
                return true;
            }

            if (!CheckTerminal(item->second[i][item->second[i].size() - 1].Name) && item->second[i][item->second[i].size() - 1].Name != str && item->second[i][item->second[i].size() - 1].Name != prevStr) {
                flag = SearchEmpty(item->second[i][item->second[i].size() - 1].Name, str);
            }
        }
    }

    return flag;
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

    if (item != Rules.cend()) {
        for (unsigned int n = 0; n < item->second.size(); n++) {

            if (item->second[n][0].number == number) {
                return true;
            }
        }
    }

    return false;
}

string ModuleAnalysis_LLGram::CheckNext(unsigned int number) {

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

    throw "CheckNext";
}

Part ModuleAnalysis_LLGram::GetNext(unsigned int number) {

        for (pair<string, vector<vector<Part>>> item : Rules) {

            for (unsigned int n = 0; n < item.second.size(); n++) {

                for (unsigned int t = 0; t < item.second[n].size(); t++) {

                    if (item.second[n][t].number == number) {

                        if(!CheckTerminal(item.second[n][t].Name) && t != 0) {
                            auto it = Rules.find(item.second[n][t].Name);
                            return it->second[0][0];
                        }

                        if (t != item.second[n].size() - 1) {
                            return item.second[n][t + 1];
                        }

                        if (t == item.second[n].size() - 1) {
                            return Part();
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

Part ModuleAnalysis_LLGram::SearchTerm(string str, unsigned int number, int i) {
    auto item = Rules.find(str);

    if (item != Rules.cend()) {
        if (i == -1) {
            return item->second[0][0];
        }
        if (CheckTerminal(item->second[i][number].Name) || item->second[i][number].Name == "e") {
              return item->second[i][number];
        }

        return SearchTerm(item->second[i][number + 1].Name, number + 1, i);
    }

    return Part();
}

QString ModuleAnalysis_LLGram::SearchTerminalE(unsigned int Stack_pointer) {
    if (!DB->SELECT("Stack_In", "ParsingTable")) {
        throw "Error DB";
    }

    vector<int> cont;
    DB->GetStackNumber(cont);
    QString definition = "Number = " + QString::number(cont[Stack_pointer]);

    if (!DB->SELECT("Terminal", "ParsingTable", definition)) {
        throw "Error DB";
    }

    return DB->GetTerminal();
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

    DB->Insert("ParsingTable", listColumns, listValue);
}

void ModuleAnalysis_LLGram::ModuleLeft(Part Ob, unsigned int &Stack_pointer) {
    bool Accept = false, Stack_Out = false, Error = true;
    QString Terminal;
    unsigned int Route(-1), Stack_In = 0;

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

    Route = SearchTerm(Ob.Name, GetNext(Ob.number).number, -1).number;

    for (unsigned int i = 0; i < count; i++) {
        QString str = QString::fromStdString(SearchTerm(Ob.Name, 1, i).Name);

        if (str == "e") {
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
        DB->DeleteRecord("ParsingTable");

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

void ModuleAnalysis_LLGram::ControlOutputVectorRules() {

    cout << endl << "Number         Term\No_term";

    for (unsigned int i = 0; i < VectorRules.size(); i++) {
        cout << endl;
        cout << VectorRules[i].number << "          " << VectorRules[i].Name;
    }
}

list<RecordParsingTable> ModuleAnalysis_LLGram::GetTable() {
    list<RecordParsingTable> object;
    DB->GetTerm(object);
    return object;
}
