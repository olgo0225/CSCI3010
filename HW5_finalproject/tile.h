#ifndef TILE_H
#define TILE_H
#include <QColor>

#include <QColor>
#include <QGraphicsItem>
#include <QtWidgets>

class tile: public QObject, public QGraphicsItem {

    Q_OBJECT
public:
    tile(int x, int y, QColor color, bool upgrade, int slot);
    tile() {};

    int get_x() {return x_;};
    int get_y() {return y_;};
    int get_slot() {return slot_;};

    bool get_upgrade() {return upgrade_;};

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QColor get_color() {return color_;};

private:
    int x_;
    int y_;
    QColor color_;
    bool upgrade_;
    int slot_;


};

#endif // TILE_H

class location : public tile{
public:

    location(int x, int y,QString name, int slot){x_=x; y_=y; name_=name;slot_=slot;};

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;  //override method

    QString get_name() {return name_;};
    int get_x() {return x_;};
    int get_y() {return y_;};
    int get_slot() {return slot_;};


private:
    QString name_;
    int x_;
    int y_;
    int slot_;
};


