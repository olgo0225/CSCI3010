#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <card.h>
#include <tile.h>
#include <player.h>
#include <instructions.h>
#include <winner.h>
#include <QMap>


struct cards{
    cards() {};
    cards(QColor color, bool upgrade) {color_=color; upgrade_=upgrade;};

        bool get_upgrade() {return upgrade_;};

        QColor get_color() {return color_;};

        virtual QString get_name() {return "";};

    QColor color_;
    bool upgrade_;
};

struct grid{
    int slot;
    bool upgrade_;
    int x;
    int y;
    QColor color;
public:
    grid(){};
    grid(int x_, int y_, int slot_, QColor color_, bool upgrade) {x=x_; y=y_; slot=slot_; color=color_; upgrade_=upgrade;};
    int get_x() {return x;};
    int get_y() {return y;};
    QColor get_color() {return color;};
    bool get_upgrade() {return upgrade_;};
    int get_slot() {return slot;};

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
    void get_player();
    void new_game();
    bool is_end();

private slots:
    void on_draw_card_clicked();

    void on_start_clicked();

    void on_submit_clicked();

    void on_skip_turn_clicked();

    void on_make_move_clicked();

    void on_new_game_clicked();

    void on_move_5_clicked();

    void on_end_game_clicked();

    void on_instructions_clicked();

    void on_quit_clicked();

    void move_helper();

private:
    Ui::MainWindow *ui;
    QGraphicsScene * game_scene;
    QGraphicsScene * card_scene;
    QGraphicsScene * player_scene;
    QVector <location_card*> deck_location;
    QVector <tile *> tiles;
    int number_players;
    QVector <player *> players;
    player * current_player;
    cards current_card;
    location_card * current_location_card;
    QVector <grid> grid_;
    QVector <location*> locations_;
    instructions * instructions_;
    winner * w1;
    QVector <cards> deck2;
    QTimer* timer;
    int timer_count;

};
#endif // MAINWINDOW_H
