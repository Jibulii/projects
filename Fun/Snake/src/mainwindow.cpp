#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>
#include "snakeobject.h"
#include <QGraphicsScene>
#include "fruitobject.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <vector>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer_ = new QTimer(this);

    scene_ = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene_);

    //Setting up the game coordinates.
    QRect rect(0, 0, 300, 300);
    scene_->setSceneRect(rect);



    QPen Pen = QPen(Qt::red);
    QLineF TopLine(scene_->sceneRect().topLeft(), scene_->sceneRect().topRight());
    QLineF LeftLine(scene_->sceneRect().topLeft(), scene_->sceneRect().bottomLeft());
    QLineF BottomLine(scene_->sceneRect().bottomLeft(), scene_->sceneRect().bottomRight());
    QLineF RightLine(scene_->sceneRect().topRight(), scene_->sceneRect().bottomRight());
    QBrush brush(Qt::lightGray);
    scene_->setBackgroundBrush(brush);
    scene_->addLine(TopLine,Pen);
    scene_->addLine(LeftLine,Pen);
    scene_->addLine(BottomLine,Pen);
    scene_->addLine(RightLine,Pen);

    connect(timer_, SIGNAL(timeout()), this, SLOT(actions()));
    snake_ = new SnakeObject;
    snake_->setFlag(QGraphicsItem::ItemIsFocusable);
    scene_->addItem(snake_);
    snake_->setFocus();

    fruit_ = new FruitObject;
    scene_->addItem(fruit_);
    createFruit();

    points_ = 0;
    deaths_ = 0;

    timer_->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startGame(){

    snake_ = new SnakeObject;
    snake_->setFlag(QGraphicsItem::ItemIsFocusable);
    scene_->addItem(snake_);
    snake_->setFocus();

    points_ = 0;
    ui->points->display(points_);

    fruit_ = new FruitObject;
    scene_->addItem(fruit_);
    createFruit();

    timer_->start(100);
}

void MainWindow::gameOver(){

    for (auto item : tail_){
        delete item;
    }

    tail_ = {};
    scene_->removeItem(snake_);
    scene_->removeItem(fruit_);
    delete snake_;
    delete fruit_;
    deaths_++;
    ui->deaths->display(deaths_);
    startGame();
}

void MainWindow::actions(){
    checkCollisions();
    snake_->setFocus();
    QPointF pos = snake_->pos();
    snake_->advance2();
    moveTail(pos);
}

void MainWindow::checkCollisions(){
    QList<QGraphicsItem *> items = scene_->collidingItems(snake_);
    for (auto item : items){
        if (item->type() == 6 or item->type() == 65538){
            timer_->stop();
            gameOver();
        }
        else if (item->type() == 65536){
            eatFruit();
        }
    }
}

void MainWindow::createFruit(){
    int x = rand() % 290;
    int y = rand() % 290;
    fruit_->setPos(x,y);
}

void MainWindow::eatFruit(){
    createFruit();
    TailObject* tail = new TailObject;
    addTail(tail);
    points_++;
    ui->points->display(points_);
    snake_->increaseSpeed();
    scene_->addItem(tail);
}

void MainWindow::addTail(TailObject* tail){
    snake_->increaseTail();
    tail_.push_back(tail);
}

void MainWindow::moveTail(QPointF pos){
    if (tail_.size()==1)
        tail_[0]->setPos(pos);
    if (tail_.size()>1){
        std::rotate(tail_.begin(),tail_.begin()+1,tail_.end());
        tail_[0]->setPos(pos);
    }
}
