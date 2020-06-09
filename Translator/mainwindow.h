#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QStringList>
#include <QStringList>
#include <QTextDocumentWriter>
#include <QTextEdit>
#include <QTextStream>
#include "lexicalanalysis.h"
#include "moduleanalysis_llgram.h"
#include "syntactisanalysis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;

    QString fileName;
    ProjectDB *DB;
    LexicalAnalysis *objectLex;
    ModuleAnalysis_LLGram *objectGram;
    SyntacticAnalysis *objectSyntac;
};
#endif // MAINWINDOW_H
