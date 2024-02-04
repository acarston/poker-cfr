#include "CFR.h"
#include <iostream> //test, DELETE

// TODO: pass in pot = 0
double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int pot, int passedStreets, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;

    double nodeUtil = 0.0;
    const int lastActions = infoset & 0b111111 ;
    const int lastAction = lastActions & 0b111;
    // nodeUtil = terminal_util(holeCards, infoset, lastActions, curPlayer);
    // if (nodeUtil) return nodeUtil;
    
    // if raise-call or check-check, fold
    if (lastActions == 0b011010 || lastActions == 0b001001) {
        if (passedStreets < NUM_STREETS) {
            infoset <<= 2;
            infoset = infoset ^ streetCards[passedStreets] + 1;
            ++passedStreets;
        }
        else return pot;
    }
    else if (lastAction == 0b100) return pot;

    infoset <<= 2;
    infoset = infoset ^ holeCards[curPlayer] + 1;

    Node* node = this->nodes[infoset];
    if (node == nullptr) {
        node = new Node(lastActions, lastAction); // pass in infoset, passedStreets or just infosetCardless,
        this->nodes[infoset] = node;
    }

    infoset >>= passedStreets * 2 + 2;
    infoset <<= 3;

    const double* nodeStrat = node->strategy();
    const int numActions = node->num_actions(); // set in constructor
    const int* actions = node->get_actions(); // actions, returns an array of 0bs, set in constructor
    const double iterWeight = double(iteration) / (iteration + 1000);

    node->update_sum(iteration, iterWeight);

    if (curPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);

        // test block
        const int actionIndex = dist(this->engine);
        if (actionIndex >= numActions || actionIndex < 0) std::cerr << "Invalid action index: " << actionIndex << std::endl;

        // const int action = actions[dist(this->engine)]; // uncomment
        const int action = actions[actionIndex]; // comment
        if (action == 0b011) pot += 2 * passedStreets;
        if (lastAction == 0b011) pot *= 2;
        return -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, infoset ^ action, numPastActions + 1);
    }

    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        const int action = actions[i];
        if (action == 0b011) pot += 2 * passedStreets;
        if (lastAction == 0b011) pot *= 2;
        cfUtils[i] = -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, infoset ^ action, numPastActions + 1);
        nodeUtil += cfUtils[i] * nodeStrat[i];
    }

    double* cumulRegrets = node->cumul_regrets();
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;

    return nodeUtil;
}

// TODO: deprecate
double CFR::terminal_util(const std::vector<int>& cards, int infoset, const int lastActions, const int curPlayer) const {
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