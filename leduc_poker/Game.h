#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>

class Game {
private:
	int infosetLen = 0;
	const static int MAX_INFOSET_LEN = 3;
	std::vector<int>* cards = nullptr;

public:
	Game(std::vector<int>& cards) : cards(&cards) {};
	void set_holecard();
	void update_infoset(const int action);
	void step(const int action);
	int cur_player();

	std::array<int, MAX_INFOSET_LEN> infoset;
};

#endif

