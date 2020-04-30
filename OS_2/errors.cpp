#include "errors.h"
#include "ui_errors.h"

void errors::MyErrObject(LexicalAnalysis *object){ //object acceptance signal
    this->object = object;
}

errors::errors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errors)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

errors::~errors()
{
    delete ui;
}

void errors::on_pushButton_clicked()
{
    std::list<std::string> err = object->GetError();

    ui->tableWidget->setRowCount(err.size());
    ui->tableWidget->setColumnCount(1);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem(); //single for one cell tables
        item->setText(QString::fromStdString(err.front())); //Lex has overload operator [] for 0 - Code, 1 - Name, 2 - Type
        ui->tableWidget->setItem(i,0,item);

        err.pop_front();
    }
}
