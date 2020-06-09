#include "db.h"

DB::DB(QString strBase) {
    db = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");
    db.setDatabaseName(strBase);
    size = 0;
}

DB::DB() {
}

bool DB::CheckConnection() {
    return db.isOpen();
}

bool DB::createConnection() {
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

QString DB::LastError() {
    qDebug() <<  query.lastError().text();
    return query.lastError().text();
}

int DB::GetSize() {
    return size;
}

bool DB::DeleteRecord(QString table_name, QString definition) {
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

bool DB::SELECT(QString column, QString table, QString definition, QString limit, QString Order_by) {
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

bool DB::UPDATE(QString table, QString value, QString definition) {
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

bool DB::Insert(QString table_name, std::vector<QString> listColumns, std::vector<QString> listValue) {
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

void DB::CREATE_TABLE(QString table_name, std::vector<std::vector<QString>> columns) {
    QString body;

    for (unsigned int i = 0; i < columns.size(); i++) {

        body += "'" + columns[i][0] + "' ";
        for (unsigned int n = 1; n < columns[i].size(); n++) {

            body += columns[i][n] + " ";
        }

        if (i != columns.size() - 1) {
            body += ", ";
        }
    }

    query.prepare("CREATE TABLE " + table_name + "( " + body + " );");
}

void DB::DROP(QString table_name) {
    query.prepare("DROP TABLE " + table_name + ";");
}
