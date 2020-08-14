#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "starsystem.hh"
#include "shipitem.h"
#include "playership.h"
#include "cargoship.hh"
#include "point.hh"
#include "enddialog.h"
#include <QElapsedTimer>
#include "highscorejson.h"
#include "mainwindow.h"
#include <QGraphicsView>
#include <QPixmap>
#include <QWheelEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <string>
#include <QTimer>
#include <QShortcut>
#include <QTimeLine>
#include <QGraphicsItemAnimation>
#include <math.h>
#include <iostream>

Gamewindow::Gamewindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gamewindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    turn_ = 0;

    ui->galaxyBoard->setScene(scene);
    ui->galaxyBoard->setRenderHint(QPainter::Antialiasing);

    //Setting up the game coordinates.
    QRect rect(-300, -300, 600, 600);
    scene->setSceneRect(rect);

    //This is the background image.
    QBrush brush(QImage("Assets/Galaxy.jpg"));

    //Moving the background image to make the visuals better.
    QTransform transform;
    transform.translate(-500,-500);
    brush.setTransform(transform);

    scene->setBackgroundBrush(brush);

    //Scale the QGraphicsview appropriately.
    ui->galaxyBoard->scale(1.4, 1.4);
    ui->galaxyBoard->show();


    //These are the game's borders.
    QPen Pen = QPen(Qt::red);
    QLineF TopLine(scene->sceneRect().topLeft(), scene->sceneRect().topRight());
    QLineF LeftLine(scene->sceneRect().topLeft(), scene->sceneRect().bottomLeft());
    QLineF BottomLine(scene->sceneRect().bottomLeft(), scene->sceneRect().bottomRight());
    QLineF RightLine(scene->sceneRect().topRight(), scene->sceneRect().bottomRight());

    scene->addLine(TopLine,Pen);
    scene->addLine(LeftLine,Pen);
    scene->addLine(BottomLine,Pen);
    scene->addLine(RightLine,Pen);

    PlayerShip * playership = new PlayerShip();
    playerShip = playership;
    scene->addItem(playership);

    playership->setFlag(QGraphicsItem::ItemIsFocusable);
    playership->setFocus();

    //This is the gametime timer.
    gametime_ = new QElapsedTimer;

    //This timer is connected to the general actions.
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(doActions()));

    //This timer is connected to updating the playership's position.
    playertimer = new QTimer(this);
    connect(playertimer, SIGNAL(timeout()), this, SLOT(AdvancePlayerShip()));

    connect(ui->endGameButton, SIGNAL(clicked()), this, SLOT(EndGame()));

    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(PauseGame()));

    connect(ui->fuelMeter, SIGNAL(valueChanged(int)), this, SLOT(checkFuel()));

    //The fuel buttons are set to false as a default.
    connect(ui->fuelButton, SIGNAL(clicked()), this, SLOT(BuyFuel()));
    ui->fuelButton->setEnabled(false);
    connect(ui->fuelButton1, SIGNAL(clicked()), this, SLOT(BuyFuel_1()));
    ui->fuelButton1->setEnabled(false);

    //These are the shortcuts for buying fuel.
    QShortcut *shortcut = new QShortcut(QKeySequence("b"), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(BuyFuel()));
    QShortcut *shortcut1 = new QShortcut(QKeySequence("v"), this);
    connect(shortcut1, SIGNAL(activated()), this, SLOT(BuyFuel_1()));

    ui->infoLabel->hide();

    //These are the timings the timers tick.
    playertimer->start(50);
    timer->start(200);
    gametime_->start();

}

//The destructor.
Gamewindow::~Gamewindow()
{
    delete ui;
    delete playerShip;
    for (auto system : StarSystem_vector){
        delete system;
    }
    for (auto ship : Ship_vector){
        delete ship;
    }
}

//The playership is advanced everytime the playertimer ticks.
void Gamewindow::AdvancePlayerShip(){
    playerShip->advance(1);
}

// Adds a StarSystemItem
void Gamewindow::addStarSystem(std::shared_ptr<Common::StarSystem> System) {
    StarSystemItem* item = new StarSystemItem();
    item->system = System;
    Common::Point point = System->getCoordinates();

     //The shipitem's coordinates are 10 times the ship's coordinates.
    item->setPos((point.x)*10, (point.y)*10);

    //A tooltip is assigned to the ship.
    std::string economy = getEconomyAsStringFromEnum(System->getEconomy());
    std::string tooltip = "Name: " + System->getName()
            + "\nEconomy: " + economy + "\nPopulation: "
            + std::to_string(System->getPopulation());
    QString string = QString::fromStdString(tooltip);
    item->setToolTip(string);

    scene->addItem(item);
    this->StarSystem_vector.push_back(item);
}

//Adds a ShipItem
void Gamewindow::addShip(std::shared_ptr<Common::Ship> ship) {
    ShipItem* item = new ShipItem();
    item->setShip(ship);
    Common::Point point = ship->getLocation()->getCoordinates();

    //The shipitem's coordinates are 10 times the ship's coordinates.
    item->setPos((point.x)*10, (point.y)*10);

    //A tooltip is assigned to the ship.
    QString string = QString::fromStdString(ship->getName());
    item->setToolTip(string);
    scene->addItem(item);

    //The z-value is set to -1 to always get the ships behind the starsystems for clarity.
    item->setZValue(-1);
    this->Ship_vector.push_back(item);
}

//Deletes the ship
void Gamewindow::deleteShip(std::string name){
    int size = this->Ship_vector.size();
    int i = 0;
    while (i < size){
        if (name == Ship_vector[i]->ship->getName()){

            //The ship is removed from the galaxy, the scene and the ship_vector.
            galaxy_->removeShip(Ship_vector[i]->ship);
            scene->removeItem(Ship_vector[i]);
            Ship_vector.erase(Ship_vector.begin() + i);
            i = size;
            break;
        }
        i++;
    }
}

void Gamewindow::initializeGameWindow(std::shared_ptr<Common::IGameRunner> gameRunner,
                                      std::shared_ptr<Student::Galaxy> galaxy, std::shared_ptr<Common::IEventHandler> handler)
{
    gameRunner_ = gameRunner;
    galaxy_ = galaxy;
    handler_ = handler;
}

void Gamewindow::doActions(){

    //These actions are set to only happen every fourth time the timer ticks.
    if (turn_ == 0){
        gameRunner_->createActions();
        gameRunner_->doActions();
        turn_=4;
        playerShip->reduceFuel();
        ui->galaxyBoard->setFocus();
        playerShip->setFocus();

    }

    CheckCollisions();
    scene->update();

    //Ui is updated.
    int number;
    number = playerShip->statistics_.getPoints();
    ui->pointCounter->display(number);
    number = playerShip->statistics_.getSavedShips();
    ui->savedShipCounter->display(number);
    number = playerShip->statistics_.getCreditBalance();
    ui->creditCounter->display(number);
    ui->fuelMeter->setValue(playerShip->getFuel());
    number = playerShip->statistics_.getLostShips();
    ui->lostShipCounter->display(number);

    //If lost ships hit 20, the game is ended
    if (number == 20){
        EndGame();
    }

    //If npc-ship-count is less than 10, 10 more ships are spawned.
    if (Ship_vector.size() <= 10){
        gameRunner_->spawnShips(10);
        std::string text =  "10 More ships spawned!";
        text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
        ui->GameLog->appendPlainText(QString::fromStdString(text));
    }

    turn_--;
}

void Gamewindow::moveShip(std::shared_ptr<Common::Ship> targetShip,
                          Common::Point origin, Common::Point target){

    std::string name1 = targetShip->getName();
    //Create a QGraphicsItemAnimation to move the ship from origin to target.
    for (auto shipItem : Ship_vector){
        if (shipItem->ship->getName() == name1){
            QTimeLine *timer = new QTimeLine(800, this);
            timer->setCurveShape(QTimeLine::LinearCurve);
            QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;

            animation->setItem(shipItem);
            animation->setTimeLine(timer);
            animation->setPosAt(0,QPoint(origin.x*10, origin.y*10));
            animation->setPosAt(1,QPoint(target.x*10, target.y*10));

            timer->start();
        }
    }
}

void Gamewindow::ShipLost(std::shared_ptr<Common::Ship> ship){
    std::string text = ship->getName();
    text += " was abandoned..";
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    ui->GameLog->appendPlainText(QString::fromStdString(text));

    //Delete the abandoned ship after a while
    std::string name = ship->getName();
    for (auto shipItem : Ship_vector){
        if (shipItem->ship->getName() == name){
            QTimer::singleShot(3000, this, [this, name]() {deleteShip(name); });
        }
    }
}

void Gamewindow::ShipSaved(std::string name, int credits){
    std::string text;
    //If the player was in time to save the ship, he gets rewarded.
    if (credits != -1){
        playerShip->statistics_.addSavedShip();
        playerShip->statistics_.addPoints(100);
        playerShip->statistics_.addCredits(credits);
        text += "The crew of " + name + " was rescued! You got " +
                std::to_string(credits) + " credits for saving it!";
    }
    //If not, he gets 10 credits.
    else {
        playerShip->statistics_.addPoints(10);
        playerShip->statistics_.addCredits(10);
        text += "You were not in time to save " + name +
                ". You were able to salvage 10 credits from the wreckage.";
    }
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    ui->GameLog->appendPlainText(QString::fromStdString(text));

}

void Gamewindow::Distress(std::shared_ptr<Common::Ship> ship, int distress){
    //Update the ship's distress.
    std::string name1 = ship->getName();
    for (auto shipItem : Ship_vector){
        if (shipItem->ship->getName() == name1){
            shipItem->setDistress(distress);
            shipItem->update();
        }
    }
    std::string text = ship->getName();
    //Inform gamelog of the distress.
    if (distress != 0)
        text += " is in distress!";
    text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());
    ui->GameLog->appendPlainText(QString::fromStdString(text));

}

void Gamewindow::setName(QString name)
{
    name_ = name;
    ui->nameLabel->setText(name);
}

void Gamewindow::CheckCollisions(){

    bool fuelActive=false;
    //List of items colliding with the player.
    QList<QGraphicsItem *> items = scene->collidingItems(playerShip);
    for (auto item : items){
        //If the player hits a ship which is in distress or abandoned,
        //The shipsaved-function is called
        if (item->type() == 65537){
            for (auto ship : Ship_vector){
                if (item->pos() == ship->pos()){
                    if (ship->getDistress() != 0){
                    std::string name = ship->ship->getName();
                    if (ship->ship->isAbandoned() == false)
                        ShipSaved(name, 150 - ship->getDistress());
                    else
                        ShipSaved(name, -1);
                    deleteShip(name);
                    }
                }
            }
        }
        //If the player is on a starsystem, fuel can be bought.
        else if (item->type() == 65538){
            for (auto system : StarSystem_vector){
                if (system->pos() == item->pos() and system->system->getPopulation() != 0){
                    CalculateFuelPrice(system->system->getPopulation());
                    fuelActive = true;
                }       
            }
        }
        //If the player hits the edge of the playable area
        else if (item->type() == 6 and ui->infoLabel->isHidden()){
            ui->infoLabel->setText("Do not leave the mission area.");
            ui->infoLabel->show();
            QTimer::singleShot(3000, this, SLOT(OutOfBounds()));
        }
    }
    //If the player does not hit a starsystem, fuel cannot be bought.
    if (fuelActive == false){
        ui->fuelButton->setEnabled(false);
        ui->fuelButton1->setEnabled(false);
        fuelActive = false;
        ui->fuelLabel->setText("Move to a populated starsystem to refuel.");
    }
}



void Gamewindow::PauseGame(){
    //Pauses the game and informs the player about it.
    if (timer->isActive() ){
        timer->stop();
        playertimer->stop();
        turn_ = 4;
        ui->infoLabel->setText("Game paused");
        ui->infoLabel->show();
        ui->fuelButton->setEnabled(false);
        ui->fuelButton1->setEnabled(false);
        ui->galaxyBoard->setEnabled(false);
    }
    //If the game is already paused it unpauses the game.
    else{
        timer->start();
        playertimer->start();
        ui->infoLabel->hide();
        ui->galaxyBoard->setEnabled(true);
    }
}

void Gamewindow::BuyFuel(){
    //Checks if the player is able to fill the tank.
    if(ui->fuelButton->isEnabled()){
        int to_be_filled = 20-playerShip->getFuel();
        //If the player can, fuel tank is filled.
        if (to_be_filled*fuelPrice_ < playerShip->statistics_.getCreditBalance()){
            playerShip->addFuel(to_be_filled, fuelPrice_);
        }
        //Else, the player is notified that he/she can't buy fuel.
        else{
            ui->fuelLabel->setText("You don't have enough credits");
            ui->GameLog->appendPlainText(QString::fromStdString("Not enough credits"));
            ui->fuelButton->setEnabled(false);
        }
    }
    else {
        ui->fuelLabel->setText("Move to a populated starsystem to refuel.");
        ui->GameLog->appendPlainText(QString::fromStdString("You are not in a populated starsystem"));
    }
}

void Gamewindow::BuyFuel_1(){
    //Checks if the player is able to buy fuel.
    if(ui->fuelButton1->isEnabled()){
        //If the player can, one unit of fuel is bought.
        if (fuelPrice_ < playerShip->statistics_.getCreditBalance()){
            playerShip->addFuel(1, fuelPrice_);
        }
        //Else, the player is notified that he/she can't buy fuel.
        else{
            ui->fuelLabel->setText("You don't have enough credits");
            ui->GameLog->appendPlainText(QString::fromStdString("Not enough credits"));
            ui->fuelButton1->setEnabled(false);
        }
    }
    else {
        ui->fuelLabel->setText("Move to a populated starsystem to refuel.");
        ui->GameLog->appendPlainText(QString::fromStdString("You are not in a populated starsystem"));
    }
}

void Gamewindow::checkFuel(){
    int value = ui->fuelMeter->value();

    //If fuel is 0, the game ends.
    if (value == 0){
        EndGame();
    }
}

void Gamewindow::CalculateFuelPrice(int population){

    if (population != 0){
        std::string price = std::to_string(population);

        //Price of fuel is determined by the number of numbers
        //In the starsystems population.
        fuelPrice_ = 100 / price.length();
        std::string text = "Price of fuel per unit: "
                + std::to_string(fuelPrice_);

        ui->fuelLabel->setText(QString::fromStdString(text));
        ui->fuelButton->setEnabled(true);
        ui->fuelButton1->setEnabled(true);
    }
}

std::string Gamewindow::SavePointsToJson(){

    qint64 gametime = gametime_->elapsed();
    unsigned points = playerShip->statistics_.getPoints();
    unsigned saved = playerShip->statistics_.getSavedShips();

    //Saves the points to Json by calling the SaveJson-function.
    SaveJson(points, saved, name_, QString::number(gametime));

    //Returns a text to be displayed in the endDialog.
    return "You were great!\nYour scores:\nPoints: "+std::to_string(points)+"\nShips saved: "+std::to_string(saved);
}

//Game is ended by closing the window and causing a QCloseEvent.
void Gamewindow::EndGame(){
    this->close();
}

//Saves the points and shows the enddialog. After the dialog is closed, the gamewindow is terminated.
void Gamewindow::closeEvent(QCloseEvent *)
{
    std::string text = SavePointsToJson();
    EndDialog dialog(this, text);
    dialog.exec();
    emit close();
    this->deleteLater();
}

void Gamewindow::OutOfBounds(){
    ui->infoLabel->hide();
}

// Switches the enumNumbers to Strings.
std::string Gamewindow::getEconomyAsStringFromEnum(int enumNumber)
{
    switch (enumNumber) {
        case 0:     return "Refinery";
        case 1:     return "Extraction";
        case 2:     return "HiTech";
        case 3:     return "Industrial";
        case 4:     return "Tourism";
        case 5:     return "Agriculture";
        case 6:     return "Service";
        case 7:     return "Military";
        case 8:     return "Terraforming";
        case 9:    return "Colony";
        default:    return "None";
    }
}
