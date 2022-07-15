#include "mainwindow.h"
#include "player.h"
#include "game.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QSlider>
#include <QRandomGenerator>
#include <QTimer>
#include <QPen>
#include <QPainter>
#include <QComboBox>
#include <QVector>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //initialize ui, view, and scene
    ui->setupUi(this);
    QGraphicsView * view = ui->graphicsView;
    scene = new QGraphicsScene;
    scene1 = new QGraphicsScene;
    QGraphicsView * view1 = ui->player_box;
    view1->setScene(scene1);
    view1->setSceneRect(0,0,view1->frameSize().width(),view1->frameSize().height());
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    //call new game
    new_game();
}

void MainWindow::new_game(){
    //create timers
    timer = new QTimer();
    player = new QTimer();
    player_undo = new QTimer();
    ui->label_4->setText("Roll: ");
    //restart game by setting game_ended to false
    game_ended=false;
    //clear players vector
    players_.clear();
    //create grid
    scene->clear();
    scene1->clear();
    //enable start button
    ui->Start->setDisabled(0);
    //intialize chutes and ladders vector
    QVector<int> l_c;
    QGraphicsView * view = ui->graphicsView;
    int x_10 = view->frameSize().height() / 10;
    int y_10 = view->frameSize().width() / 10;
    //add 10 vert and horiz lines
    for (int i=1; i<=10; i++){
        scene->addLine(0, i*x_10, view->frameSize().width(), i*x_10);
        scene->addLine(i*y_10, 0, i*y_10, view->frameSize().height());
    }
    //add numbers
    int counter=100;
    for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
            QString t = QString::number(counter);
            QGraphicsTextItem *text = scene->addText(t);
            text->setPos(j*40, i*40);
            counter-=1;
            slot_ *s1 = new slot_(counter, j*40+20, i*40+20);
            grid.append(s1);
            //add numbers to l_c array to deal with no repeat squares for chutes and ladders
            l_c.append(counter);
        }
    }
    //no chute/ladder should be at 100, 0, or 1
    l_c.erase(std::remove(l_c.begin(), l_c.end(), 100), l_c.end());
    l_c.erase(std::remove(l_c.begin(), l_c.end(), 0), l_c.end());
    l_c.erase(std::remove(l_c.begin(), l_c.end(), 1), l_c.end());

    //initialize 6 chutes
    for (int i=0; i<6; i++){
        int random = QRandomGenerator::global()->generate() % l_c.size();
        int a = l_c[random];
        //remove from array
        l_c.erase(std::remove(l_c.begin(), l_c.end(), a), l_c.end());
        random = QRandomGenerator::global()->generate() % l_c.size();
        int b = l_c[random];
        //remove from array
        l_c.erase(std::remove(l_c.begin(), l_c.end(), b), l_c.end());
        int start_grid;
        int end_grid;
        //chute must have higher starting  point
        if (a>b){
            start_grid=a;
            end_grid=b;
        }
        else{
            start_grid=b;
            end_grid=a;
        }
        chutes* c1 = new chutes(get_slot_x(start_grid), get_slot_x(end_grid), get_slot_y(start_grid), get_slot_y(end_grid), start_grid, end_grid);
        chutes_.append(c1);
        scene->addItem(c1);
    }
    //initalize 7 ladders
    for (int i=0; i<7; i++){
        int random = QRandomGenerator::global()->generate() % l_c.size();
        int a = l_c[random];
        l_c.erase(std::remove(l_c.begin(), l_c.end(), a), l_c.end());
        random = QRandomGenerator::global()->generate() % l_c.size();
        int b = l_c[random];
        l_c.erase(std::remove(l_c.begin(), l_c.end(), b), l_c.end());
        int start_grid;
        int end_grid;
        //ladders must have higher ending point
        if (a<b){
            start_grid=a;
            end_grid=b;
        }
        else{
            start_grid=b;
            end_grid=a;
        }
        ladders* l1 = new ladders(get_slot_x(start_grid), get_slot_x(end_grid), get_slot_y(start_grid), get_slot_y(end_grid), start_grid, end_grid);
        ladders_.append(l1);
        scene->addItem(l1);
    }
    ui->skipped->setText("");
    //before game starts buttons should be disabled
    ui->numb_players->setDisabled(1); //number of players slider
    ui->numb_play_button->setDisabled(1);  //number of players button
    ui->pushButton_4->setDisabled(1); //undo button
    ui->pushButton_5->setDisabled(1);  //roll button
    ui->pushButton_6->setDisabled(1);  //reroll button
    ui->pushButton_7->setDisabled(1);  //leaderboard button
    ui->pushButton_8->setDisabled(1);  //quit button
    ui->next_player->setDisabled(1);  //next player turn button
    ui->move->setDisabled(1); // move button
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::get_slot_x(int slot){
    //gets x value of slot
    return grid[100-slot]->get_x();
}

int MainWindow::get_slot_y(int slot){
    //gets y value of slot
    return grid[100-slot]->get_y();
}

chutes* MainWindow::get_chute(){
    int x = current_player->get_x();
    int y = current_player->get_y();
    for(int i=0; i<chutes_.size(); i++){
        //if player's x and y are both within 30 of chute return chute
        if (abs(x-chutes_[i]->get_x_start())<=30 && abs(y-chutes_[i]->get_y_start())<=30){
            qDebug() << "Found chute";
             ui->skipped->setText("Player: " + current_player->get_user() + " found a chute.");
            return chutes_[i];
        }
    }
    return NULL;
}

ladders* MainWindow::get_ladder(){
    int x = current_player->get_x();
    int y = current_player->get_y();
    for(int i=0; i<ladders_.size(); i++){
        //if player's x and y are both within 30 of ladder return ladder
        if (abs(x-ladders_[i]->get_x_start())<=30 && abs(y-ladders_[i]->get_y_start())<=30){
            qDebug() << "Found ladder";
            ui->skipped->setText("Player: " + current_player->get_user() + " found a ladder.");
            return ladders_[i];
        }
    }
    return NULL;
}

void MainWindow::on_Start_clicked()
{
    //when start game is clicked
    ui->numb_players->setDisabled(0);  //enable numb_players slider
    ui->numb_play_button->setDisabled(0);  //enable numb_players button
    ui->numb_players->setMinimum(2);  //set minimum of players slider to 2
    ui->numb_players->setMaximum(4); //set maximum of players slider to 2

}


void MainWindow::updateCountdown(){
    timer_count+=1;  //increase counter by one every second
    ui->label_6->setText(QString::number(timer_count));  //update text
    if (timer_count==10){  //if count down is at 10 seconds
        get_player_timer();  //get next player
        timer_count=0;  //reset countdown
    }
}

void MainWindow::on_numb_play_button_clicked()
{
    //when number of players has been selected
    //set number of players to be value of slider
    numb_players=ui->numb_players->value();
    play = new PlayerNames;
    qDebug() << numb_players;
    //connect signal to when all names have been added
    connect(play, &PlayerNames::names_added, this, &MainWindow::AddPlayers);
    play->set_players(numb_players);
    play->addWidgets();
    play->show();

}

void MainWindow::AddPlayers(){
    //get inputted names
    QVector<QString> names = play->get_names();
    for (int i=0; i<numb_players; i++){
        //generate random colors
        int r = QRandomGenerator::global()->generate() % 256;
        int g = QRandomGenerator::global()->generate() % 256;
        int b = QRandomGenerator::global()->generate() % 256;
        QColor c(r, g, b);
        QString user = QString::number(i+1);
        //initalize Player with random color, user number, and name
        Player * p1 = new Player(c,user,names[i]);
        players_.append(p1);
        scene1->addItem(p1);

    }
    qDebug() << "Players added";
    ui->numb_players->setDisabled(1);  //disable numb players slider
    ui->numb_play_button->setDisabled(1);  //disable numb players button
    current_player=players_[0];  //set current player to first player
    std::string text = "Player " + current_player->get_user().toStdString();
    ui->label_5->setText(text.c_str());
    ui->pushButton_5->setDisabled(0);  //enable roll
    ui->pushButton_7->setDisabled(0);  //enable quit
    ui->pushButton_8->setDisabled(0);  //enable leaderboard
    timer_();  //begin timer
}

void MainWindow::timer_(){
    timer_count=0;  //start timer count at 0
    timer->start(1000);  //interval to one second
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCountdown()));  //when timer times out, updateCountdown is called
}

void MainWindow::on_pushButton_5_clicked()
{
    //ROLL
    int roll = QRandomGenerator::global()->generate() % ((12+1)-1)+1;  //roll a number between 1-12
    std::string text = "Roll: "+ std::to_string(roll);
    ui->label_4->setText(text.c_str());
    ui->pushButton_5->setDisabled(1);  //player should not be able to roll again for that turn
    current_player->set_roll(roll);  //set player's roll
    if (!current_player->get_active()){  //if player is not on board
        if (roll==6){ //must roll an exact 6
            current_player->set_active(true);  //now on board
            current_player->set_y(375);  //set to starting position
            current_player->set_x(375);
            scene->addItem(current_player);  //add player to scene
            get_player();  //move to next player
            ui->pushButton_5->setDisabled(0);  //next player can roll
            return;
        }
        else{
            get_player();  //if not exact 6 is rolled
            ui->pushButton_5->setDisabled(0);  //next player can roll
            return;
        }
    }
    else{
        ui->next_player->setDisabled(0); //player should be able to choose when they are done with their turn
        ui->move->setDisabled(0); //player can move or reroll
        return;
    }

}

void MainWindow::move_helper(){
    player_timer=0;
    player = new QTimer();
    player->start(350);
    connect(player, SIGNAL(timeout()), this, SLOT(updateBoard()));
    //while moving disable buttons
    ui->pushButton_4->setDisabled(1); //undo button
    ui->pushButton_5->setDisabled(1);  //roll button
    ui->pushButton_6->setDisabled(1);  //reroll button
    ui->pushButton_7->setDisabled(1);  //leaderboard button
    ui->pushButton_8->setDisabled(1);  //quit button
    ui->next_player->setDisabled(1);  //next player turn button
    ui->move->setDisabled(1); // move button
    timer->stop();
}

void MainWindow::updateBoard(){
    if (player_timer==current_player->get_roll()){
        if (!is_end()){
            player->stop();
            chutes* c2 = get_chute();  //check if chute or ladder was encountered at end of move
            ladders* l2 = get_ladder();
            if (c2!=NULL){  //if chute
                current_player->set_ladder(false);
                current_player->set_chute(true);  //to know last move was down a chute
                scene->removeItem(current_player);
                current_player->set_x(c2->get_x_end());  //move to end of chute
                current_player->set_y(c2->get_y_end());
                scene->addItem(current_player);
                scene->update();
            }
            else if (l2!=NULL){  //if ladder
                current_player->set_ladder(true); //to know last move was up a ladder
                current_player->set_chute(false);
                scene->removeItem(current_player);
                current_player->set_x(l2->get_x_end()); //move to end of ladder
                current_player->set_y(l2->get_y_end());
                scene->addItem(current_player);
                scene->update();
            }
            else{
                current_player->set_ladder(false);
                current_player->set_chute(false);
            }
            //after move enable buttons
            ui->pushButton_7->setDisabled(0);  //leaderboard button
            ui->pushButton_8->setDisabled(0);  //quit button
            ui->next_player->setDisabled(0);  //next player turn button
            can_undo();
            if (!game_ended){
                timer->start();
            }
            qDebug() << "move player called ending";
            is_end();  //run end game
        }

    }
    else{
        QGraphicsView * view = ui->graphicsView;
        scene->removeItem(current_player); //remove current player from scene
        current_player->set_x(current_player->get_x()-40);  //move player to the left
        player_timer+=1;
        if (current_player->get_x()>=view->frameSize().width() || current_player->get_x()<0){ //if player is at the end of the board
            current_player->set_x(375);
            current_player->set_y(current_player->get_y()-40);
        }
        else if ((current_player->get_x()-get_slot_x(100))<=30 && current_player->get_y()-get_slot_y(100)<=30){  //if player is at or past slot 100
            qDebug() << "Winner in function";
            current_player->set_x(get_slot_x(100));
            current_player->set_y(get_slot_y(100));
            scene->addItem(current_player);  //add player where it should be
            scene->update();
            player->stop();
            timer->stop();
            qDebug() <<"winner called end";
            is_end();  //run end game
            return;
         }
        scene->addItem(current_player);  //add player where it should be
        scene->update();
    }
}

void MainWindow::get_player(){
    ui->pushButton_5->setDisabled(0);
    ui->pushButton_4->setDisabled(1);
    timer_count=0;  //restart timer count
    player->stop();  //stop player timer
    QString user=QString::number(current_player->get_user().toInt()+1); //get next player
    if(user.toInt()>numb_players){ //if next player is beyond number of players, it is player 1s turn
        user=players_[0]->get_user();
    }
    else if (user.toInt()<=0){
        user=QString::number(numb_players);
    }
    for (int i=0; i<players_.size(); i++){
        if (players_[i]->get_user()==user){
            if (players_[i]->get_rerolls()>=6 || !players_[i]->get_active()){
                ui->pushButton_6->setDisabled(1);  //if player is out of rerolls
            }
            else{
                 ui->pushButton_6->setDisabled(0);
            }
            current_player=players_[i];
            std::string text_ = "Player " + current_player->get_user().toStdString();  //update player text
            ui->label_5->setText(text_.c_str());
            return;
        }
    }
    ui->pushButton_5->setDisabled(0); //enable rolling for next player
    for (int i=0; i<players_.size(); i++){
        user=QString::number(current_player->get_user().toInt());
        if (players_[i]->get_user()==user){
            if (players_[i+1]->get_rerolls()>=6 || !players_[i+1]->get_active()){
                ui->pushButton_6->setDisabled(1);  //if player is out of rerolls
            }
            else{
                 ui->pushButton_6->setDisabled(0);
            }
            current_player=players_[i+1];
            std::string text_ = "Player " + current_player->get_user().toStdString();  //update player text
            ui->label_5->setText(text_.c_str());
            return;
        }
    }
    return;
}

void MainWindow::get_player_timer(){
    ui->skipped->setText("Timer ran out for Player " + current_player->get_user());  //let users know which player was skipped
    get_player();
}

void MainWindow::on_pushButton_4_clicked()
{
    //UNDO
    current_player->increase_undos();  //increase player's undos
    int x= current_player->get_x();
    int y = current_player->get_y();
    scene->removeItem(current_player);
    if (current_player->get_ladder()){
        for (int i=0; i<ladders_.size(); i++){  //if player was at the end of a ladder move it down
            if(x==ladders_[i]->get_x_end() && y==ladders_[i]->get_y_end()){
                current_player->set_x(ladders_[i]->get_x_start());
                current_player->set_y(ladders_[i]->get_y_start());
            }
        }
    }
    else if (current_player->get_chute()){
        for (int i=0; i<chutes_.size(); i++){  //if player was at the end of a chute move it up
            if(x==chutes_[i]->get_x_end() && y==chutes_[i]->get_y_end()){
                current_player->set_x(chutes_[i]->get_x_start());
                current_player->set_y(chutes_[i]->get_y_start());
                scene->addItem(current_player);
            }
        }
    }
    player_timer_undo=0;  //restart player_undo timer
    player_undo = new QTimer();
    player_undo->start(250);
    connect(player_undo, SIGNAL(timeout()), this, SLOT(updateBoardUndo()));
    ui->pushButton_4->setDisabled(1);  //user should not be able to undo again that turn
    if (current_player->get_rerolls()<5){
        ui->pushButton_6->setDisabled(0);  //after undoing turn player has option to reroll if they still can
    }
    timer->stop();
}

void MainWindow::updateBoardUndo(){
    QGraphicsView * view = ui->graphicsView;

     if (player_timer_undo==current_player->get_roll()){  //once at end of previous roll
         player_undo->stop();  //stop
         timer->start();
     }
     else{
         scene->removeItem(current_player);
         current_player->set_x(current_player->get_x()+40);  //move player backwards
         if (current_player->get_x()>=view->frameSize().width() || current_player->get_x()<0){  //if at edge of board
             current_player->set_x(10);  //move to beginning of previous line
             current_player->set_y(current_player->get_y()+40);
         }
         scene->addItem(current_player);
         scene->update();
         player_timer_undo+=1;  //increase timer

     }



}

void MainWindow::on_pushButton_6_clicked()
{
    //REROLL
    int roll = QRandomGenerator::global()->generate() % ((12+1)-1)+1;  //should be able to reroll 1-12
    std::string text = "Roll: "+ std::to_string(roll);
    ui->label_4->setText(text.c_str());
    ui->pushButton_5->setDisabled(1); //should not be able to roll
    current_player->increase_rerolls(); //increase number of rerolls
    if (current_player->get_rerolls()>=6){
         ui->pushButton_6->setDisabled(1);
    }
    current_player->set_roll(roll);
    ui->move->setDisabled(0); // move button

}


void MainWindow::on_pushButton_8_clicked()
{
    //QUIT
    if (current_player->get_active()){  //if player is not on board
        scene->removeItem(current_player);  //remove current player from scene
        players_.erase(std::remove(players_.begin(), players_.end(), current_player), players_.end());  //remove current player from players array
        numb_players-=1;
        for (int i=0; i<players_.size(); i++){  //renumber players after one quits
            players_[i]->set_user(QString::number(i+1));
        }
        get_player();
        is_end();  //check if game can still be played
    }
    else{
        scene1->removeItem(current_player);
        players_.erase(std::remove(players_.begin(), players_.end(), current_player), players_.end());
        numb_players-=1;
        for (int i=0; i<players_.size(); i++){  //renumber players after one quits
            players_[i]->set_user(QString::number(i+1));
        }
          //remove current player from players array
        get_player();
        if(players_.size()<2){
            ui->skipped->setText("Too few players. GAME OVER");  //if too few players
            ui->numb_players->setDisabled(1); //number of players slider
            ui->numb_play_button->setDisabled(1);  //number of players button
            ui->pushButton_4->setDisabled(1); //undo button
            ui->pushButton_5->setDisabled(1);  //roll button
            ui->pushButton_6->setDisabled(1);  //reroll button
            ui->pushButton_8->setDisabled(1);  //quit button
            ui->next_player->setDisabled(1);  //next player turn button
            ui->move->setDisabled(1); // move button
            timer->stop();  //stop timer
        }
    }


}

bool MainWindow::is_end(){
    qDebug() << "ending called";
    if(players_.size()<2){
        ui->skipped->setText("Too few players. GAME OVER");  //if too few players
        ui->numb_players->setDisabled(1); //number of players slider
        ui->numb_play_button->setDisabled(1);  //number of players button
        ui->pushButton_4->setDisabled(1); //undo button
        ui->pushButton_5->setDisabled(1);  //roll button
        ui->pushButton_6->setDisabled(1);  //reroll button
        ui->pushButton_8->setDisabled(1);  //quit button
        ui->next_player->setDisabled(1);  //next player turn button
        ui->move->setDisabled(1); // move button
        ui->pushButton_7->setDisabled(0);  //leaderboard button
        timer->stop();  //stop timer
        return true;
    }
    else if (abs(current_player->get_x()-get_slot_x(100))<=30 && abs(current_player->get_y()-get_slot_y(100))<=30 && current_player->get_active()){  //if player is at slot 100
        timer->stop();
        player->stop();
        if (!game_ended){
            scene->removeItem(current_player);
            current_player->set_x(get_slot_x(100));
            current_player->set_y(get_slot_y(100));
            qDebug() << "Winner in is_end";
            scene->addItem(current_player);
            qDebug() << "winner added";
            player->stop();
            timer->stop();  //stop timers
            qDebug() << "timers stopped";
            current_player->add_win();
            for (int i=0; i<players_.size(); i++){  //run through current players
                bool player_added=false;  //set a false player_added
                for (int j=0; j<player_history.size(); j++){  //run through player history size
                    if (players_[i]->get_name()==player_history[j].get_name()){  //if player is a past player
                        player_history[j].add_game();  //add game
                        if (current_player->get_name()==player_history[j].get_name()){  //if player is the winner
                            player_history[j].add_win();  //add win
                        }
                        player_added=true;  //player was added
                    }
                }
                if (!player_added){  //other wise if player is not a past player
                    player_board p1(players_[i]->get_name(), players_[i]->get_wins(), 1);  //create a new player_board object
                    player_history.append(p1);
                }
            }
            qDebug() << "players games and wins added";
            ui->skipped->setText("Player " + current_player->get_user() + " won! GAME OVER");
            ui->numb_players->setDisabled(1); //number of players slider
            ui->numb_play_button->setDisabled(1);  //number of players button
            ui->pushButton_4->setDisabled(1); //undo button
            ui->pushButton_5->setDisabled(1);  //roll button
            ui->pushButton_6->setDisabled(1);  //reroll button
            ui->pushButton_8->setDisabled(1);  //quit button
            ui->next_player->setDisabled(1);  //next player turn button
            ui->move->setDisabled(1); // move button
            ui->pushButton_7->setDisabled(0);  //leaderboard button
            qDebug() << "buttons disabled";
            game_ended=true;
            scene1->clear();
            return true;
        }
    }
    return false;
}

void MainWindow::on_pushButton_2_clicked()
{
    //NEWGAME
    new_game();
}


void MainWindow::on_next_player_clicked()
{
    get_player();  //get next player
}


void MainWindow::on_move_clicked()
{
    move_helper();
    ui->pushButton_5->setDisabled(1);  //roll button
    ui->pushButton_6->setDisabled(1);  //reroll button
    ui->move->setDisabled(1); // move button
    ui->move->setDisabled(1); //can only move once per turn
    ui->skipped->setText("");
}

void MainWindow::can_undo(){
    if (current_player->get_undos()>=3 || !current_player->get_active()){
        ui->pushButton_4->setDisabled(1);  //if player is out of undos
    }
    else{
         ui->pushButton_4->setDisabled(0);
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    //END GAME
    ui->numb_players->setDisabled(1); //number of players slider
    ui->numb_play_button->setDisabled(1);  //number of players button
    ui->pushButton_4->setDisabled(1); //undo button
    ui->pushButton_5->setDisabled(1);  //roll button
    ui->pushButton_6->setDisabled(1);  //reroll button
    ui->pushButton_8->setDisabled(1);  //quit button
    ui->next_player->setDisabled(1);  //next player turn button
    ui->move->setDisabled(1); // move button
    player->stop(); //player timer stopped
    timer->stop();  //stop timer
}

QVector<player_board> sorter(QVector<player_board> &p1){
    for (int i = 0; i < p1.size()-1; i++){
        for (int j = 0; j < p1.size()-i-1; j++){
            if (p1[j].get_percent_wins() < p1[j+1].get_percent_wins()){  //sort vector is descedning order with bubble sort
                player_board temp = p1[j];
                p1[j] = p1[j+1];
                p1[j+1] = temp;
            }
        }
    }
    return p1;
}


void MainWindow::on_pushButton_7_clicked()
{
    //LEADERBOARD
    QString filename = "/Users/oliviagolden/HW4/leader_board.txt";
    QFile file(filename);
    if(!file.exists()){
           qDebug() << "File <i>cannot</i> be found "<<filename;
       }else{
           qDebug() << filename<<" Opening...";
       }
    if (file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        player_history=sorter(player_history);
        qDebug() << "writing to" << filename;
        if (player_history.size()==0){
            stream << "No completed games yet.";
        }
        else{
            for (int i=0; i<10; i++){  //gets only first 10 players
                if (i<player_history.size()){
                    stream << "Player: " << player_history[i].get_name() << " Games Won: " <<  player_history[i].get_wins() << " Games Played: " << player_history[i].get_games() << " Percent Won: " << player_history[i].get_percent_wins() << "%\n";
                }
            }
        }
        qDebug() << "writing complete";
    }
    file.close();  //close file
    l1 = new Leaderboard();  //get new leaderboard;
    l1->show();
}



