#include "playernames.h"
#include "ui_playernames.h"

#include <QTextEdit>
#include <QLabel>

PlayerNames::PlayerNames(QWidget *parent, int n) :
    QDialog(parent),
    ui(new Ui::PlayerNames)
{
    ui->setupUi(this);

}

PlayerNames::~PlayerNames()
{
    delete ui;
}

void PlayerNames::addWidgets(){
    for (int i=1; i<=numb_players; i++){
        std::string play_label = "Player: " + std::to_string(i);
        QLabel *p1 = new QLabel(play_label.c_str());
        p1->setParent(this);
        p1->setGeometry(40, i*50, 60, 30);
        QLabel *p1_n = new QLabel ("Name: ");
        p1_n->setParent(this);
        p1_n->setGeometry(100, i*50, 50, 30);
        QTextEdit *name = new QTextEdit();
        name->setGeometry(160, i*50, 75, 30);
        name->setParent(this);
        std::string name_ = "Player"+ std::to_string(i);
        name->setObjectName(name_.c_str());
    }
}

void PlayerNames::on_submit_clicked()
{
    names.clear();
    for (int i=1; i<=numb_players; i++){
        std::string name_ = "Player"+ std::to_string(i);
        QString name1 = findChild<QTextEdit*>(name_.c_str())->toPlainText();
        names.append(name1);
    }
    qDebug() << "Got players names";
    emit names_added();
    this->close();
}

