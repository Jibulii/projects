#include "starsystemitem.h"
#include <QToolTip>
#include <utility.hh>

StarSystemItem::StarSystemItem()
{
    // The color of the planet is determined in the constructor.
    // The color is between red and orange.
    color_.setRgb(255,Common::randomMinMax(0,155),0);
}

StarSystemItem::~StarSystemItem()
{
}

QRectF StarSystemItem::boundingRect() const
{
    return QRectF(-5,-5,10,10);
}
void StarSystemItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Starsystem is drawn according to specifications.
    QBrush Brush(color_);
    QRectF rec = boundingRect();
    painter->setBrush(Brush);
    painter->drawEllipse(rec);
}


