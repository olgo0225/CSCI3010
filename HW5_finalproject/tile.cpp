#include "tile.h"
#include <QPolygonF>

tile::tile(int x, int y, QColor color, bool upgrade, int slot)
{
 x_=x;
 y_=y;
 color_=color;
 upgrade_=upgrade;
 slot_=slot;
}

QRectF tile::boundingRect() const
{
    return QRectF(x_, y_, 40, 40);
}


void tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    if (upgrade_){ //if upgrade tile
        QBrush b = painter->brush();
        // update the line for setBrush to be this
        painter->setBrush(QBrush(color_));

        painter->drawRect(QRect(x_, y_, 40, 40));
        painter->drawEllipse(QRect(this->x_, this->y_, 10, 10));  //add circle
        painter->setBrush(b);
    }else{
        QBrush b = painter->brush();
        // update the line for setBrush to be this
        painter->setBrush(QBrush(color_));

        painter->drawRect(QRect(x_, y_, 40, 40));
        painter->setBrush(b);
    }


}

void location::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    // update the line for setBrush to be this

    if (name_=="triangle"){
        QColor color(255,0,0);
        painter->setBrush(QBrush(color));
        painter->drawRect(QRect(x_, y_, 40, 40));
        QPolygonF triangle;
        triangle << QPointF(this->x_, this->y_+5) << QPointF(this->x_+10, this->y_) <<  QPointF(this->x_+10, this->y_+10);
        painter->drawPolygon(triangle); //add triangle
    }
    else if (name_=="square"){
        QColor color(255,0,255);
        painter->setBrush(QBrush(color));
        painter->drawRect(QRect(x_, y_, 40, 40));
        QPolygonF square;
        square << QPointF(this->x_, this->y_) << QPointF(this->x_+10, this->y_) <<  QPointF(this->x_+10, this->y_+10) << QPointF(this->x_, this->y_+10);
        painter->drawPolygon(square); //add square
    }
    else if (name_=="pentagon"){
        QColor color(0,255,0);
        painter->setBrush(QBrush(color));
        painter->drawRect(QRect(x_, y_, 40, 40));
        QPolygonF pentagon;
        pentagon << QPointF(this->x_, this->y_+5) << QPointF(this->x_+5, this->y_) <<  QPointF(this->x_+10, this->y_) << QPointF(this->x_+10, this->y_+10) << QPointF(this->x_+5, this->y_+10);
        painter->drawPolygon(pentagon);  //add pentagon
    }
    else{
        QColor color(255,255,0);
        painter->setBrush(QBrush(color));
        painter->drawRect(QRect(x_, y_, 40, 40));
        QPolygonF hexagon;
        hexagon << QPointF(this->x_, this->y_+5) << QPointF(this->x_+5, this->y_) <<  QPointF(this->x_+10, this->y_) << QPointF(this->x_+15, this->y_+5) << QPointF(this->x_+10, this->y_+10) << QPointF(this->x_+5, this->y_+10);
        painter->drawPolygon(hexagon); //add hexagon
    }
    painter->setBrush(b);

}

QRectF location::boundingRect() const{
    return QRectF(x_, y_, 40, 40);
}


QPainterPath location::shape() const{
    QPainterPath path;
    path.addEllipse(x_, y_, 10, 10);
    return path;
}
