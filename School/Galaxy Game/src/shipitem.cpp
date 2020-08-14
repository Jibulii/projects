#include "shipitem.h"
#include <QToolTip>
#include <utility.hh>
#include "shipengine.hh"
#include <QGraphicsItem>
#include <QPainter>

ShipItem::ShipItem()
{
    distress_ = 0;
    //Default color is blue (Warpdriveships stay blue).
    color_ = Qt::blue;
}

ShipItem::~ShipItem()
{

}

QRectF ShipItem::boundingRect() const
{
    return QRect(-5,-5,10,10);
}

//A ship in a certain state of distress is larger than normal, to make the distressblinking more visible.
QRectF ShipItem::distressRect() const
{
    return QRect(-7.5,-7.5,15,15);
}

void ShipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush Brush(Qt::red);

    //An abandoned ship is gray
    if (ship->isAbandoned()){
        Brush.setColor(Qt::gray);
    }

    //Distress changes the color from red to white and back.
    else if (distress_ != 0){
        if (distress_ % 2 == 0){
            Brush.setColor(Qt::red);
            distress_++;
        }
        else{
            Brush.setColor(Qt::white);
            rec = distressRect();
            distress_++;
        }
    }
    else {
        Brush.setColor(color_);
    }

    painter->fillRect(rec,Brush);
    painter->drawRect(rec);

}

void ShipItem::setDistress(int distress)
{
    distress_ = distress;
}

int ShipItem::getDistress()
{
    return distress_;
}

void ShipItem::setShip(std::shared_ptr<Common::Ship> targetship){
    ship = targetship;
    auto maxHealth = ship->getEngine()->getMaxHealth();

    //Probabilitydriveships are cyan.
    if (maxHealth == 3)
        color_ = Qt::cyan;

    //Wormholedrivehips are light blue.
    else if (maxHealth == 25)
        color_.setRgb(0,102,204);
}
