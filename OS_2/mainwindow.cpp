#include "mainwindow.h"
#include "ui_mainwindow.h"


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

    LexicalAnalysis Object;
    Object.Start("filename");

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
