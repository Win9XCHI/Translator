#include "errors.h"
#include "ui_errors.h"

errors::errors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errors)
{
    ui->setupUi(this);

    LexicalAnalysis object;

    std::list<std::string> err = object.GetError();

    ui->tableWidget->setRowCount(err.size());
    ui->tableWidget->setColumnCount(1);

    QTableWidgetItem *item = new QTableWidgetItem();

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        item->setText(QString::fromStdString(err.front()));
        ui->tableWidget->setItem(i,0,item);

        err.pop_front();
    }
}

errors::~errors()
{
    delete ui;
}
