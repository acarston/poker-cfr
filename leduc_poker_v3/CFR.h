#ifndef CFR_H
#define CFR_H

#include <vector>
#include <string>
#include <random>
#include <unordered_map>

#include "Node.h"

class CFR {
public:
	double mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, const int lastBet = 0, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);

	std::unordered_map<int, Node*> nodes;

private:
	int terminal_util(const int curPlayer, std::vector<int>& pot, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int passedStreets) const;
	double terminal_util_2(const std::vector<int>& cards, int infoset, const int curPlayer) const;

	const int NUM_STREETS = 1;
	const int CARD_LEN = 2;
	const int ACTION_LEN = 3;

	std::random_device dev;
	std::mt19937 rng{ dev() };
};

#endif
