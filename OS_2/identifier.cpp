#include "identifier.h"
#include "ui_identifier.h"

identifier::identifier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::identifier)
{
    ui->setupUi(this);
}

identifier::~identifier()
{
    delete ui;
}

void identifier::on_pushButton_clicked()
{
    LexicalAnalysis object;
    object.Start("C:/Qt Labs/OS_2/Example.txt");

    std::list<Lex> listLex = object.GetID();

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
