#ifndef CARD_H
#define CARD_H
#include <QColor>
#include <QGraphicsItem>
#include <QtWidgets>

class card : public QObject, public QGraphicsItem {

   Q_OBJECT
public:
    card() {};
    card(QColor color, bool upgrade);

    bool get_upgrade() {return upgrade_;};

    QColor get_color() {return color_;};

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    virtual bool get_location() {return false;};
    virtual QString get_name() {return "";};

private:
QColor color_;
bool upgrade_;

};


#endif // CARD_H


class location_card : public card{
public:
    location_card(){};

    static location_card* factory(QString type); //FACTORY DESIGN METHOD

};

class triangle:public location_card{
public:
    triangle(){};
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QString get_name() override {return "triangle";};

    bool get_location() override {return true;};

};

class square:public location_card{
public:
    square(){};
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QString get_name() override {return "square";};

    bool get_location() override {return true;};

};

class hexagon:public location_card{
public:
    hexagon(){};
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QString get_name() override {return "hexagon";};

    bool get_location() override {return true;};

};

class pentagon:public location_card{
public:
    pentagon(){};
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QString get_name() override {return "pentagon";};

    bool get_location() override {return true;};

};
