#include "lexicalanalysis.h"

LexicalAnalysis::LexicalAnalysis() : countForArray(0), countForFunc(0), LastNumberId(1), LastNumberOp(1), LastNumberLit(1), LastNumberKey(1), LastNumberDel(1) {
    DB.createConnection();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Key_Word'");
    Key_Words = DB.GetReserveLex();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Operation'");
    Operations = DB.GetReserveLex();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Delimiter'");
    Delimiters = DB.GetReserveLex();
}

LexicalAnalysis::LexicalAnalysis(std::string filename) : countForArray(0), countForFunc(0), LastNumberId(1), LastNumberOp(1), LastNumberLit(1), LastNumberKey(1), LastNumberDel(1) {
    DB.createConnection();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Key_Word'");
    Key_Words = DB.GetReserveLex();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Operation'");
    Operations = DB.GetReserveLex();

    DB.SELECT("Name, Type", "ReserveLex", "Type = 'Delimiter'");
    Delimiters = DB.GetReserveLex();

    file.open(filename);
}

LexicalAnalysis::~LexicalAnalysis() {}

void LexicalAnalysis::Start(std::string filename) {
    try {
        if (!filename.empty()) {
            file.open(filename);
        }

        if (!file.is_open()) {
            throw "System error: Not open file";
        }
        DB.DeleteRecord("ID");
        DB.DeleteRecord("Lex");

        Controller();
        file.close();

    } catch (std::string str) {
        error.push_back(str);
    }
}

void LexicalAnalysis::Controller() {
    char symbol = ' ';
    bool flag = false;

    while (!file.eof()) {

        if (!flag) {
            file.get(symbol);
        }
        if (symbol == '/') {
            symbol = Comment(symbol);
            flag = true;
        }
        if (isalpha(symbol)) {
            symbol = ID(symbol);
            flag = true;
        }
        if (symbol == '.' || symbol == '+' || symbol == '-' || symbol == '=' || symbol == '!' || symbol == '<' || symbol == '>' || symbol == '&' || symbol == '|' || symbol == '^') {
            symbol = Operation(symbol);
            flag = true;
        }
        if (isdigit(symbol) || symbol == '"' || symbol == '\'') {
            symbol = Literals(symbol);
            flag = true;
        }
        if (Check_endLex(symbol)) {
            flag = false;
        }
    }

    if (countForFunc != 0 || countForArray != 0) {
        char c[5];
        std::string str = "Parentheses mismatch: Func: ";
        itoa(countForFunc, c, 10);
        str += c;
        str += ", Array: ";
        itoa(countForArray, c, 10);
        str += c;

        error.push_back(str);
    }
}

char LexicalAnalysis::Comment(char symbol) {
    try {

        file.get(symbol);
        if (symbol != '/') {
            throw "Error comments";
        }

        while (symbol != '\n') {
            file.get(symbol);
        }
    } catch (std::string str) {
        error.push_back(str);
    }
    return symbol;
}

char LexicalAnalysis::Literals(char symbol) {
    std::string str;
    str.push_back(symbol);

    while (true) {
        file.get(symbol);

        if ((str[0] == '\'' && symbol == '\'') || (str[0] == '"' && symbol == '"')) {
            str.push_back(symbol);
            file.get(symbol);
            break;
        }
        if (Check_endLex(symbol, false) && str[0] != '\'' && str[0] != '"') {
            break;
        }
        str.push_back(symbol);
    }

    InitialInsert("L" + QString::number(LastNumberLit), QString::fromStdString(str), "Literal");

    return symbol;
}

bool LexicalAnalysis::Check_endLex(char symbol, bool flag) {
    std::string str;
    str.push_back(symbol);

    if (Delimiters.find(str) != Delimiters.cend() || symbol == '\n' || symbol == '\t') {

        if((symbol != '.' && symbol != '\n' && symbol != '\t' && symbol != ' ') && flag) {
            InitialInsert("D" + QString::number(LastNumberDel), QString::fromStdString(str), "Delimiter");
        }

        if (symbol == '[' && flag) {
            countForArray++;
        }
        if (symbol == ']' && flag) {
            countForArray--;
        }
        if (symbol == '(' && flag) {
            countForFunc++;
        }
        if (symbol == ')' && flag) {
            countForFunc--;
        }
        return true;
    }
    return false;
}

char LexicalAnalysis::Operation(char symbol) {
    std::string str;
    str.push_back(symbol);

    while (true) {
        file.get(symbol);

        if ((symbol == '+' && str[0] == '+') || (symbol == '-' && str[0] == '-') || (symbol == '.' && str[0] == '.') || (symbol == '&' && str[0] == '&') || (symbol == '|' && str[0] == '|')
                || (symbol == '!' && str[0] == '=') || (symbol == '=' && str[0] == '=') || (symbol == '<' && str[0] == '=') || (symbol == '>' && str[0] == '=')) {
            str.push_back(symbol);
        } else {
            break;
        }
    }

    try {
        if (str.size() > 2) {
            throw "Invalid operator: " + str;
        }
        if (Operations.find(str) == Operations.cend() && str != ".." && str != ".") {
            throw "Invalid operator: " + str;
        }
        if (str == ".." || str == ".") {
            InitialInsert("D" + QString::number(LastNumberDel), QString::fromStdString(str), "Delimiter");
        } else {
            InitialInsert("O" + QString::number(LastNumberOp), QString::fromStdString(str), "Operation");
        }

    } catch (std::string str) {
        error.push_back(str);
    }

    return symbol;
}

void LexicalAnalysis::InitialInsert(QString str1, QString str2, QString str3) {
    std::vector<QString> listColumns;
    std::vector<QString> listValue;
    listColumns.push_back("Code");
    listColumns.push_back("Name");
    listColumns.push_back("Type");

    DB.SELECT("Code", "Lex", "Name = '" + str2 + "'");
    QString str4 = DB.Check_Name();

    if (str4 != "") {
        str1 = str4;
    }

    listValue.push_back(str1);
    listValue.push_back(str2);
    listValue.push_back(str3);
    DB.Insert("Lex", listColumns, listValue);

    if (str3 == "Key_Word" && str4 == "") {
        LastNumberKey++;
    }

    if (str3 == "ID" && str4 == "") {
        listColumns.pop_back();
        listValue.pop_back();

        DB.Insert("ID", listColumns, listValue);

        LastNumberId++;
    }

    if (str3 == "Operation" && str4 == "") {
        LastNumberOp++;
    }

    if(str3 == "Literal" && str4 == "") {
        LastNumberLit++;
    }

    if(str3 == "Delimiter" && str4 == "") {
        LastNumberDel++;
    }
}

char LexicalAnalysis::ID(char symbol) {
    std::string str;
    str.push_back(symbol);

    while (true) {
        file.get(symbol);

        if (isalpha(symbol) || symbol == '_' || isdigit(symbol)) {
            str.push_back(symbol);
        } else {
            break;
        }
    }
    if (Check_key_words(str)) {
        InitialInsert("X" + QString::number(LastNumberKey), QString::fromStdString(str), "Key_Word");
    } else {
        if (Check16Digit(str)) {
            InitialInsert("L" + QString::number(LastNumberLit), QString::fromStdString(str), "Literal");
        } else {
            InitialInsert(QString::number(LastNumberId), QString::fromStdString(str), "ID");
        }
    }

    return symbol;
}

bool LexicalAnalysis::Check_key_words(const std::string str) {
    if (Key_Words.find(str) == Key_Words.cend()) {
        return false;
    }
    return true;
}

std::list<Lex> LexicalAnalysis::GetLexical() {
    std::list<Lex> object;
    DB.SELECT("Code, Name, Type", "Lex");
    DB.GetLex(object);

    return object;
}

std::list<Lex> LexicalAnalysis::GetID() {
    std::list<Lex> object;
    DB.SELECT("Code, Name, Key", "ID");
    DB.GetID(object);

    return object;
}

void LexicalAnalysis::Clear() {
    error.clear();
    countForArray = 0;
    countForFunc = 0;

    LastNumberId = 1;
    LastNumberOp = 1;
    LastNumberLit = 1;
    LastNumberKey = 1;
    LastNumberDel = 1;
}

std::list<std::string> LexicalAnalysis::GetError() {
    return error;
}

bool LexicalAnalysis::Check16Digit(const std::string str) {
    if (str[str.size() - 1] != 'h') {
        return false;
    }

    for (unsigned int i = 0; i < str.size() - 1; i++) {

        if (!isxdigit(str[i])) {
            return false;
        }
    }

    return true;
}

