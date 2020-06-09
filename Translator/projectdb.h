#ifndef PROJECTDB_H
#define PROJECTDB_H

#include <QtSql>
#include <QDebug>
#include <map>
#include <string>
#include "db.h"
#include "structures.h"

class ProjectDB : public DB {

public:
    ProjectDB(QString);
    ProjectDB();
    virtual ~ProjectDB() {}

    //Return records from table "ReserveLex"
    //Input: -
    //Output: map with records
    std::map<std::string, std::string> GetReserveLex();

    //Return records from table "Lex"
    //Input: container for save records
    //Output: container for save records
    void GetLex(std::list<Lex>&);

    //Return records from table "ID"
    //Input: container for save records
    //Output: container for save records
    void GetID(std::list<Lex>&);

    //Return record from table "Lex"
    //Input: -
    //Output: string with code
    QString Check_Name();

    QString GetMaxNumber();
    Lex GetLex();
    RecordParsingTable GetAllInfoTerminal();

    void GetStackNumber(std::vector<int> &);
    QString GetTerminal();

    void GetTerm(std::list<RecordParsingTable>&);

};

#endif // PROJECTBD_H
