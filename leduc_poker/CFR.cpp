#include "CFR.h"

double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int pot, int passedStreets, int sinceChance, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;

    double nodeUtil = 0.0;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset& (0b1 << sinceChance * ACTION_LEN) - 1;
    const int lastAction = lastActions & 0b111;

    for (int i = 0; i < passedStreets; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= streetCards[i] + 1;
    }
    
    // if raise-call or check-check, fold
    //if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
    if (lastActions == 0b011010 || lastActions == 0b001001) {
        //if (passedStreets < NUM_STREETS) {
        //    infoset <<= CARD_LEN;
        //    infoset ^= streetCards[passedStreets] + 1;
        //    sinceChance = 0;
        //    ++passedStreets;
        //}
        //else return (winner(curPlayer, holeCards, streetCards, passedStreets)) ? pot : -pot;

        return (winner(curPlayer, holeCards, streetCards, passedStreets)) ? pot : -pot; // TEST
    }
    else if (lastAction == 0b100) return -pot; // -pot ??
    ++sinceChance;

    infoset <<= CARD_LEN;
    infoset ^= holeCards[curPlayer] + 1;

    Node* node = this->nodes[infoset];
    if (node == nullptr) {
        node = new Node(lastActions, lastAction, passedStreets);
        this->nodes[infoset] = node;
    }

    infoset >>= passedStreets * CARD_LEN + CARD_LEN;
    infoset <<= ACTION_LEN;

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

bool CFR::winner(const int curPlayer, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int passedStreets) const {
    int playerCard = holeCards[curPlayer], oppCard = holeCards[!curPlayer];
    if (passedStreets) {
        for (int streetCard : streetCards) {
            if (playerCard == streetCard) return true;
            else if (oppCard == streetCard) return false;
        }
    }
    return playerCard > oppCard;
}
