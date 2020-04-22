#include "lexanalys.h"
#include "ui_lexanalys.h"

lexanalys::lexanalys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lexanalys)
{
    ui->setupUi(this);
}

lexanalys::~lexanalys()
{
    delete ui;
}

void lexanalys::on_pushButton_clicked()
{
    LexicalAnalysis object;
    object.Start("C:/Qt Labs/OS_2/Example.txt");

    std::list<Lex> listLex = object.GetLexical();

    ui->tableWidget->setRowCount(listLex.size());
    ui->tableWidget->setColumnCount(3);

    QTableWidgetItem *item = new QTableWidgetItem();

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        item->setText(QString::fromStdString(listLex.front().Code));
        ui->tableWidget->setItem(i,0,item);

        item->setText(QString::fromStdString(listLex.front().Name));
        ui->tableWidget->setItem(i,1,item);

        item->setText(QString::fromStdString(listLex.front().Type));
        ui->tableWidget->setItem(i,2,item);

        listLex.pop_front();
    }
}
