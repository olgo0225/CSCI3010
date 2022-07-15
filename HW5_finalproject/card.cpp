#include "card.h"

card::card(QColor color, bool upgrade)  //constructor
{
 color_=color;
 upgrade_=upgrade;

}

QRectF card::boundingRect() const
{
    return QRectF(20, 20, 50, 50);
}

void card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));

    painter->drawRect(QRect(20, 20, 50, 50));
    painter->setBrush(b);

}


void triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    QColor color(255,0,0);
    painter->setBrush(QBrush(color));
    QPolygonF triangle;
    triangle << QPointF(20, 20+25) << QPointF(20+50,20) <<  QPointF(20+50, 20+50);
    painter->drawPolygon(triangle); //draws triangle
    painter->setBrush(b);

}

QRectF triangle::boundingRect() const{
    return QRectF(20, 20, 50, 50);
}

void square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    QColor color(255,0,255);
    painter->setBrush(QBrush(color));
    QPolygonF square;
    square << QPointF(20, 20) << QPointF(20+50, 20) <<  QPointF(20+50, 20+50) << QPointF(20, 20+50);
    painter->drawPolygon(square);  //draws square
    painter->setBrush(b);
}


QRectF square::boundingRect() const{
    return QRectF(20, 20, 50, 50);
}

void hexagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    QColor color(255,255,0);
    painter->setBrush(QBrush(color));
    QPolygonF hexagon;
    hexagon << QPointF(20, 20+25) << QPointF(20+15, 20) <<  QPointF(20+35, 20) << QPointF(20+50, 20+25) << QPointF(20+35, 20+50) << QPointF(20+15, 20+50);
    painter->drawPolygon(hexagon);  //draws hexagon
    painter->setBrush(b);
}


QRectF hexagon::boundingRect() const{
    return QRectF(20, 20, 50, 50);
}

void pentagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    QColor color(0,255,0);
    painter->setBrush(QBrush(color));
    QPolygonF pentagon;
    pentagon << QPointF(20, 20+25) << QPointF(20+25, 20) <<  QPointF(20+50, 20) << QPointF(20+50, 20+50) << QPointF(20+25, 20+50);
    painter->drawPolygon(pentagon);  //draws pentagon
    painter->setBrush(b);
}


QRectF pentagon::boundingRect() const{
    return QRectF(20, 20, 50, 50);
}

location_card* location_card::factory(QString type){  //FACTORY DESIGN METHOD
    if ( type == "triangle" ) return new triangle();
    else if ( type == "square" ) return new square();
    else if ( type == "hexagon" ) return new hexagon();
    else return new pentagon();
}


