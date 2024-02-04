#include "CFR.h"

double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& cards, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;

    double nodeUtil = 0.0;
    nodeUtil = terminal_util(cards, infoset, curPlayer);
    if (nodeUtil) return nodeUtil;

    infoset <<= 2;
    infoset = infoset ^ cards[curPlayer] + 1;

    Node* node = this->nodes[infoset];
    if (node == nullptr) {
        node = new Node();
        this->nodes[infoset] = node;
    }

    infoset >>= 2;
    infoset <<= 2;

    const double* nodeStrat = node->strategy();
    const int numActions = node->get_num_actions();
    const double iterWeight = double(iteration) / (iteration + 1000);

    node->update_sum(iteration, iterWeight);

    if (curPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);
        const int iAction = dist(this->rng);

        return -mccfr(targetPlayer, iteration, cards, infoset ^ iAction + 1, numPastActions + 1);
    }

    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        cfUtils[i] = -mccfr(targetPlayer, iteration, cards, infoset ^ i + 1, numPastActions + 1);
        nodeUtil += cfUtils[i] * nodeStrat[i];
    }

    double* cumulRegrets = node->get_cumul_regrets();
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;

    return nodeUtil;
}

double CFR::terminal_util(const std::vector<int>& cards, int infoset, const int curPlayer) const {
    if (!(infoset & 0b100 || infoset & 0b1000)) return 0;

    int payoff = 0;
    int terminalHist = infoset & 0b1111;

    if (terminalHist == 0b1001) return 0;
    if (terminalHist == 0b0110) return 1;
    if (terminalHist == 0b1010) payoff = 1;
    else payoff = 2;

    int loser = (cards[0] < cards[1]) ? 0 : 1;
    if (curPlayer == loser) payoff *= -1;

    return payoff;
}
