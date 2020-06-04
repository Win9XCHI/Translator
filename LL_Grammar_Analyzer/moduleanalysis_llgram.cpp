#include "moduleanalysis_llgram.h"
#include <iostream>

ModuleAnalysis_LLGram::ModuleAnalysis_LLGram() {}

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

bool ModuleAnalysis_LLGram::CheckGrammar() {

}

void SortRules();

unsigned int ModuleAnalysis_LLGram::Numbering(string str, unsigned int number) {

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
                    bool flag = false;

                    for (unsigned int q = 0; q < Grammar["Terminal"].size(); q++) {

                        if (item->second[i][n].Name == Grammar["Terminal"][q] || item->second[i][n].Name == "e") {
                            flag = true;
                            break;
                        }
                    }

                    if (flag) {
                        continue;
                    }

                    flag = false;

                    for (unsigned int q = 0; q < Grammar["No_Terminal"].size(); q++) {

                        auto one = item->second[i][n].Name;
                        auto two = Grammar["No_Terminal"][q];

                        if (item->second[i][n].Name == Grammar["No_Terminal"][q]) {
                            flag = true;
                            break;
                        }
                    }

                    if (flag) {
                        number = Numbering(item->second[i][n].Name, number);
                        continue;
                    }

                    throw "Not found rule name";

                }
            }
        }

    } catch (string str) {

        cout << endl << str;
        throw "Error";
    }

    return number;
}

void ModuleAnalysis_LLGram::ParsingTable() {

    try {
        cout << endl << "Count: " << Numbering(Grammar["Entry"][0]);

        cout << "";

    } catch (...) {
        cout << endl << "End. Error!";
    }

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
