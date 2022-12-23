#ifndef KUHNCFR_H
#define KUHNCFR_H

#include <unordered_map>
#include <string>
using namespace std;

#include "Node.cpp"

class KuhnCFR {
    public:
        unordered_map<string, Node> nodes;

        double CalculateUtilities(vector<int> cards, string history = "", vector<double> reachProbabilities = {1.0, 1.0});
    private:
        const int TERMINAL_HISTORY_LENGTH = 2, PLAYER_1 = 0, PLAYER_2 = 1;
        const vector<string> ACTIONS = {"b", "p"};
        const int NUM_ACTIONS = ACTIONS.size();

        double GetTerminalUtility(string history, vector<int> cards, double nodeUtility, int currentPlayer);
        Node& GetNode(string key);
};

#endif