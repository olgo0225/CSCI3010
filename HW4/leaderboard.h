#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QDialog>
#include <QFile>

namespace Ui {
class Leaderboard;
}

class Leaderboard : public QDialog
{
    Q_OBJECT

public:
    explicit Leaderboard(QWidget *parent = nullptr);
    ~Leaderboard();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Leaderboard *ui;
    QString s1;
};

#endif // LEADERBOARD_H
