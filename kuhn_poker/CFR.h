#ifndef CFR_H
#define CFR_H

#include <vector>
#include <string>
#include <random>
#include <unordered_map>

#include "Node.h"
#include "Node2.h"

class CFR {
public:
	double mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<char>& cards, const std::string& history = "");

	// std::unordered_map<std::string, Node*> nodes;
	std::unordered_map<std::string, Node2*> nodes;

private:
	int cur_player(const std::string& history) const;
	double terminal_util(const std::vector<char>& cards, const std::string& history, const int currentPlayer) const;

	std::mt19937 engine;
};

#endif