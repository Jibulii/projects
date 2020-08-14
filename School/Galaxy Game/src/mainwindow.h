#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "gamewindow.h"
#include <QWidget>
#include <QApplication>

namespace Ui {
class Mainwindow;
}

/**
 * @brief The Mainwindow class shows the main window, which
 * lets the player choose to play the game, quit the app or
 * check the high scores. It inherits the QWidget-class.
 */
class Mainwindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Mainwindow normal constructor for QWidget.
     * @param parent Parent of the widget.
     */
    explicit Mainwindow(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~Mainwindow();


public slots:
    /**
     * @brief startGame Starts the game.
     */
    void startGame();

    /**
     * @brief slotReboot Reboots the application.
     */
    void slotReboot();

    /**
     * @brief showScores Shows the high score -window
     */
    void showScores();

private:
    Ui::Mainwindow *ui;
};

#endif // MAINWINDOW_H
