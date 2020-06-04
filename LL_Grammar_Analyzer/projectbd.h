#ifndef PROJECTBD_H
#define PROJECTBD_H

#include <QtSql>
#include <QDebug>
#include <map>
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
};

class ProjectBD {
private:
    QSqlDatabase db;
    QSqlQuery query;

    int size;

public:
    ProjectBD();
    virtual ~ProjectBD() {}

    //Create database connection
    //Input: -
    //Output: Created or not
    bool createConnection();

    //Return last error in database
    //Input: -
    //Output: string with error
    QString LastError();

    //Return size returned records after query SELECT
    //Input: -
    //Output: size
    int GetSize();

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

    //Delete records in table
    //Input: table name, definition
    //Output: Query done or not
    bool DeleteRecord(QString, QString = "");

    //Execute query SELECT
    //Input: columns, table name, definition, limit, order by
    //Output: Query done or not
    bool SELECT(QString, QString, QString = "", QString = "", QString = "");

    //Execute query UPDATE
    //Input: table name, values, definition
    //Output: Query done or not
    bool UPDATE(QString, QString, QString);

    //Insert record in table
    //Input: table name, columns, values
    //Output: Query done or not
    bool Insert(QString, std::vector<QString>, std::vector<QString>);

    //Create new table
    //Input: table name, array: name, type and rule for columns
    //Output: -
    void CREATE_TABLE(QString, std::vector<std::vector<QString>>);
};

#endif // PROJECTBD_H
