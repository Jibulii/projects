#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "ship.hh"
#include "statistics.hh"
#include <QGraphicsItem>
/**
 * @brief The PlayerShip class Implements the ship which the player
 * drives.
 */
class PlayerShip : public QGraphicsItem
{
public:
    /**
     * @brief PlayerShip The constructor
     */
    PlayerShip();

    /**
     * @brief Destructor.
     */
    ~PlayerShip();

    /**
     * Enumerate a type to make a distinction to normal a QGraphicsitem
     */
    enum { Type = UserType+3 };
       int type() const { return Type; }

     /**
     * @brief paint Paints the ship to the QGraphicsScene
     * everytime the scene updates or the ship moves.
     * @param painter QPainter in charge of painting the ship to the scene
     * @param option Optional options to alter the painting.
     * @param widget ship's parent widget.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    /**
     * @brief keyPressEvent Overrided function from QGraphicsitem to catch keystrokes
     * from the player.
     * @param event The keystroke event
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * @brief boundingRect creates the rectangle where the ship is painted
     * @return QRectF-item.
     */
    QRectF boundingRect() const;

    /**
     * @brief statistics_ Statistics-class element, which keeps tracks of the
     * player's progress.
     */
    Student::Statistics statistics_;

    /**
     * @brief reduceFuel Reduces the playerships fuel by 1.
     */
    void reduceFuel();

    /**
     * @brief getFuel A function to get the current amount of fuel in
     * the player's ship.
     * @return The amount of fuel left.
     */
    unsigned int getFuel();

    /**
     * @brief addFuel Is used to add fuel to the player's ship
     * @param amount The amount of fuel to be added.
     * @param price Cost of the fuel per unit.
     * @post fuel has been added to the ship.
     */
    void addFuel(unsigned int amount, unsigned int price);

    /**
     * @brief advance Overrided function to update the ship's position
     * @param phase The phase to be executed.
     */
    void advance(int);

private:
    /**
     * @brief speed_ The current speed of the ship.
     */
    qreal speed_;

    /**
     * @brief angle_ The current angle to which the ship is moving
     */
    qreal angle_;

    /**
     * @brief fuel_ The ship's fuel.
     */
    unsigned int fuel_;

};

#endif // PLAYERSHIP_H
