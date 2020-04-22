#ifndef LEXANALYS_H
#define LEXANALYS_H

#include <QDialog>
#include "lexicalanalysis.h"
#include "mainwindow.h"

namespace Ui {
class lexanalys;
}

class lexanalys : public QDialog
{
    Q_OBJECT

public:
    explicit lexanalys(QWidget *parent = nullptr);
    ~lexanalys();

private slots:
    void on_pushButton_clicked();

private:
    Ui::lexanalys *ui;
};

#endif // LEXANALYS_H
