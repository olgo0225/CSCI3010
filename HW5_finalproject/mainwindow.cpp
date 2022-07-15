#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include<QRandomGenerator>
#include <QTimer>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //set up scenes
    ui->setupUi(this);

    game_scene = new QGraphicsScene;

    card_scene = new QGraphicsScene;

    player_scene = new QGraphicsScene;

    instructions_ = new instructions();

    //call new game to starts
    new_game();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_draw_card_clicked()
{
    QVector <QString> loc = {"triangle", "square", "hexagon", "pentagon"};
    card_scene->clear();
    int draw;
    //2 times more likely to draw regular card than location card
    int rand = QRandomGenerator::global()->generate() % 14;
    if (rand<4){  //draw a location card
        current_location_card=location_card::factory(loc[rand]);
        card_scene->addItem(current_location_card);
    }
    else{  //draw a regular card
        draw = QRandomGenerator::global()->generate() % deck2.size();
        current_card=deck2[draw];
        current_location_card=NULL;
        //create card to display
        card * c1 = new card(current_card.get_color(), current_card.get_upgrade());
        card_scene->addItem(c1);
        if (current_card.get_upgrade()){ //if card is an upgrade
            current_player->increase_moves();  //increase moves
            ui->notify->setText("Player " + QString::number(current_player->get_user()) + " got a Move 5 upgrade."); //display on UI
        }
    }
    ui->make_move->setDisabled(0);
    ui->draw_card->setDisabled(1);
}


void MainWindow::on_start_clicked()
{
    ui->numb_players->setDisabled(0);  //enable numb_players slider
    ui->submit->setDisabled(0);  //enable numb_players button
    ui->numb_players->setMinimum(2);  //set minimum of players slider to 2
    ui->numb_players->setMaximum(4); //set maximum of players slider to 2
    ui->start->setDisabled(1);
}


void MainWindow::on_submit_clicked()
{
    number_players=ui->numb_players->value();
    for (int i=0; i<number_players; i++){
        //generate random colors
        int r = QRandomGenerator::global()->generate() % 256;
        int g = QRandomGenerator::global()->generate() % 256;
        int b = QRandomGenerator::global()->generate() % 256;
        QColor c(r, g, b);
        //initalize Player with random color, user number, and name
        player * p1 = new player(c,i+1,"");
        players.append(p1);
        player_scene->addItem(p1);
    }
    ui->numb_players->setDisabled(1);
    ui->submit->setDisabled(1);
    ui->draw_card->setDisabled(0);
    ui->start->setDisabled(1);
    current_player=players[0];
    //add color coordinated text to update current player
    QColor color_text = current_player->get_color();
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::WindowText, color_text);
    ui->currentplayer->setPalette(palette);
    ui->currentplayer->setText("Player " + QString::number(current_player->get_user()));
}

void MainWindow::get_player(){
    int user=current_player->get_user()+1; //get next player
    if(user>number_players){ //if next player is beyond number of players, it is player 1s turn
        user=players[0]->get_user();
    }
    else if (user<=0){
        user=number_players;
    }
    for (int i=0; i<players.size(); i++){
        if (players[i]->get_user()==user){
            current_player=players[i];
            //enable buttons based on player's upgrades
            if (current_player->get_skips()>=1){
                ui->skip_turn->setDisabled(0);
            }
            else{
                ui->skip_turn->setDisabled(1);
            }
            if (current_player->get_moves()>=1){
                ui->move_5->setDisabled(0);
            }
            else{
                ui->move_5->setDisabled(1);
            }
            QColor color_text = current_player->get_color();
            QPalette palette = ui->label->palette();
            palette.setColor(QPalette::WindowText, color_text);
            ui->currentplayer->setPalette(palette);
            ui->currentplayer->setText("Player " + QString::number(current_player->get_user()));
            ui->draw_card->setDisabled(0);
            return;
        }
    }
}

void MainWindow::on_skip_turn_clicked()
{
    current_player->decrease_skips(); //player used a skip
    ui->make_move->setDisabled(1); //can now not make a move
    get_player(); //get next player
}



void MainWindow::on_make_move_clicked()
{
    if (!current_player->get_active()){  //if not on board yet
        player_scene->removeItem(current_player);
        current_player->set_x(375);
        current_player->set_y(375);
        game_scene->addItem(current_player);
        current_player->set_slot(0); //add to board and set slot as 0
        current_player->set_active(true);
    }
    if (current_location_card!=NULL){  //if location card was drawn

        QString name = current_location_card->get_name();
        for (int i=0; i<locations_.size(); i++){
            if (locations_[i]->get_name()==name){  //find location
                game_scene->removeItem(current_player);
                current_player->set_x(locations_[i]->get_x()); //set player location
                current_player->set_y(locations_[i]->get_y());
                game_scene->addItem(current_player);
                current_player->set_slot(locations_[i]->get_slot()); //update player slot
                get_player();  //get next player
                ui->draw_card->setDisabled(0);
                ui->make_move->setDisabled(1);
                return;
            }
        }
    }
    else{  //if regular card
        for (int i=current_player->get_slot(); i<grid_.size(); i++){ //iterate over grid
                if(grid_[i].get_color()==current_card.get_color()){ //find next tile with the same color
                    game_scene->removeItem(current_player);
                    current_player->set_x(grid_[i].get_x());  //update player location
                    current_player->set_y(grid_[i].get_y());
                    game_scene->addItem(current_player);
                    current_player->set_slot(grid_[i].get_slot());
                    if (grid_[i].get_upgrade()){  //if upgrade was found
                        current_player->increase_skips();
                        ui->notify->setText("Player " + QString::number(current_player->get_user()) + " recieved a skip upgrade.");
                    }
                    else{
                        ui->notify->setText("");
                    }
                    if(is_end()){
                        return; //check is player has reached end
                    }
                    get_player();
                    ui->make_move->setDisabled(1);
                    ui->draw_card->setDisabled(0);

                    return;
                }
        }
    }
    //if no tile was found
    ui->notify->setText("Player " + QString::number(current_player->get_user()) + " cannot move.");
    ui->make_move->setDisabled(1);
    get_player();
    return;
}


void MainWindow::on_new_game_clicked()
{
    new_game();
}

void MainWindow::new_game(){

    //clear vectors
    deck2.clear();
    deck_location.clear();
    players.clear();
    tiles.clear();
    grid_.clear();
    locations_.clear();
    current_location_card=NULL;
    game_scene->clear();
    player_scene->clear();
    card_scene->clear();
    QColor color_text(0,0,0);
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::WindowText, color_text);
    ui->currentplayer->setPalette(palette);
    ui->currentplayer->setText("Current player:");
    ui->notify->setText("");
    //create grid
    QVector <QColor> colors;
    for (int k=0; k<5; k++){
        int r = QRandomGenerator::global()->generate() % 256;
        int g = QRandomGenerator::global()->generate() % 256;
        int b = QRandomGenerator::global()->generate() % 256;
        QColor c(r, g, b);
        colors.append(c);
    }
    QGraphicsView * view = ui->graphicsView;
    view->setScene(game_scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    QGraphicsView * view_card = ui->card_view;
    view_card->setScene(card_scene);
    view_card->setSceneRect(0,0,view_card->frameSize().width(),view_card->frameSize().height());
    QGraphicsView * view_play = ui->players_view;
    view_play->setScene(player_scene);
    view_play->setSceneRect(0,0,view_play->frameSize().width(),view_play->frameSize().height());

    int x_10 = view->frameSize().height() / 10;
    int y_10 = view->frameSize().width() /10;
    //add 10 vert and horiz lines
    for (int i=1; i<=10; i++){
        game_scene->addLine(0, i*x_10, view->frameSize().width(), i*x_10);
        game_scene->addLine(i*y_10, 0, i*y_10, view->frameSize().height());
    }
    //generate random positions for locations
    int tri = 1 + QRandomGenerator::global()->generate() % 24; //location cards can only be generated in speicifc locations to avoid overlap
    int square = 25 + QRandomGenerator::global()->generate() % 25;
    int pent = 50 + QRandomGenerator::global()->generate() % 25;
    int hex = 75 + QRandomGenerator::global()->generate() % 25;
    QVector <int> locations = {tri,square,pent,hex};
    int counter=100;
    //generate game board tiles
    for (int i=0; i<10; i++){
            for (int j=0; j<10; j++){
                if (counter==locations[0]){
                    location *l1 = new location(j*40, i*40,"triangle", counter);
                    game_scene->addItem(l1);
                    locations_.append(l1);
                    grid g1(0, 0, counter, QColor(0,0,0), false);
                    grid_.append(g1);
                }
                else if (counter==locations[1]){
                    location *l1 = new location(j*40, i*40,"square", counter);
                    game_scene->addItem(l1);
                    locations_.append(l1);
                    grid g1(0, 0, counter, QColor(0,0,0), false);
                    grid_.append(g1);
                }
                else if (counter==locations[2]){
                    location *l1 = new location(j*40, i*40,"pentagon", counter);
                    game_scene->addItem(l1);
                    locations_.append(l1);
                    grid g1(0, 0, counter, QColor(0,0,0), false);
                    grid_.append(g1);
                }
                else if (counter==locations[3]){
                    location *l1 = new location(j*40, i*40, "hexagon", counter);
                    game_scene->addItem(l1);
                    locations_.append(l1);
                    grid g1(0, 0, counter, QColor(0,0,0), false);
                    grid_.append(g1);
                }
                else{
                    int rand = QRandomGenerator::global()->generate() % 100;
                    int color = QRandomGenerator::global()->generate() % colors.size();
                    if (rand<10 && counter!=100){
                        tile * t1 = new tile(j*40, i*40, colors[color], true, counter); //generate update tiles
                        grid g1(j*40, i*40, counter, colors[color], true);
                        grid_.append(g1);
                        game_scene->addItem(t1);
                    }else{ //regular tiles
                    tile * t1 = new tile(j*40, i*40, colors[color], false, counter);
                    grid g1(j*40, i*40, counter, colors[color], false);
                    grid_.append(g1);
                    game_scene->addItem(t1);
                }
            }
            counter-=1;
        }

    }

    QGraphicsTextItem *text = game_scene->addText("END");
    text->setPos(0,0);
    QGraphicsTextItem *text1 = game_scene->addText("START");
    text1->setPos(357,357);
    std::reverse(grid_.begin(), grid_.end());

    //generate cards
    for (int j=0; j<colors.size(); j++){
        int rand = QRandomGenerator::global()->generate() % 100;
        if (rand<15){
            cards c1(colors[j], true); //update cards
            deck2.append(c1);
        }
        else{
            cards c1(colors[j], false);
            deck2.append(c1);
        }

    }

    //change colors of candyland name basaed on color themes
    int rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text1 = colors[rand];
    QPalette palette1 = ui->label->palette();
    palette1.setColor(QPalette::WindowText, color_text1);
    ui->C->setPalette(palette1);
    ui->C->setText("C");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text2 = colors[rand];
    QPalette palette2 = ui->label->palette();
    palette2.setColor(QPalette::WindowText, color_text2);
    ui->A->setPalette(palette2);
    ui->A->setText("A");
    ui->A1->setPalette(palette2);
    ui->A1->setText("A");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text3 = colors[rand];
    QPalette palette3 = ui->label->palette();
    palette3.setColor(QPalette::WindowText, color_text3);
    ui->N->setPalette(palette3);
    ui->N->setText("N");
    ui->N1->setPalette(palette3);
    ui->N1->setText("N");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text4 = colors[rand];
    QPalette palette4 = ui->label->palette();
    palette4.setColor(QPalette::WindowText, color_text4);
    ui->D->setPalette(palette4);
    ui->D->setText("D");
    ui->D1->setPalette(palette4);
    ui->D1->setText("D");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text5 = colors[rand];
    QPalette palette5 = ui->label->palette();
    palette5.setColor(QPalette::WindowText, color_text5);
    ui->Y->setPalette(palette5);
    ui->Y->setText("Y");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text6 = colors[rand];
    QPalette palette6 = ui->label->palette();
    palette6.setColor(QPalette::WindowText, color_text6);
    ui->L->setPalette(palette6);
    ui->L->setText("L");
    rand = QRandomGenerator::global()->generate() % colors.size();
    QColor color_text7 = colors[rand];
    QPalette palette7 = ui->label->palette();
    palette7.setColor(QPalette::WindowText, color_text7);
    ui->N->setPalette(palette7);
    ui->N->setText("N");

    //disable buttons that should be disabled
    ui->start->setDisabled(0);
    ui->skip_turn->setDisabled(1);
    ui->move_5->setDisabled(1);
    ui->numb_players->setDisabled(1);
    ui->draw_card->setDisabled(1);
    ui->submit->setDisabled(1);
    ui->make_move->setDisabled(1);
    ui->start->setDisabled(0);
}

void MainWindow::on_move_5_clicked()
{
    current_player->decrease_moves(); //player used a moves
    timer = new QTimer;
    timer_count=0;
    timer->start(350);  //start timer
    connect(timer, SIGNAL(timeout()), this, SLOT(move_helper()));  //each time timer times out call move_helper
    ui->make_move->setDisabled(1);
    ui->move_5->setDisabled(1);


}

void MainWindow::move_helper(){
    if (timer_count==5){  //if player has moved 5 times
        timer->stop();
        if (grid_[current_player->get_slot()].get_upgrade()){  //if upgrade was found
            current_player->increase_skips();
            ui->notify->setText("Player " + QString::number(current_player->get_user()) + " recieved a skip upgrade.");
        }
        get_player();
        is_end();
    }
    else{
        if(current_player->get_slot()==100){ //if player is at end
            is_end();
            timer->stop();
            return;
        }
        if(!is_end()){
            game_scene->removeItem(current_player);
            current_player->set_slot(current_player->get_slot()+1);
            current_player->set_x(grid_[current_player->get_slot()-1].get_x());
            current_player->set_y(grid_[current_player->get_slot()-1].get_y());
            game_scene->addItem(current_player);  //move player along board
        }
        timer_count+=1;
    }
}


void MainWindow::on_end_game_clicked()
{
    //clear vectors
    players.clear();
    player_scene->clear();
    card_scene->clear();
    QColor color_text(0,0,0);
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::WindowText, color_text);
    ui->currentplayer->setPalette(palette);
    ui->currentplayer->setText("Current player:");
    ui->notify->setText("");
    ui->draw_card->setDisabled(1); //cannot draw card
}


void MainWindow::on_instructions_clicked()
{
    instructions_->show();
}

bool MainWindow::is_end(){
    if (current_player->get_slot()==100){  //if player reaches the end
        players.clear();
        player_scene->clear();
        card_scene->clear();
        QColor color_text(0,0,0);
        QPalette palette = ui->label->palette();
        palette.setColor(QPalette::WindowText, color_text);
        ui->currentplayer->setPalette(palette);
        ui->currentplayer->setText("Current player:");
        ui->draw_card->setDisabled(1);
        qDebug() << "winner";
        w1 = new winner();  //create new winner dialog
        connect(w1, &winner::quit, this, &MainWindow::on_quit_clicked); //if user chooses to quit
        connect(w1, &winner::new_game, this, &MainWindow::new_game); //if user chooses new game
        w1->set_player("Player "+ QString::number(current_player->get_user()) + " wins!");
        w1->show();
        return true;
    }
    else{
        return false;
    }
}

void MainWindow::on_quit_clicked()
{
    this->close();
}

