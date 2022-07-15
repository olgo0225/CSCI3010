#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <vector>

class game
{
public:
    game(int players);
    int get_numb_players() {return numb_players_;};
    QVector<Player*> get_players(){return players_;};

private:
    int numb_players_;
    QVector<Player*> players_;
};

#endif // GAME_H
