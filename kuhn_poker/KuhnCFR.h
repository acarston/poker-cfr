#ifndef KUHNCFR_H
#define KUHNCFR_H

#include <unordered_map>
#include <string>
#include <math.h>
#include <random>

using namespace std;

#include "Node.h"

class KuhnCFR {
    public:
        unordered_map<string, Node> nodes;
        
        double CalculateUtilities(const unsigned int iteration, const vector<int>& cards, const string& history = "", const vector<double>& reachProbabilities = {1.0, 1.0});
        double MCCFR(const int targetPlayer, const unsigned int iteration, const vector<int>& cards, const string& history = "");
        // double NewMCCFR(const int targetPlayer, const unsigned int iteration, const vector<int>& cards, const string& history = "", const vector<double>& reachProbabilities = { 1.0, 1.0 });
    private:
        const int TERMINAL_HISTORY_LENGTH = 2, PLAYER_1 = 0, PLAYER_2 = 1;
        const vector<string> ACTIONS = {"b", "p"};
        const int NUM_ACTIONS = ACTIONS.size();

        std::mt19937 engine;

        double GetTerminalUtility(const string& history, const vector<int>& cards, const int currentPlayer);
        Node& GetNode(const string& key);
};

#endif