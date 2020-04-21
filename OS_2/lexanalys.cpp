#include "lexanalys.h"
#include "ui_lexanalys.h"
#include "lexicalanalysis.h"

lexanalys::lexanalys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lexanalys)
{
    ui->setupUi(this);

//    LexicalAnalysis object;

    //    ui->tableWidget->setRowCount(1);
    //    ui->tableWidget->setColumnCount(3);

    //    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
    //    {
    //        std::list<Lex> listLex = object.GetLexical();
    //        QTableWidgetItem *itm = new QTableWidgetItem(tr("%1").arg(listLex.front()); //listLex.front());
    //        ui->tableWidget->setItem(0, i, itm);
    //    }

//         std::list<Lex> listLex = object.GetLexical();

//         ui->tableWidget->setRowCount(listLex.size());
//         ui->tableWidget->setColumnCount(3);

//         QTableWidgetItem *itm = new QTableWidgetItem();
//        // QString tmp = QString::fromStdString(listLex.front().Code);
//         //for(int i = 0; i < 3; i++) {
//             for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
//                 itm->setText(QString::fromStdString(listLex.front().Code));
//                 ui->tableWidget->setItem(j,0,itm);
//                 itm->setText(QString::fromStdString(listLex.front().Name));
//                 ui->tableWidget->setItem(j,1,itm);
//                 itm->setText(QString::fromStdString(listLex.front().Type));
//                 ui->tableWidget->setItem(j,2,itm);
//             }
         //}


    //     QTableWidgetItem *itm_code = new QTableWidgetItem();
    //     QTableWidgetItem *itm_name = new QTableWidgetItem();
    //     QTableWidgetItem *itm_type = new QTableWidgetItem();

    //     itm_code->setText(listLex.Code);
    //     itm_name->setText(listLex.Name);
    //     itm_type->setText(listLex.Type);

    //     int lastRow = ui->tableWidget->rowCount();
    //     ui->tableWidget->insertRow(lastRow);

    //     ui->tableWidget->setItem(lastRow,0,itm_code);
    //     ui->tableWidget->setItem(lastRow,1,itm_name);
    //     ui->tableWidget->setItem(lastRow,2,itm_type);
}

lexanalys::~lexanalys()
{
    delete ui;
}

void lexanalys::on_pushButton_clicked()
{
    LexicalAnalysis object;

         std::list<Lex> listLex = object.GetLexical();

         ui->tableWidget->setRowCount(listLex.size());
         ui->tableWidget->setColumnCount(3);

         QTableWidgetItem *itm = new QTableWidgetItem();

             for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
                 itm->setText(QString::fromStdString(listLex.front().Code));
                 ui->tableWidget->setItem(j,0,itm);
                 itm->setText(QString::fromStdString(listLex.front().Name));
                 ui->tableWidget->setItem(j,1,itm);
                 itm->setText(QString::fromStdString(listLex.front().Type));
                 ui->tableWidget->setItem(j,2,itm);
             }
}
