#include "Game.h"

void Game::set_holecard() {
	infoset[0] = (*cards)[cur_player()];
}

void Game::update_infoset(const int action) {
	
}

void Game::step(const int action) {
	infoset[infosetLen] = action;
	++infosetLen;
}

int Game::cur_player() {
	return (infosetLen % 2 == 0) ? 0 : 1;
}
