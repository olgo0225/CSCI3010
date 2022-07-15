#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPainter>
#include "player.h"
#include "chutes.h"
#include "ladders.h"
#include <QTimer>
#include "playernames.h"
#include "leaderboard.h"

struct player_board{
    QString name;
    int wins;
    int games_played;

    player_board(QString name_, int wins_, int games_){
        name=name_;
        wins=wins_;
        games_played=games_;
    }

    void add_game() {games_played+=1;};
    void add_win() {wins+=1;};
    QString get_name(){return name;};

    int get_wins() {return wins;};
    int get_games() {return games_played;};

    int get_percent_wins() {return ((wins*100)/games_played);};



};



struct slot_{
    int number;
    int pos_x;
    int pos_y;
    slot_(int numb, int x, int y) {number=numb; pos_x=x; pos_y=y;};

    int get_numb(){return number;};
    int get_x() {return pos_x;};
    int get_y() {return pos_y;};

};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Player * get_player_undo(QString user);
    void move_helper();
    QVector <chutes*> get_chutes() {return chutes_;};
    void timer_();
    void get_player();
    chutes* get_chute();
    ladders* get_ladder();
    bool is_end();
    void new_game();
    int get_slot_x(int slot);
    int get_slot_y(int slot);
    void can_undo();
    void set_up_leader();

private slots:

    void on_Start_clicked();

    void get_player_timer();

    void on_numb_play_button_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_2_clicked();

    void updateCountdown();

    void updateBoard();

    void AddPlayers();

    void on_next_player_clicked();

    void on_move_clicked();

    void updateBoardUndo();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int numb_players;
    QVector<Player*> players_;
    Player * current_player;
    QVector<chutes*> chutes_;
    QVector<ladders*> ladders_;
    QVector<slot_*> grid;
    QTimer* timer;
    int timer_count;
    PlayerNames * play;
    QTimer * player;
    int player_timer;
    QTimer * player_undo;
    int player_timer_undo;
    QGraphicsScene *scene1;
    Leaderboard * l1;
    bool game_ended;
    QVector<player_board> player_history;



};
#endif // MAINWINDOW_H

