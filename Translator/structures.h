#include <QDebug>
#include <string>

struct Lex {
    std::string Code;
    std::string Name;
    std::string Type;

    std::string operator [](int i) {
        switch (i) {
            case 0: {
                return Code;
            }
            case 1: {
                return Name;
            }
            case 2: {
                return Type;
            }
        }
        return "";
    }

    bool operator ==(std::vector<std::string> str) {
        for (unsigned int i = 0; i < str.size(); i++) {

            if (Type == str[i] || Name == str[i]) {
                return true;
            }
        }
        return false;
    }

    QString ToString() {
        return QString::fromStdString(Code) + " | " + QString::fromStdString(Name) + " | " + QString::fromStdString(Type);
    }
};

struct RecordParsingTable {
    unsigned int NumberRecord;
    std::string Terminal;
    unsigned int Route;
    bool Accept;
    unsigned int Stack_In;
    bool Stack_Out;
    bool Error;

    std::string operator [](int i) {
        switch (i) {
            case 0: {
                return std::to_string(NumberRecord);
            }
            case 1: {
                return Terminal;
            }
            case 2: {
                return std::to_string(Route);
            }
            case 3: {
                return std::to_string(Accept);
            }
            case 4: {
                return std::to_string(Stack_In);
            }
            case 5: {
                return std::to_string(Stack_Out);
            }
            case 6: {
                return std::to_string(Error);
            }
        }
        return "";
    }

    QString ToString() {
        return QString::number(NumberRecord) + " | " + QString::fromStdString(Terminal);
    }
};

