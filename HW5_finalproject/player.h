#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
#include <QColor>
#include <QGraphicsItem>

class player: public QObject, public QGraphicsItem {

    Q_OBJECT
public:
    player(QColor color, int user, QString name_);  // constructor

    QColor get_color() {return color_;};

    int get_user() {return user_;};
    QColor get_pawn() {return color_;};
    bool get_active() {return active;};
    void set_active(bool active_) {active=active_;};
    int get_skips() {return skips;};
    int get_moves() {return move_5;};

    void decrease_skips() {skips-=1;};
    void decrease_moves() {move_5=-1;};

    void set_user(int user1) {user_=user1;};

    void increase_moves() {move_5+=1;};
    void increase_skips() {skips+=1;};

    void set_slot(int slot) {slot_=slot;};
    int get_slot() {return slot_;};

    int get_x() {return x_;};
    int get_y() {return y_;};

    void set_x(int x) {x_=x;};
    void set_y(int y) {y_=y;};

    QString get_name() {return name;};

    static int get_width() {return width_;};

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;


private:
  int x_;
  int y_;
  QColor color_;
  int user_;
  bool active;
  int skips;
  int move_5;
  int slot_;
  QString name;
  static const int width_ = 20;

};

#endif // PLAYER_H
