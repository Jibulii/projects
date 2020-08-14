#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startdialog.hh"
#include "galaxy.hh"
#include "initialize.hh"
#include "igamerunner.hh"
#include "utility.hh"
#include "eventhandler.hh"
#include "cargoship.hh"
#include "highscorejson.h"
#include "time.h"
#include "scoredialog.h"
#include <QDebug>
#include "mainwindow.h"
#include "gamewindow.h"
#include "starsystemfactory.hh"
#include "enddialog.h"
#include <QProcess>
#include "utility.hh"
#include "probabilitydrive.h"
#include "initialize.hh"
#include "starsystem.hh"
#include "shipfactory.hh"
#include "ship.hh"
#include "shipitem.h"
#include <vector>
#include <algorithm>
#include <QApplication>
#include <memory>
#include <iostream>

Mainwindow::Mainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mainwindow)
{
    ui->setupUi(this);
    // Added opening screen image
    QPixmap bkgnd("Assets/OpeningScreen.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Changing buttons to transparent
    ui->startGame->setFlat(true);
    ui->startGame->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->startGame->setStyleSheet("* {color: white }");

    ui->endGame->setFlat(true);
    ui->endGame->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->endGame->setStyleSheet("* {color: white }");

    ui->highscores->setFlat(true);
    ui->highscores->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    ui->highscores->setStyleSheet("* {color: white }");

    connect(ui->highscores, SIGNAL(clicked()), this, SLOT(showScores()));
    connect(ui->startGame, SIGNAL(clicked()), this, SLOT(startGame()));
}

Mainwindow::~Mainwindow()
{
    delete ui;
}

void Mainwindow::startGame()
{
    startdialog dialog(this);

    // If startdialog is accepted, the game starts.
    if (dialog.exec() == QDialog::Accepted){

        //This string is the player's name.
        QString string = dialog.getString();
        this->hide();
        Gamewindow* window = new Gamewindow(this);

        window->setName(string);
        window->setWindowFlags(Qt::Window);

        //If the gamewindow is closed, slotReboot-slot is called.
        connect(window, SIGNAL(destroyed(QObject*)), this, SLOT(slotReboot()));

        //The gamewindow is passed as a variable to the eventhandler.
        std::shared_ptr<Common::IEventHandler> handler = std::make_shared<Student::EventHandler>(window);
        std::shared_ptr<Student::Galaxy> galaxy = std::make_shared<Student::Galaxy>();
        std::shared_ptr<Common::IGameRunner> gameRunner = Common::getGameRunner(galaxy, handler);
        Common::utilityInit(time(NULL));
        Common::addNewShipType("ProbabilityCargoShip", [=] (std::shared_ptr<Common::StarSystem> initialLocation,
                               std::shared_ptr<Common::IEventHandler> events) -> std::shared_ptr<Common::Ship>  {
                               return std::make_shared<Common::CargoShip>(std::make_shared<Common::ProbabilityDrive>(galaxy),
                                                                          initialLocation, events);});

        window->initializeGameWindow(gameRunner, galaxy, handler);
        window->show();

        Common::StarSystem::StarSystemVector stars = galaxy->getSystems();

        for (auto star : stars){
            //Add starsystems to the map
            window->addStarSystem(star);
        }

        //20 ships are spawned initially.
        gameRunner->spawnShips(20);
    }

}

void Mainwindow::slotReboot()
{
    //Starts an another process and closes the original app.
    QProcess process;
    process.startDetached("GalaxyGame.exe",QStringList());
    qApp->quit();
}

void Mainwindow::showScores(){
    //Creates a Scoredialog and sets the values.
    ScoreDialog dialog;
    dialog.SetTable(ReadJson());
    dialog.exec();
}
