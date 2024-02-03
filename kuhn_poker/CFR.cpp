#include "CFR.h"

double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<char>& cards, const std::string& history) {
    const int curPlayer = cur_player(history);

    double nodeUtil = 0.0;
    nodeUtil = terminal_util(cards, history, curPlayer);
    if (nodeUtil != 0.0) return nodeUtil;

    std::string infoset = cards[curPlayer] + history;
    // Node* node = this->nodes[infoset];
    Node2* node = this->nodes[infoset];
    if (node == nullptr) {
        // node = new Node(); // params?
        node = new Node2(history);
        this->nodes[infoset] = node;
    }

    const double iterWeight = double(iteration) / (iteration + 1000);
    // const double* nodeStrat = node->Strategy(iteration, iterWeight);
    const double* nodeStrat = node->strategy(iteration, iterWeight);
    const int numActions = 2;
    const char* actions = new char[2] { 'b', 'p' };

    // node->update_sum(iteration, iterWeight); // where?

    if (curPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);
        int iAction = dist(this->engine);

        return -mccfr(targetPlayer, iteration, cards, (history + actions[iAction]));
    }

    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        cfUtils[i] = -mccfr(targetPlayer, iteration, cards, (history + actions[i]));
        nodeUtil += cfUtils[i] * nodeStrat[i];
    }

    double* cumulRegrets = node->get_cumul_regrets();
    // for (int i = 0; i < numActions; ++i) node->cumulativeRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;

    return nodeUtil;
}

int CFR::cur_player(const std::string& history) const {
    return (history.size() % 2 == 0) ? 0 : 1;
}

// TODO: cleanup, optimize
double CFR::terminal_util(const std::vector<char>& cards, const std::string& history, const int currentPlayer) const {
    if (history.size() < 2) return 0;

    int terminalHistoryStart = history.size() - 2;
    std::string terminalHistory = history.substr(terminalHistoryStart, 2);

    int nodeUtility = 0;
    if (terminalHistory == "bp") return nodeUtility = 1;
    else if (terminalHistory == "bb") nodeUtility = 2;
    else if (terminalHistory == "pp") nodeUtility = 1;

    int loser = (cards[0] < cards[1]) ? 0 : 1;
    if (currentPlayer == loser) nodeUtility *= -1;

    return nodeUtility;
}
