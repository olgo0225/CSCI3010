#include "winner.h"
#include "ui_winner.h"

winner::winner(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::winner)
{
    ui->setupUi(this);
}

winner::~winner()
{
    delete ui;
}

void winner::on_quit_clicked()
{
    emit quit();
    this->close();

}

void winner::set_player(QString player){
    ui->player->setText(player);
}


void winner::on_new_game_clicked()
{
    emit new_game();
    this->close();
}

