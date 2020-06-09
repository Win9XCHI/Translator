#include "projectdb.h"

ProjectDB::ProjectDB(QString str) : DB(str) {
}

ProjectDB::ProjectDB() : DB() {
}

std::map<std::string, std::string> ProjectDB::GetReserveLex() {
    QSqlRecord rec = query.record();
    std::map<std::string, std::string> buff;

    while(query.next()) {
        buff[query.value(rec.indexOf("Name")).toString().toStdString()] = query.value(rec.indexOf("Type")).toString().toStdString();
    }
    return buff;
}

void ProjectDB::GetLex(std::list<Lex> &cont) {
    QSqlRecord rec = query.record();
    Lex object;

    while(query.next()) {
        object.Code = query.value(rec.indexOf("Code")).toString().toStdString();
        object.Name = query.value(rec.indexOf("Name")).toString().toStdString();
        object.Type = query.value(rec.indexOf("Type")).toString().toStdString();

        cont.push_back(object);
    }
}

void ProjectDB::GetID(std::list<Lex> &cont) {
    QSqlRecord rec = query.record();
    Lex object;

    while(query.next()) {
        object.Code = query.value(rec.indexOf("Code")).toString().toStdString();
        object.Name = query.value(rec.indexOf("Name")).toString().toStdString();
        object.Type = query.value(rec.indexOf("Key")).toString().toStdString();

        cont.push_back(object);
    }
}

QString ProjectDB::Check_Name() {
    QSqlRecord rec = query.record();
    query.next();
    return query.value(rec.indexOf("Code")).toString();
}

QString ProjectDB::GetMaxNumber() {
    QSqlRecord rec = query.record();
    query.next();
    return query.value(rec.indexOf("Max")).toString();
}

Lex ProjectDB::GetLex() {
    QSqlRecord rec = query.record();
    Lex object;

    query.next();
    object.Code = query.value(rec.indexOf("Code")).toString().toStdString();
    object.Name = query.value(rec.indexOf("Name")).toString().toStdString();
    object.Type = query.value(rec.indexOf("Type")).toString().toStdString();

    return object;
}

RecordParsingTable ProjectDB::GetAllInfoTerminal() {
    QSqlRecord rec = query.record();
    RecordParsingTable object;

    query.next();
    object.NumberRecord = query.value(rec.indexOf("Number")).toString().toInt();
    object.Terminal = query.value(rec.indexOf("Terminal")).toString().toStdString();
    object.Route = query.value(rec.indexOf("Route")).toString().toStdString();
    object.Accept = query.value(rec.indexOf("Accept")).toString().toInt();
    object.Stack_In = query.value(rec.indexOf("Stack_In")).toString().toInt();
    object.Stack_Out = query.value(rec.indexOf("Stack_Out")).toString().toInt();
    object.Error = query.value(rec.indexOf("Error")).toString().toInt();

    return object;
}

void ProjectDB::GetStackNumber(std::vector<int> &cont) {
    QSqlRecord rec = query.record();

    while(query.next()) {
        int number = query.value(rec.indexOf("Stack_In")).toInt();

        if (number != 0) {
            cont.push_back(number);
        }
    }
}

QString ProjectDB::GetTerminal() {
    QSqlRecord rec = query.record();
    query.next();
    return query.value(rec.indexOf("Terminal")).toString();
}

void ProjectDB::GetTerm(std::list<RecordParsingTable> &cont) {
    QSqlRecord rec = query.record();
    RecordParsingTable object;

    while(query.next()) {
        object.NumberRecord = query.value(rec.indexOf("Number")).toString().toInt();
        object.Terminal = query.value(rec.indexOf("Terminal")).toString().toStdString();
        object.Route = query.value(rec.indexOf("Route")).toString().toStdString();
        object.Accept = query.value(rec.indexOf("Accept")).toString().toInt();
        object.Stack_In = query.value(rec.indexOf("Stack_In")).toString().toInt();
        object.Stack_Out = query.value(rec.indexOf("Stack_Out")).toString().toInt();
        object.Error = query.value(rec.indexOf("Error")).toString().toInt();

        cont.push_back(object);
    }
}

