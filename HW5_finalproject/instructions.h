#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QDialog>

namespace Ui {
class instructions;
}

class instructions : public QDialog
{
    Q_OBJECT

public:
    explicit instructions(QWidget *parent = nullptr);
    ~instructions();

private slots:
    void on_pushButton_clicked();

private:
    Ui::instructions *ui;
};

#endif // INSTRUCTIONS_H
