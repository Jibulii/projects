#ifndef ENDDIALOG_H
#define ENDDIALOG_H
#include <QDialog>

namespace Ui {

class EndDialog;
}
/**
 * @brief EndDialog class shows a message in a dialog
 *  when the game ends accepting or closing
 * the dialog will open a new
 * instance of mainwindow.
 */
class EndDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief EndDialog The constructor
     * @param parent The parent of the dialog.
     * @param text Text to be shown when the dialog is opened.
     */
    explicit EndDialog(QWidget *parent = 0, std::string text = "");

    /**
     * @brief Destructor.
     */
    ~EndDialog();

private:
    Ui::EndDialog *ui;
};

#endif // ENDDIALOG_H
