#ifndef PLAYERNAMES_H
#define PLAYERNAMES_H

#include <QDialog>

namespace Ui {
class PlayerNames;
}

class PlayerNames : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerNames(QWidget *parent = nullptr, int n=0);
    ~PlayerNames();
    void addWidgets();
    void set_players(int numb) {numb_players=numb;};
    QVector <QString> get_names() {return names;};


private slots:
    void on_submit_clicked();
signals:
    void names_added();

private:
    Ui::PlayerNames *ui;
    int numb_players=0;
    QVector <QString> names;
};

#endif // PLAYERNAMES_H
