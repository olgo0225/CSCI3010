#include "leaderboard.h"
#include "ui_leaderboard.h"
#include <QFile>
#include <QGraphicsTextItem>

Leaderboard::Leaderboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Leaderboard)
{
    ui->setupUi(this);
    QGraphicsView * view = ui->graphicsView;
    auto scene = new QGraphicsScene;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    QString filename = "/Users/oliviagolden/HW4/leader_board.txt";
    QFile file(filename);
    if (file.open(QFile::ReadOnly)){
        QTextStream in(&file);
        int i=0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            QGraphicsTextItem *text = scene->addText(line);
            text->setPos(0,i*15);
            i++;
        }
        file.close();
    }

}

Leaderboard::~Leaderboard()
{
    delete ui;
}


void Leaderboard::on_pushButton_clicked()
{
    this->close();
}

