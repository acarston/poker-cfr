#include "CFR.h"

// TODO: reorganize variable placements to make more sense
// TODO: remove 'bet' from pass-in
double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, const int lastBet, int passedStreets, int sinceChance, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset & (0b1 << sinceChance * ACTION_LEN) - 1;
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
        //else return terminal_util(curPlayer, pot, holeCards, streetCards, passedStreets);

        return terminal_util(curPlayer, pot, holeCards, streetCards, passedStreets); // TEST
    }
    else if (lastAction == 0b100) return pot[!curPlayer];
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
    const double* nodeStrat2 = nodeStrat + 1; // TEST
    const double* nodeStrat3 = nodeStrat + 2; // TEST
    const int numActions = node->num_actions();
    const int* actions = node->get_actions();
    const double iterWeight = double(iteration) / (iteration + 1000);

    node->update_sum(iteration, iterWeight); // should be done if curPlayer != targetPlayer ????

    if (curPlayer != targetPlayer) {
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);
        const int action = actions[dist(this->rng)];

        // TODO: make block a function
        int bet = 0;
        if (action == 0b011) {
            bet = 1 * (passedStreets + 1);
            // if (lastAction == 0b011) bet *= 2;
        }
        else if (action == 0b010) bet = pot[!curPlayer] - pot[curPlayer];
        pot[curPlayer] += bet;

        return -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, bet, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
    }

    double nodeUtil = 0.0;
    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        const int action = actions[i];

        int bet = 0;
        if (action == 0b011) {
            bet = 1 * (passedStreets + 1);
            // if (lastAction == 0b011) bet *= 2;
        }
        else if (action == 0b010) bet = pot[!curPlayer] - pot[curPlayer];
        pot[curPlayer] += bet;

        cfUtils[i] = -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, bet, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
        pot[curPlayer] -= bet;
        nodeUtil += cfUtils[i] * nodeStrat[i];
    }

    double* cumulRegrets = node->cumul_regrets();
    double* cumulRegrets2 = cumulRegrets + 1; // TEST
    double* cumulRegrets3 = cumulRegrets + 2; // TEST
    for (int i = 0; i < numActions; ++i) {
        auto test = (cfUtils[i] - nodeUtil); // TEST
        // cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;
        cumulRegrets[i] += test;
    }

    return nodeUtil;
}

int CFR::terminal_util(const int curPlayer, std::vector<int>& pot, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int passedStreets) const {
    int playerCard = holeCards[curPlayer], oppCard = holeCards[!curPlayer];
    if (passedStreets) {
        for (int streetCard : streetCards) {
            if (playerCard == streetCard) return pot[!curPlayer];
            else if (oppCard == streetCard) return -pot[curPlayer];
        }
    }
    return (playerCard == oppCard) ? 0 : (playerCard > oppCard) ? pot[!curPlayer] : -pot[curPlayer];
}


