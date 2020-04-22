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
    unsigned int sizeWrite = 0;
    while (symbol != EOF) {

        if (sizeWrite % 2 == 0) {
            file.get(symbol);
        }
        if (isalpha(symbol)) {
            symbol = ID(symbol);
        }
        if (symbol == '.' || symbol == ':' || symbol == '+' || symbol == '-' || symbol == '=' || symbol == '!' || symbol == '<' || symbol == '>' || symbol == '&' || symbol == '|' || symbol == '^') {
            symbol = Operation(symbol);
        }
        if (isdigit(symbol) || symbol == '"' || symbol == '\'') {
            symbol = Literals(symbol);
        }
        if (Check_endLex(symbol)) {
            if (sizeWrite % 2 == 0) {
                continue;
            }
        }
        sizeWrite++;
    }

    if (countForFunc != 0 || countForArray != 0) {
        error.push_back("Parentheses mismatch");
    }

    while (!error.empty()) {
        qDebug() << QString::fromStdString(error.front()) << endl;
        error.pop_front();
    }
}

char LexicalAnalysis::Literals(char symbol) {
    std::string str;
    str.push_back(symbol);

    while (true) {
        file.get(symbol);

        if (symbol == '\'' || symbol == '"' || Check_endLex(symbol)) {
            break;
        }
        str.push_back(symbol);
    }

    InitialInsert("L" + QString::number(LastNumberLit), QString::fromStdString(str), "Literal");

    return symbol;
}

bool LexicalAnalysis::Check_endLex(char symbol) {
    std::string str;
    str.push_back(symbol);

    if (Delimiters.find(str) != Delimiters.cend() || symbol == '\n' || symbol == '\t') {

        if (symbol != ' ' && symbol != '\n' && symbol != '\t' && symbol != '[' && symbol != '(') {
            std::string str;
            str.push_back(symbol);
            InitialInsert("D" + QString::number(LastNumberDel), QString::fromStdString(str), "Delimiter");
        }

        if (symbol == '[') {
            InitialInsert("O" + QString::number(LastNumberOp), QString::fromStdString(str), "Operation");
            countForArray++;
        }
        if (symbol == ']') {
            countForArray--;
        }
        if (symbol == '(') {
            InitialInsert("O" + QString::number(LastNumberOp), QString::fromStdString(str), "Operation");
            countForFunc++;
        }
        if (symbol == ')') {
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
        if (Operations.find(str) == Operations.cend()) {
            throw "Invalid operator: " + str;
        }
        InitialInsert("O" + QString::number(LastNumberOp), QString::fromStdString(str), "Operation");

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

        if (isalpha(symbol) || symbol == '_' || isdigit(symbol)/* || (symbol == '.' && !isdigit(str[str.size() - 1]))*/) {
            str.push_back(symbol);
        } else {
            break;
        }
    }
    if (Check_key_words(str)) {
        InitialInsert("X" + QString::number(LastNumberKey), QString::fromStdString(str), "Key_Word");
    } else {
        InitialInsert(QString::number(LastNumberId), QString::fromStdString(str), "ID");
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
    DB.SELECT("Code, Name, Type", "Lex");
    DB.GetLex(object);

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

