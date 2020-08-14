#include "eventhandler.hh"
#include "point.hh"

using namespace Student;

EventHandler::EventHandler(Gamewindow* window)
{
    window_ = window;
}

EventHandler::~EventHandler()
{

}

void EventHandler::shipSpawned(std::shared_ptr<Common::Ship> ship)
{
    window_->addShip(ship);
}

//This function is not used in this game;
void EventHandler::shipRemoved(std::shared_ptr<Common::Ship>)
{
}

//This function is not used in this game;
void EventHandler::shipRelocated(std::shared_ptr<Common::Ship>,
                                 std::shared_ptr<Common::StarSystem>)
{

}

void EventHandler::shipMoved(std::shared_ptr<Common::Ship> ship,
                             Common::Point origin, Common::Point target)
{
    window_->moveShip(ship, origin, target);
}

//This function is not used in this game;
void EventHandler::exceptionInExecution(std::shared_ptr<Common::Ship>,
                                        const std::string &)
{

}

void EventHandler::distressOn(std::shared_ptr<Common::Ship> ship)
{
    window_->Distress(ship, 1);
}

void EventHandler::distressOff(std::shared_ptr<Common::Ship> ship)
{
    window_->Distress(ship, 0);
}

void EventHandler::shipAbandoned(std::shared_ptr<Common::Ship> ship)
{
    window_->playerShip->statistics_.addLostShip();
    window_->ShipLost(ship);
}
