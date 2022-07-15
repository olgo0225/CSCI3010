#include "game.h"
#include <QRandomGenerator>

game::game (int players){
    numb_players_=players;
    for (int i=0; i<numb_players_; i++){
        //Player* p1 = new Player(QColor(255,0,0), QString::number(i));
        //players_.append(p1);
    }
}
