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

private:
    Ui::errors *ui;
};

#endif // ERRORS_H
