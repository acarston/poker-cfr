#ifndef CFR_H
#define CFR_H

#include <vector>
#include <string>
#include <random>
#include <unordered_map>

#include "Node.h"

class CFR {
public:
	double mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int pot = 0, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);

	std::unordered_map<int, Node*> nodes;

private:
	bool winner(const int curPlayer, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int passedStreets) const;

	const int NUM_STREETS = 1;
	const int CARD_LEN = 2;
	const int ACTION_LEN = 3;

	std::random_device dev;
	std::mt19937 rng { dev() };
};

#endif
