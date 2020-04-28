#include "identifier.h"
#include "ui_identifier.h"

identifier::identifier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::identifier)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

identifier::~identifier()
{
    delete ui;
}

void identifier::MyIdObject(LexicalAnalysis *object){ //object acceptance signal
    this->object = object;
}

void identifier::on_pushButton_clicked()
{
    std::list<Lex> listLex = object->GetID();

    ui->tableWidget->setRowCount(listLex.size());
    ui->tableWidget->setColumnCount(3);

    ui->tableWidget->horizontalHeaderItem(0)->setText("Code");
    ui->tableWidget->horizontalHeaderItem(1)->setText("Name");
    ui->tableWidget->horizontalHeaderItem(2)->setText("Type");

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int n = 0; n < ui->tableWidget->columnCount(); n++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(QString::fromStdString(listLex.front()[n])); //Lex has overload operator [] for 0 - Code, 1 - Name, 2 - Type
                    ui->tableWidget->setItem(i,n,item);
        }

        listLex.pop_front();
    }
}
