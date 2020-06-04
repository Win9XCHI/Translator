#include "projectbd.h"

ProjectBD::ProjectBD() {
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    db.setDatabaseName("DB.db");
    size = 0;
}


bool ProjectBD::createConnection() {
    bool connected = db.open();

    if (!connected) {
        qDebug() << db.lastError().text();
        throw "System error: Not connect to database";
    }
    else {
        query = QSqlQuery( db );
    }
    return connected;
}

QString ProjectBD::LastError() {
    qDebug() <<  query.lastError().text();
    return query.lastError().text();
}

int ProjectBD::GetSize() {
    return size;
}

bool ProjectBD::DeleteRecord(QString table_name, QString definition) {
    bool flag = true;

    if (table_name != "") {
        if (definition != "") {
            query.prepare("DELETE FROM " + table_name + " WHERE " + definition + ";");
        }
        else {
            query.prepare("DELETE FROM " + table_name + ";");
        }
        flag = query.exec();
    }
    else {
        flag = false;
    }

    return flag;
}

bool ProjectBD::SELECT(QString column, QString table, QString definition, QString limit, QString Order_by) {
    bool flag = true;

    if (table != "" ) {

            if (limit == "") {

                if (Order_by == "") {
                    if (definition == "") {
                        query.prepare("SELECT " + column + " FROM " + table + " ;");
                    }
                    else {
                        query.prepare("SELECT " + column + " FROM " + table + " WHERE " + definition + " ;");
                    }
                }
                else {
                    if (definition == "") {
                        query.prepare("SELECT " + column + " FROM " + table + " ORDER BY " + Order_by + ";");
                    }
                    else {
                        query.prepare("SELECT " + column + " FROM " + table + " WHERE " + definition + " ORDER BY " + Order_by + ";");
                    }
                }
            }
            else {
                if (Order_by == "") {
                    if (definition == "") {
                        query.prepare("SELECT " + column + " FROM " + table + " LIMIT " + limit + " ;");
                    }
                    else {
                        query.prepare("SELECT " + column + " FROM " + table + " WHERE " + definition + " LIMIT " + limit + ";");
                    }
                }
                else {
                    if (definition == "") {
                        query.prepare("SELECT " + column + " FROM " + table + " LIMIT " + limit + " ORDER BY " + Order_by + ";");
                    }
                    else {
                        query.prepare("SELECT " + column + " FROM " + table + " WHERE " + definition + " LIMIT " + limit + " ORDER BY " + Order_by + ";");
                    }
                }
            }
            flag = query.exec();
            size = 0;
            while (query.next()) size++;
            while (query.previous()) {

            }
        }
        else {
            flag = false;
        }

        return flag;
}

bool ProjectBD::UPDATE(QString table, QString value, QString definition) {
    bool flag = true;

    if (table != "" && value != "") {

        if (definition != "") {
            query.prepare("UPDATE " + table + " SET " + value + " WHERE " + definition + ";");
        }
        else {
            query.prepare("UPDATE " + table + " SET " + value + ";");
        }
        flag = query.exec();
    }
    else {
        flag = false;
    }

    return flag;
}

std::map<std::string, std::string> ProjectBD::GetReserveLex() {
    QSqlRecord rec = query.record();
    std::map<std::string, std::string> buff;

    while(query.next()) {
        buff[query.value(rec.indexOf("Name")).toString().toStdString()] = query.value(rec.indexOf("Type")).toString().toStdString();
    }
    return buff;
}

void ProjectBD::GetLex(std::list<Lex> &cont) {
    QSqlRecord rec = query.record();
    Lex object;

    while(query.next()) {
        object.Code = query.value(rec.indexOf("Code")).toString().toStdString();
        object.Name = query.value(rec.indexOf("Name")).toString().toStdString();
        object.Type = query.value(rec.indexOf("Type")).toString().toStdString();

        cont.push_back(object);
    }
}

void ProjectBD::GetID(std::list<Lex> &cont) {
    QSqlRecord rec = query.record();
    Lex object;

    while(query.next()) {
        object.Code = query.value(rec.indexOf("Code")).toString().toStdString();
        object.Name = query.value(rec.indexOf("Name")).toString().toStdString();
        object.Type = query.value(rec.indexOf("Key")).toString().toStdString();

        cont.push_back(object);
    }
}

QString ProjectBD::Check_Name() {
    QSqlRecord rec = query.record();
    query.next();
    return query.value(rec.indexOf("Code")).toString();
}

bool ProjectBD::Insert(QString table_name, std::vector<QString> listColumns, std::vector<QString> listValue) {
    bool flag = true;

    if (table_name != "" && !listValue.empty() && !listColumns.empty()) {
        QString Columns = "";
        QString CValue = "";

        for (unsigned int i = 0; i < listColumns.size(); i++) {

            if (i == listColumns.size() - 1) {

                Columns += listColumns.at(i);
                listColumns.at(i).push_front(':');
                CValue += listColumns.at(i);

            } else {
                Columns += listColumns.at(i) + ", ";
                listColumns.at(i).push_front(':');
                CValue += listColumns.at(i)  + ", ";
            }
        }

        query.prepare("INSERT INTO " +  table_name + " (" + Columns + ") "
                          "VALUES (" + CValue + ");");

        for (unsigned int i = 0; i < listValue.size(); i++) {
            query.bindValue(listColumns.at(i), listValue.at(i));
        }

        flag = query.exec();

    } else {
        flag = false;
    }

    return flag;
}

void ProjectBD::CREATE_TABLE(QString table_name, std::vector<std::vector<QString>> columns) {
    QString body;

    for (unsigned int i = 0; i < columns.size(); i++) {

        for (unsigned int n = 0; n < columns[i].size(); n++) {

            body += columns[i][n] + " ";
        }

        if (i != columns.size() - 1) {
            body += ", ";
        }
    }

    query.prepare("CREATE TABLE " + table_name + "( " + body + " );");
}



