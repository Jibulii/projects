#ifndef STARTDIALOG_HH
#define STARTDIALOG_HH

#include <QDialog>

namespace Ui {


class startdialog;
}
/**
 * @brief The startdialog class Shows the start dialog.
 * The player can assign their player name here and then
 * start the game.
 */
class startdialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief startdialog The constructor
     * @param parent The parent of the dialog.
     */
    explicit startdialog(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~startdialog();

    /**
     * @brief getString gets the player's name.
     * @return the player's name as a QString.
     */
    QString getString();
public slots:

private:
    Ui::startdialog *ui;

};

#endif // STARTDIALOG_HH
