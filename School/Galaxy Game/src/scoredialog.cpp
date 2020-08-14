#include "scoredialog.h"
#include "ui_scoredialog.h"
#include <QStandardItem>

ScoreDialog::ScoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScoreDialog)
{
    ui->setupUi(this);
    QStringList headers;
    headers << "Name" << "Points" << "Saved ships" << "Time";
    ui->scoreTable->setColumnCount(4);
    ui->scoreTable->setHorizontalHeaderLabels(headers);

}

ScoreDialog::~ScoreDialog()
{
    delete ui;
}

void ScoreDialog::SetTable(std::vector<std::vector<std::string> > vector){

    //The table is populated with data.
    ui->scoreTable->setRowCount(vector.size());
    for (unsigned int i = 0; i < vector.size(); i++){
        for (unsigned int e = 0; e < vector[i].size(); e++){
            QTableWidgetItem * item = new QTableWidgetItem(QString::fromStdString(vector[i][e]));
            ui->scoreTable->setItem(i, e, item);
        }
    }
}
