#include "lexanalys.h"
#include "ui_lexanalys.h"

lexanalys::lexanalys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lexanalys)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

lexanalys::~lexanalys()
{
    delete ui;
}

void lexanalys::MyObject(LexicalAnalysis *object){ //object acceptance signal
    this->object = object;
}

void lexanalys::on_pushButton_clicked()
{
    std::list<Lex> listLex = object->GetLexical();

    ui->tableWidget->setRowCount(listLex.size());
    ui->tableWidget->setColumnCount(3);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int n = 0; n < ui->tableWidget->columnCount(); n++) {
            QTableWidgetItem *item = new QTableWidgetItem(); //single for one cell tables
            item->setText(QString::fromStdString(listLex.front()[n])); //Lex has overload operator [] for 0 - Code, 1 - Name, 2 - Type
            ui->tableWidget->setItem(i,n,item);
        }

        listLex.pop_front();
    }
}
