#ifndef KUHNCFR_H
#define KUHNCFR_H

#include <unordered_map>
#include <string>
using namespace std;

#include "Node.h"

class KuhnCFR {
    public:
        unordered_map<string, Node> nodes;

        //change name to verb
        //calculateRegrets, AccumulateRegrets, AddRegrets
        double CalculateUtilities(vector<int> cards, string history = "", vector<double> reachProbabilities = {1.0, 1.0});
        Node& GetNode(string key); //both methods should be private
        double GetTerminalUtility(string history, vector<int> cards, double nodeUtility, int currentPlayer);
    private:
        const int TERMINAL_HISTORY_LENGTH = 2, PLAYER_1 = 0, PLAYER_2 = 1;
        const vector<string> ACTIONS = {"b", "p"};
        const int NUM_ACTIONS = ACTIONS.size();

        // double GetTerminalUtility(string history, vector<int> cards);
        //Node GetNode(string key);
};

#endif