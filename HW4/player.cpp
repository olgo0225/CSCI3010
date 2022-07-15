#include <iostream>
#include <QtWidgets>

#include "player.h"

Player::Player(QColor color, QString user, QString name_) {
  color_ = color;
  user_=user;
  active=false;
  x_ = 25*(user.toInt()-1);
  y_ = 0;
  rerolls=0;
  undos=0;
  name=name_;
  games_played=0;
  wins=0;

}


QRectF Player::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

// define the actual shape of the object
QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}

// called by Qt to actually display the point
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    // update the line for setBrush to be this
    painter->setBrush(QBrush(color_));

    painter->drawEllipse(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}


