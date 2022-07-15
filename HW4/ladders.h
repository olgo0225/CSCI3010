#ifndef LADDERS_H
#define LADDERS_H


#include <QColor>
#include <QGraphicsItem>

class ladders : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    ladders(int start_x_, int end_x_, int start_y_, int end_y_, int start_s, int end_s) {start_x=start_x_; end_x=end_x_; start_y=start_y_; end_y=end_y_; start_slot=start_s; end_slot=end_s;};

    int get_x_start() {return start_x;};
    int get_x_end() {return end_x;};
    int get_y_start() {return start_y;};
    int get_y_end() {return end_y;};
    int get_start() {return start_slot;};
    int get_end() {return end_slot;};

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
    int start_x;
    int end_x;
    int start_y;
    int end_y;
    int start_slot;
    int end_slot;

};
#endif // LADDERS_H
