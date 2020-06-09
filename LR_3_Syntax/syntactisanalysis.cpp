#include "syntactisanalysis.h"

SyntacticAnalysis::SyntacticAnalysis() {
    DB.createConnection();

    DB.SELECT("MAX(Number) AS Max", "ParsingTable");
    number = DB.GetMaxNumber().toInt();
}

template <class Container>
void split3(const std::string& str, Container& cont, char delim = ' ')
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

QString SyntacticAnalysis::Start() {
    unsigned int Route = 1, IndexLex = 0;

    try {

        while (Route != 0) {

            QString definition = "Number = " + QString::number(Route);
            DB.SELECT("*", "ParsingTable", definition);
            RecordParsingTable State = DB.GetTerminal();

            QString Limit = QString::number(IndexLex) + ", 1";
            DB.SELECT("Code, Name, LOWER(Type) AS Type", "Lex", "", Limit);
            Lex lex = DB.GetLex();

            vector<string> Terminals;
            split3(State.Terminal, Terminals);

            cout << endl << "Lex: " << lex.Name << ", " << lex.Type;
            cout << endl << "Terminal: " << State.NumberRecord << ", " << State.Terminal;


            if (lex == Terminals) {

                if (State.Stack_In != 0) {
                    Stack.push_back(State.Stack_In);
                }

                if (State.Stack_Out) {
                    Route = Stack.front();
                    Stack.pop_front();
                } else {

                    Route = State.Route;
                }

                if (State.Accept) {
                    IndexLex++;
                }

            } else {
                throw State.ToString() + "\n" + lex.ToString();
            }
        }

    } catch (QString str) {

        return str;
    }

    return "true";
}


