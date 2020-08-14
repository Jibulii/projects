#ifndef SCOREDIALOG_H
#define SCOREDIALOG_H

#include <QDialog>
#include <QStandardItem>

namespace Ui {
class ScoreDialog;
}
/**
 * @brief The ScoreDialog class Shows the highscore table dialog.
 */
class ScoreDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief ScoreDialog Constructor
     * @param parent Parent of the widget.
     */
    explicit ScoreDialog(QWidget *parent = 0);

    /**
     * @brief Destructor.
     */
    ~ScoreDialog();

    /**
     * @brief SetTable sets the table's contents
     * @param vector vector which has all the players who have played
     * the game.
     */
    void SetTable(std::vector<std::vector<std::string> > vector);
private:
    Ui::ScoreDialog *ui;
};

#endif // SCOREDIALOG_H
