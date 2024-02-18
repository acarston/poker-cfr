#ifndef CFR_H
#define CFR_H

#include <string>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <queue>

#include "Node.h"
#include "Card.h"

class CFR {
private:
	const static int NUM_STREETS = 5;
	const static int CARD_LEN = 6;
	const static int NUM_PLAYERS = 2;
	const static int ACTION_LEN = 3;
	const static int NUM_HOLE_CARDS = 2;
	const static int HAND_SIZE = 5;

	std::random_device dev;
	std::mt19937 rng{ dev() };

	class BoardPermutation : std::iterator<std::input_iterator_tag, Card**> {
	private:
		Card** board = nullptr;
		bool done = false;
		const int LAST_IDX = 4;
		int i = LAST_IDX;
		int j = LAST_IDX;

	public:
		BoardPermutation(Card** board) : board(board) {};

		explicit operator bool() const { return !done; }
		reference operator*() { return board; }

		BoardPermutation& operator++();
		BoardPermutation operator++(int);
	};

public:
	std::unordered_map<int, Node*> nodes;
	
	static void clear_queue(std::queue<Card*>& q);
	static int compare_hands(Card** hand1, Card** hand2);
	static int rank_hand(Card** cards);
	static int terminal_util(const int curPlayer, const int* const pot, Card*** const boards);
	static int update_pot(int* pot, const int curPlayer, const int action, const int lastAction, const int passedStreets);

	double mccfr(const int targetPlayer, const unsigned int iteration, const Card*** const deal, Card*** const boards, int* const pot, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);
};

#endif