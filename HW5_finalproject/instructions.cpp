#include "instructions.h"
#include "ui_instructions.h"
#include <QFile>
#include <QGraphicsTextItem>


instructions::instructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::instructions)
{
    ui->setupUi(this);
    QGraphicsView * view = ui->graphicsView;
    auto scene = new QGraphicsScene;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    QString filename = "/Users/oliviagolden/HW5/instructions.txt";
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

instructions::~instructions()
{
    delete ui;
}

void instructions::on_pushButton_clicked()
{
    this->close();
}

