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
	// TODO: deprecate
	double terminal_util(const std::vector<int>& cards, int infoset, const int lastActions, const int curPlayer) const;

	const int NUM_STREETS = 1;

	std::random_device dev;
	std::mt19937 rng { dev() };
};

#endif
