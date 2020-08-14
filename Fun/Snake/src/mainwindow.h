#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include "snakeobject.h"
#include "fruitobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void checkCollisions();
    void gameOver();

    void createFruit();
    void eatFruit();
    void moveTail(QPointF pos);
    void addTail(TailObject *tail);

    void startGame();
public slots:
    void actions();
private:
    Ui::MainWindow *ui;
    QTimer* timer_;
    SnakeObject* snake_;
    QGraphicsScene* scene_;
    FruitObject* fruit_;
    std::vector <TailObject*> tail_;
    int deaths_;
    int points_;
};

#endif // MAINWINDOW_H
