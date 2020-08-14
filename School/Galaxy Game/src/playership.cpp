#include "playership.h"
#include "ship.hh"
#include "gamewindow.h"
#include <QToolTip>
#include <utility.hh>
#include <QGraphicsItem>
#include <QPainter>
#include <QKeyEvent>

PlayerShip::PlayerShip()
{
    setPos(100,100);
    this->statistics_.addCredits(500);
    fuel_ = 20;
    speed_ = 0;
}

PlayerShip::~PlayerShip()
{

}

QRectF PlayerShip::boundingRect() const
{
    return QRect(-5,-5,10,10);
}

void PlayerShip::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    QRectF rec = boundingRect();
    QBrush Brush(Qt::green);

    QPointF pos = this->pos();
    if (pos.x() < -300){
        pos.setX(-290);
    }
    else if (pos.x() > 300){
        pos.setX(290);
    }
    else if (pos.y() > 300){
        pos.setY(290);
    }
    else if(pos.y() < -300){
        pos.setY(-290);
    }
    this->setPos(pos);
    painter->fillRect(rec,Brush);
    painter->drawRect(rec);
}

void PlayerShip::advance(int)
{

    setPos(mapToParent(0,-(speed_)));


}

void PlayerShip::keyPressEvent(QKeyEvent *event){

    //If an arrowkey is pressed, the direction changes.
    //If the playership is stationary, it starts moving.
    //If the key is to the same as the direction, the ship goes faster.
    if (event->key() == Qt::Key_Left){
        if (speed_ == 0){
            setRotation(-90);
            speed_ = 3;
        }
        else if (this->rotation() == -90 and speed_ != 0){
            speed_ = 6;
        }
        else
            setRotation(-90);
    }
    else if (event->key() == Qt::Key_Right){
        if (speed_ == 0){
            setRotation(90);
            speed_ = 3;
        }
        else if (this->rotation() == 90 and speed_ != 0){
            speed_ = 6;
        }
        else
            setRotation(90);
    }
    else if (event->key() == Qt::Key_Up){
        if (speed_ == 0){
            setRotation(0);
            speed_ = 3;
        }
        else if (this->rotation() == 0 and speed_ != 0){
            speed_ = 6;
        }
        else
            setRotation(0);
    }
    else if (event->key() == Qt::Key_Down){
        if (speed_ == 0){
            setRotation(180);
            speed_ = 3;
        }
        else if (this->rotation() == 180 and speed_ != 0){
            speed_ = 6;
        }
        else
            setRotation(180);
    }
    //If space is pressed, the ship stops.
    else if (event->key() == Qt::Key_Space){
            speed_ = 0;
    }
}

//If the playership is moving, it loses fuel.
void PlayerShip::reduceFuel(){
    if (fuel_ > 0 and speed_ != 0)
        fuel_--;
}

unsigned int PlayerShip::getFuel(){
    return fuel_;
}

void PlayerShip::addFuel(unsigned int amount, unsigned int price){
    if (fuel_ + amount > 20)
        fuel_ = 20;
    else
        fuel_ = fuel_ + amount;

    statistics_.reduceCredits(amount * price);
}
