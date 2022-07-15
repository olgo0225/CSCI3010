#include <iostream>
#include <QtWidgets>

#include "player.h"

player::player(QColor color, int user, QString name_) {
  color_ = color;
  user_=user;
  active=false;
  x_ = 25*(user-1);
  y_ = 0;
  move_5=0;
  skips=0;
  name=name_;
  slot_=0;

}


QRectF player::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

// define the actual shape of the object
QPainterPath player::shape() const
{
    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}

// called by Qt to actually display the point
void player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    // update the line for setBrush to be this
    painter->setBrush(QBrush(color_));

    painter->drawEllipse(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

