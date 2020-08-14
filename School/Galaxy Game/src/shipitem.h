#ifndef SHIPITEM_H
#define SHIPITEM_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include "ship.hh"

/**
 * @brief The ShipItem class Shipitem class inherits QGraphicsitem.
 * It is used to show the ship on the map.
 */
class ShipItem : public QGraphicsItem
{
public:
    /**
     * @brief ShipItem Constructor
     */
    ShipItem();

    /**
     * @brief Destructor.
     */
    ~ShipItem();

    /**
     * Enumerate a type to make a distinction to normal a QGraphicsitem
     */
    enum { Type = UserType+1 };
       int type() const { return Type; }

    /**
    * @brief boundingRect creates the rectangle where the ship is painted
    * @return QRectF-item.
    */
    QRectF boundingRect() const;

    /**
    * @brief paint Paints the ship to the QGraphicsScene
    * everytime the scene updates or the ship moves.
    * @param painter QPainter in charge of painting the ship to the scene
     * @param option Optional options to alter the painting.
    * @param widget ship's parent widget.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    /**
     * @brief ship the ship which the shipitem represents.
     */
    std::shared_ptr<Common::Ship> ship;

    /**
     * @brief setDistress sets the distress of the ship. It is used to
     * implement the blinking distress light.
     * @param distress The value of distress to be set.
     */
    void setDistress(int distress);

    /**
    * @brief boundingRect creates the rectangle where the ship is painted
    * while it is in distress. It is a bit larger than the normal ship rect.
    * @return QRectF-item.
    */
    QRectF distressRect() const;

    /**
     * @brief getDistress Is used to get the current distress value.
     * @return Current value of distress.
     */
    int getDistress();

    /**
     * @brief setShip Assigns the ship to the shipitem.
     * @param ship2 The ship to be assigned.
     */
    void setShip(std::shared_ptr<Common::Ship> ship2);
private:

    /**
     * @brief distress_ The amount of distress.
     */
    unsigned int distress_;

    /**
     * @brief color_ the color of the ship.
     */
    QColor color_;
};
#endif // SHIPITEM_H
