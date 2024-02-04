#include "CFR.h"

double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int pot, int passedStreets, int sinceChance, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;

    double nodeUtil = 0.0;
    const int lastActions = (sinceChance > 1) ? infoset & 0b111111 : (infoset & 0b111111) >> (2 - sinceChance) * 3; // since chance????
    const int lastAction = lastActions & 0b111;

    for (int i = 0; i < passedStreets; ++i) {
        infoset <<= 2;
        infoset ^= streetCards[i] + 1;
    }
    
    // if raise-call or check-check, fold
    if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
        if (passedStreets < NUM_STREETS) {
            infoset <<= 2;
            infoset ^= streetCards[passedStreets] + 1;
            sinceChance = 0;
            ++passedStreets;
        }
        else return pot;
    }
    else if (lastAction == 0b100) return pot;
    ++sinceChance;

    infoset <<= 2;
    infoset ^= holeCards[curPlayer] + 1;

    Node* node = this->nodes[infoset];
    if (node == nullptr) {
        node = new Node(lastActions, lastAction, passedStreets); // TODO: pass in passedStreets for printing ease
        this->nodes[infoset] = node;
    }

    infoset >>= passedStreets * 2 + 2;
    infoset <<= 3;

    const double* nodeStrat = node->strategy();
    const int numActions = node->num_actions();
    const int* actions = node->get_actions();
    const double iterWeight = double(iteration) / (iteration + 1000);

    node->update_sum(iteration, iterWeight);

    if (curPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);

        const int action = actions[dist(this->rng)];
        if (action == 0b011) pot += 2 * passedStreets;
        if (lastAction == 0b011) pot *= 2;
        return -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
    }

    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        const int action = actions[i];
        if (action == 0b011) pot += 2 * passedStreets;
        if (lastAction == 0b011) pot *= 2;
        cfUtils[i] = -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
        nodeUtil += cfUtils[i] * nodeStrat[i];
    }

    double* cumulRegrets = node->cumul_regrets();
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;

    return nodeUtil;
}