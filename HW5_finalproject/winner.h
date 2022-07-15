#ifndef WINNER_H
#define WINNER_H

#include <QDialog>

namespace Ui {
class winner;
}

class winner : public QDialog
{
    Q_OBJECT

public:
    explicit winner(QWidget *parent = nullptr);
    void set_player(QString player);
    ~winner();

private slots:
    void on_quit_clicked();

    void on_new_game_clicked();

signals:
    void quit();
    void new_game();

private:
    Ui::winner *ui;
    int player;
};

#endif // WINNER_H
