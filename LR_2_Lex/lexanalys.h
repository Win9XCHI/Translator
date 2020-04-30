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

public slots:
    void MyObject(LexicalAnalysis *);

private:
    Ui::lexanalys *ui;
    LexicalAnalysis *object; //LA
};

#endif // LEXANALYS_H