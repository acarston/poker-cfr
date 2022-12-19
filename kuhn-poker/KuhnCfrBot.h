#ifndef KUHNCFRBOT_H
#define KUHNCFRBOT_H

#include <unordered_map>
#include <string>
using namespace std;

#include "Node.cpp"

class KuhnCfrBot {
    public:
        unordered_map<string, Node> nodes;

        double Cfr(vector<int> cards, string history, vector<double> reachProbs);
    private:
        bool IsTerminal(string history);
        int GetTerminalPayoff(string history);
        Node GetNode(string key);
};

#endif