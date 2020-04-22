#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lexanalys.h"
#include "identifier.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    QFileDialog dialog;
    QStringList fileNames;

    //LexicalAnalysis Object;
    //Object.Start("filename");

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        ui->textEdit_2->setText(fileNames[0]);

        QFile file(fileNames[0]);

        if (file.open(QIODevice::ReadOnly))
        {
            ui->textEdit->setPlainText(QString::fromUtf8(file.readAll()));
        }
        file.close();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QFile file(ui->textEdit_2->toPlainText());

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(ui->textEdit->toPlainText().toUtf8());
        file.close();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    //LexicalAnalysis object;
    object.Start("C:/Qt Labs/OS_2/Example.txt");
    lexanalys *tmp = new lexanalys();
    tmp->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    identifier *id = new identifier();
    id->show();
}

void MainWindow::on_pushButton_5_clicked()
{
    errors *err = new errors();
    err->show();
}
