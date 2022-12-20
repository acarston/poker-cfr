#ifndef KUHNCFRBOT_H
#define KUHNCFRBOT_H

#include <unordered_map>
#include <string>
using namespace std;

#include "Node.cpp"

class KuhnCfrBot {
    public:
        unordered_map<string, Node> nodes;

        KuhnCfrBot();
        bool IsTerminal(string terminalHistory);
        double Cfr(vector<int> cards, string history = "", vector<double> reachProbabilities = {1.0, 1.0});
    private:
        const int TERMINAL_HISTORY_LENGTH = 2, PLAYER_1 = 0, PLAYER_2 = 1;

        Node GetNode(string history);
        void UpdateNodeRegrets(vector<double>& cumulativeRegrets);
};

#endif