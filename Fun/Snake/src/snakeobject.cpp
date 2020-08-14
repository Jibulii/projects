#include "snakeobject.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <iostream>

SnakeObject::SnakeObject()
{
    tail_length_ = 0;
    speed_ = 10;
    x_speed_ = speed_;
    y_speed_ = 0;
    QPoint point(10,25);
    pos_ = point;
    setPos(10,25);
}

QRectF SnakeObject::boundingRect() const
{
    return QRect(0,0,10,10);
}

int SnakeObject::tailLength()
{
    return tail_length_;
}

void SnakeObject::increaseSpeed(){
    speed_ += 0.1;
}

void SnakeObject::increaseTail(){
    tail_length_++;
}

void SnakeObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::green);
    painter->fillRect(rec,Brush);
    painter->drawRect(rec);
}

void SnakeObject::advance2()
{
    pos_.setX(pos_.x()+x_speed_);
    pos_.setY(pos_.y()+y_speed_);
    setPos(pos_.x(), pos_.y());
}

void SnakeObject::keyPressEvent(QKeyEvent *event){
    if (tail_length_ > 1){
        if (event->key() == Qt::Key_Left){
            if (x_speed_ != -speed_ and y_speed_ != 0){
                x_speed_= -speed_;
                y_speed_= 0;
            }
        }
        else if (event->key() == Qt::Key_Right){
            if (x_speed_ != speed_ and y_speed_ != 0){
                x_speed_= speed_;
                y_speed_= 0;
            }
        }
        else if (event->key() == Qt::Key_Up){
            if (x_speed_ != 0 and y_speed_ != -speed_){
                x_speed_= 0;
                y_speed_= -speed_;
            }
        }
        else if (event->key() == Qt::Key_Down){
            if (x_speed_ != 0 and y_speed_ != speed_){
                x_speed_= 0;
                y_speed_= speed_;
            }
        }
    }
    else{
        if (event->key() == Qt::Key_Left){
                x_speed_= -speed_;
                y_speed_= 0;
        }
        else if (event->key() == Qt::Key_Right){
                x_speed_= speed_;
                y_speed_= 0;
        }
        else if (event->key() == Qt::Key_Up){
                x_speed_= 0;
                y_speed_= -speed_;
        }
        else if (event->key() == Qt::Key_Down){
                x_speed_= 0;
                y_speed_= speed_;
        }
    }
}
