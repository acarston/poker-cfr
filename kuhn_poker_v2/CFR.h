#ifndef CFR_H
#define CFR_H

#include <vector>
#include <string>
#include <random>
#include <unordered_map>

#include "Node.h"

class CFR {
public:
	double mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& cards, int infoset = 0, int numPastActions = 0);

	std::unordered_map<int, Node*> nodes;

private:
	double terminal_util(const std::vector<int>& cards, int infoset, const int curPlayer) const;

	std::mt19937 engine;
};

#endif
