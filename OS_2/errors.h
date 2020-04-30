#ifndef ERRORS_H
#define ERRORS_H

#include <QDialog>
#include "lexicalanalysis.h"

namespace Ui {
class errors;
}

class errors : public QDialog
{
    Q_OBJECT

public:
    explicit errors(QWidget *parent = nullptr);
    ~errors();

public slots:
    void MyErrObject(LexicalAnalysis *); //accept LA

private slots:
    void on_pushButton_clicked();

private:
    Ui::errors *ui;
    LexicalAnalysis *object;//LA
};

#endif // ERRORS_H
