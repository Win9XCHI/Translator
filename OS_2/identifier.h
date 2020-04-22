#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QDialog>
#include "lexicalanalysis.h"
#include "mainwindow.h"

namespace Ui {
class identifier;
}

class identifier : public QDialog
{
    Q_OBJECT

public:
    explicit identifier(QWidget *parent = nullptr);
    ~identifier();

private slots:
    void on_pushButton_clicked();

private:
    Ui::identifier *ui;
};

#endif // IDENTIFIER_H
