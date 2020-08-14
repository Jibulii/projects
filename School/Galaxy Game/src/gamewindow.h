#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include "starsystemitem.h"
#include "starsystem.hh"
#include "galaxy.hh"
#include "gamerunner.hh"
#include "shipitem.h"
#include "playership.h"
#include "enddialog.h"
#include <QWidget>
#include <QElapsedTimer>
#include <QDialog>
#include <QGraphicsView>
#include <vector>

namespace Ui {
class Gamewindow;
}
/**
 * @brief The Gamewindow class Implements the window where the game is played.
 */
class Gamewindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Gamewindow Constructor
     * @param parent The parent of the widget
     */
    explicit Gamewindow(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~Gamewindow();

    /**
     * @brief addStarSystem Creates a starsystemitem and adds it to the
     * QGraphicsscene where the game is played.
     * @param System The system to be assigned to the starsystemitem.
     */
    void addStarSystem(std::shared_ptr<Common::StarSystem> System);

    /**
     * @brief StarSystem_vector Vector of pointers to all of the starsystemitems in the gamewindow.
     */
    std::vector<StarSystemItem*> StarSystem_vector;

    /**
     * @brief Ship_vector Vector of pointers to all of the shipitems in the gamewindow.
     */
    std::vector<ShipItem*> Ship_vector;

    /**
     * @brief playerShip Pointer to the playership
     */
    PlayerShip* playerShip;

    /**
     * @brief addShip Creates a shipitem and adds it to the QGraphicsscene where
     * the game is played.
     * @param ship The ship to be assigned to the Shipitem
     */
    void addShip(std::shared_ptr<Common::Ship> ship);

    /**
     * @brief initializeGameWindow Sets the proper parameters to the gamewindow.
     * @param gameRunner The Gamerunner
     * @param galaxy The Galaxy
     * @param handler The Eventhandler
     * @post the parameters are set.
     */
    void initializeGameWindow(std::shared_ptr<Common::IGameRunner> gameRunner,
                              std::shared_ptr<Student::Galaxy> galaxy, std::shared_ptr<Common::IEventHandler> handler);

    /**
     * @pre A ship has been set to be moved.
     * @brief moveShip This is used to move the ships drawn on the gameboard.
     * @param targetShip Ship about to be moved
     * @param origin The Starsystem the ship is leaving.
     * @param target The destination starsystem.
     * @post The ship's position is set to the destination.
     */
    void moveShip(std::shared_ptr<Common::Ship> targetShip,
                  Common::Point origin, Common::Point target);

    /**
     * @brief getEconomyAsStringFromEnum Changes the Economy number to string
     * @param enumNumber The Economynumber to be changed.
     * @return The economy as a string.
     */
    std::string getEconomyAsStringFromEnum (int enumNumber);

    /**
     * @brief ShipLost A function to implement the abandonement of a ship.
     * It removes the ship from the galaxy and starts a 3-second timer after which
     * the ShipItem is deleted.
     * @param ship The ship to be deleted.
     */
    void ShipLost(std::shared_ptr<Common::Ship> ship);

    /**
     * @brief Distress Announces that the ship is in distress to the gamelog. Also
     * Starts the ships distresscounter.
     * @param ship The ship which is in distress.
     * @param distress The amount of distress (1).
     */
    void Distress(std::shared_ptr<Common::Ship> ship, int distress);

    /**
     * @brief CheckCollisions Checks the playership's current collisions and acts
     * accordingly.
     */
    void CheckCollisions();

    /**
     * @brief ShipSaved Implements the player saving the ship.
     * The function adds the correct amount of points and credits to the player.
     * It also logs the saved ship to the gamelog.
     * @param name Name of the saved ship.
     * @param credits Amount of credits gained from the rescue.
     */
    void ShipSaved(std::string name, int credits);

    /**
     * @brief CalculateFuelPrice Calculates the fuelprice based on the planet's population.
     * @param population The population of the planet.
     */
    void CalculateFuelPrice(int population);

    /**
     * @brief closeEvent Overrides the normal CloseEvent of the QWidget.
     * @param event The CloseEvent.
     * @post The window is closed.
     */
    virtual void closeEvent(QCloseEvent *);

    /**
     * @brief setName Sets the players name to the gamewindow and shows it in the namelabel.
     * @param name Name of the player.
     * @post The name is set.
     */
    void setName(QString name);

    /**
     * @brief SavePointsToJson Calls a function from highscorejson.cpp to save the players
     * game to the scores.json.
     * @return returns a string to be shown in the enddialog.
     */
    std::string SavePointsToJson();

public slots:

    /**
     * @brief doActions Implements the games turns. This slot is called by the Qtimer timer_.
     * It calls NPC-ships CreateActions and DoActions. It also updates the UI.
     */
    void doActions();

    /**
     * @brief PauseGame Pauses and unpauses the game.
     */
    void PauseGame(); 

    /**
     * @brief BuyFuel Implements buying a full tank of fuel.
     */
    void BuyFuel();

    /**
     * @brief deleteShip Slot to call delete ship after a period of time.
     * ((QTimer::singleshot needs a slot to call))
     * @param name
     */
    void deleteShip(std::string name);

    /**
     * @brief checkFuel Checks the player's's fuel. If the fuel is 0, the game ends.
     */
    void checkFuel();

    /**
     * @brief BuyFuel_1 Implements buying 1 unit of fuel.
     */
    void BuyFuel_1();

    /**
     * @brief EndGame Closes the gamewindow, causing the CloseEvent to trigger.
     * This ends the game.
     */
    void EndGame();

    /**
     * @brief OutOfBounds Hides the OutofBoundlabel.
     * ((QTimer::singleshot needs a slot to call))
     */
    void OutOfBounds();

    /**
     * @brief AdvancePlayerShip Advances the playership's position.
     */
    void AdvancePlayerShip();

private:
    /**
     * @brief name_ The player's name.
     */
    QString name_;

    /**
     * @brief scene The QGraphicsScene of the game.
     */
    QGraphicsScene *scene;

    /**
     * @brief timer The game's general timer.
     */
    QTimer *timer;

    /**
     * @brief playertimer The playership's timer.
     */
    QTimer *playertimer;

    /**
     * @brief gametime_ Counts the elapsed gametime.
     */
    QElapsedTimer *gametime_;

    /**
     * @brief gameRunner_ The Gamerunner.
     */
    std::shared_ptr<Common::IGameRunner> gameRunner_;

    /**
     * @brief galaxy_ The Galaxy.
     */
    std::shared_ptr<Student::Galaxy> galaxy_;

    /**
     * @brief handler_ The Eventhandler.
     */
    std::shared_ptr<Common::IEventHandler> handler_;

    /**
     * @brief fuelPrice_ current price of fuel.
     */
    int fuelPrice_;

    /**
     * @brief turn_ A counter to know which actions are to be executed.
     */
    int turn_;

    Ui::Gamewindow *ui;
};



#endif // GAMEWINDOW_H
