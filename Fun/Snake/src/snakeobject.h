#ifndef SNAKEOBJECT_H
#define SNAKEOBJECT_H
#include <QGraphicsItem>
#include <vector>
#include "tailobject.h"

class SnakeObject : public QGraphicsItem
{
public:
    SnakeObject();
    enum { Type = UserType+1 };
       int type() const { return Type; }
    void keyPressEvent(QKeyEvent *event);
    void advance2();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    void moveTail(QPoint *pos);
    void addTail(TailObject *tail);
    void increaseTail();
    int tailLength();
    void increaseSpeed();
private:
    int speed_;
    QPoint pos_;
    int x_speed_;
    int y_speed_;
    int tail_length_;

};

#endif // SNAKEOBJECT_H
