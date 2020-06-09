#include "syntactisanalysis.h"

SyntacticAnalysis::SyntacticAnalysis() {
    DB = new ProjectDB("DB.db");
    if (!DB->CheckConnection()) {
        DB->createConnection();
    }

    DB->SELECT("MAX(Number) AS Max", "ParsingTable");
    number = DB->GetMaxNumber().toInt();
}

SyntacticAnalysis::SyntacticAnalysis(ProjectDB *db) {
    DB = db;
    DB->SELECT("MAX(Number) AS Max", "ParsingTable");
    number = DB->GetMaxNumber().toInt();
}

QString SyntacticAnalysis::Start() {
    unsigned int Route = 1, IndexLex = 0;

    try {

        while (Route != 0) {

            QString definition = "Number = " + QString::number(Route);
            DB->SELECT("*", "ParsingTable", definition);
            RecordParsingTable State = DB->GetAllInfoTerminal();

            QString Limit = QString::number(IndexLex) + ", 1";
            DB->SELECT("Code, Name, LOWER(Type) AS Type", "Lex", "", Limit);
            Lex lex = DB->GetLex();

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


