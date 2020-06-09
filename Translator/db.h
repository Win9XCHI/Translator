#ifndef DB_H
#define DB_H

#include <QtSql>
#include <QDebug>

class DB {
protected:
    QSqlDatabase db;
    QSqlQuery query;

    int size;

public:
    DB(QString);
    DB();
    virtual ~DB() {}

    //Create database connection
    //Input: -
    //Output: Created or not
    bool createConnection();

    bool CheckConnection();

    //Return last error in database
    //Input: -
    //Output: string with error
    QString LastError();

    //Return size returned records after query SELECT
    //Input: -
    //Output: size
    int GetSize();

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
    void DROP(QString);

};

#endif // DB_H
