#ifndef STARSYSTEMITEM_H
#define STARSYSTEMITEM_H

#include "starsystem.hh"
#include <QGraphicsItem>
#include <QPainter>

/**
 * @brief The StarSystemItem class StarSystemItem class
 * inherits QGraphicsitem.
 * It is used to show the StarSystem on the map.
 */
class StarSystemItem : public QGraphicsItem
{
public:
    /**
     * @brief StarSystemItem Constructor
     */
    StarSystemItem();
    /**
     * @brief Destructor.
     */
    ~StarSystemItem();

    /**
     * Enumerate a type to make a distinction to normal a QGraphicsitem
     */
    enum { Type = UserType+2 };
       int type() const { return Type; }

    /**
    * @brief boundingRect creates the rectangle where the ship is painted
    * @return QRectF-item.
    */
    QRectF boundingRect() const;

    /**
    * @brief paint Paints the system to the QGraphicsScene
    * everytime the scene updates or the ship moves.
    * @param painter QPainter in charge of painting the system to the scene
     * @param option Optional options to alter the painting.
    * @param widget system's parent widget.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief starsystem the starsystem which the starsystemitem represents.
     */
    std::shared_ptr<Common::StarSystem> system;
private:

    /**
     * @brief color_ Color of the starsystem
     */
    QColor color_;
};

#endif // STARSYSTEMITEM_H
