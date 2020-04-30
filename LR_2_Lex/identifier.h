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

public slots:
    void MyIdObject(LexicalAnalysis *);

private:
    Ui::identifier *ui;
    LexicalAnalysis *object; //LA
};

#endif // IDENTIFIER_H