#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    DB = new ProjectDB("DB.db");

    if (!DB->CheckConnection()) {
        DB->createConnection();
    }

    objectLex = new LexicalAnalysis(DB);
    objectGram = new ModuleAnalysis_LLGram(DB);
    objectSyntac = new SyntacticAnalysis(DB);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked() {
    QFile file(ui->lineEdit->text());

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(ui->textEdit->toPlainText().toUtf8());
        file.close();
    }
}

void MainWindow::on_pushButton_2_clicked() {
    QFileDialog dialog;
    QStringList fileNames;

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        ui->lineEdit->setText(fileNames[0]);

        QFile file(fileNames[0]);
        fileName = fileNames[0];
        objectLex->Start(fileNames[0].toStdString());

        if (file.open(QIODevice::ReadOnly))
        {
            ui->textEdit->setPlainText(QString::fromUtf8(file.readAll()));
        }
        file.close();
    }
}

void MainWindow::on_pushButton_3_clicked() {
    std::list<Lex> listLex = objectLex->GetLexical();

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

    listLex.clear();
    listLex = objectLex->GetID();

    ui->tableWidget_2->setRowCount(listLex.size());
    ui->tableWidget_2->setColumnCount(3);

    ui->tableWidget_2->horizontalHeaderItem(0)->setText("Code");
    ui->tableWidget_2->horizontalHeaderItem(1)->setText("Name");
    ui->tableWidget_2->horizontalHeaderItem(2)->setText("Key");

    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++) {
        for (int n = 0; n < ui->tableWidget_2->columnCount(); n++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(QString::fromStdString(listLex.front()[n])); //Lex has overload operator [] for 0 - Code, 1 - Name, 2 - Type
                    ui->tableWidget_2->setItem(i,n,item);
        }

        listLex.pop_front();
    }

    std::list<std::string> err = objectLex->GetError();

    ui->tableWidget_3->setRowCount(err.size());
    ui->tableWidget_3->setColumnCount(1);

    for (int i = 0; i < ui->tableWidget_3->rowCount(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem(); //single for one cell tables
        item->setText(QString::fromStdString(err.front())); //Lex has overload operator [] for 0 - Code, 1 - Name, 2 - Type
        ui->tableWidget_3->setItem(i,0,item);

        err.pop_front();
    }
}

void MainWindow::on_pushButton_4_clicked() {
    QFileDialog dialog;
    QStringList fileNames;

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        ui->lineEdit_2->setText(fileNames[0]);

        QFile file(fileNames[0]);
        objectGram->UploadGrammar(fileNames[0].toStdString());

        if (file.open(QIODevice::ReadOnly))
        {
            ui->textEdit_2->setPlainText(QString::fromUtf8(file.readAll()));
        }
        file.close();
    }
}

void MainWindow::on_pushButton_6_clicked() {
    ui->textEdit_3->setPlainText(QString::number(objectGram->CheckGrammar()));
}

void MainWindow::on_pushButton_7_clicked() {
    objectGram->ParsingTable();

    std::list<RecordParsingTable> listTerm = objectGram->GetTable();

    ui->tableWidget_4->setRowCount(listTerm.size());
    ui->tableWidget_4->setColumnCount(3);

    for (int i = 0; i < ui->tableWidget_4->rowCount(); i++) {
        for (int n = 0; n < ui->tableWidget_4->columnCount(); n++) {
            QTableWidgetItem *item = new QTableWidgetItem(); //single for one cell tables
            item->setText(QString::fromStdString(listTerm.front()[n]));
            ui->tableWidget_4->setItem(i,n,item);
        }

        listTerm.pop_front();
    }
}

void MainWindow::on_pushButton_8_clicked() {
    ui->textEdit_3->setPlainText(objectSyntac->Start());
}

void MainWindow::on_textEdit_textChanged()
{
    on_pushButton_clicked();
    objectLex->Start(fileName.toStdString());
}
