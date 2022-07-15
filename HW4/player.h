#ifndef PLAYER_H
#define PLAYER_H

#include <QColor>
#include <QGraphicsItem>

class Player : public QObject, public QGraphicsItem {

   Q_OBJECT

public:
    Player(QColor color, QString user, QString name_);  // constructor

    QString get_user() {return user_;};
    QColor get_pawn() {return color_;};
    bool get_active() {return active;};
    void set_active(bool active_) {active=active_;};
    int get_rerolls() {return rerolls;};
    int get_undos() {return undos;};

    void set_user(QString user1) {user_=user1;};

    void increase_undos() {undos+=1;};
    void increase_rerolls() {rerolls+=1;};

    int get_roll() {return roll;};
    void set_roll(int roll_) {roll=roll_;};

    int get_x() { return x_; }
    int get_y() { return y_; }

    void set_x(int x) {x_=x;};
    void set_y(int y) {y_=y;};

    float get_percent_wins() {return ((wins/games_played)*100);};
    void add_win() {wins+=1;};
    void add_game() {games_played+=1;};

    bool get_ladder() {return moved_ladder;};
    bool get_chute() {return moved_chute;};

    void set_ladder(bool lad) {moved_ladder=lad;};
    void set_chute(bool chut) {moved_chute=chut;};

    int get_games() {return games_played;};
    int get_wins() {return wins;};

    QString get_name() {return name;};


    void player_moved() {emit PlayerMoved(this);}

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; }

signals:
    void PlayerMoved(Player *p);

private:
  int x_;
  int y_;
  QColor color_;
  QString user_;
  bool active;
  int rerolls;
  int undos;
  int roll;
  int wins;
  int games_played;
  QString name;
  static const int width_ = 20;
  bool moved_ladder;
  bool moved_chute;

};  // class Player



#endif // PlAYER_H
