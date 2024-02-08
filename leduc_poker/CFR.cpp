#include "CFR.h"

int CFR::terminal_util(const int curPlayer, std::vector<int>& pot, const std::vector<int>& holeCards, const std::vector<int>& streetCards, int passedStreets) {
    int playerCard = holeCards[curPlayer], oppCard = holeCards[!curPlayer];
    if (passedStreets) {
        for (int streetCard : streetCards) {
            if (playerCard == streetCard) return pot[!curPlayer];
            else if (oppCard == streetCard) return -pot[curPlayer];
        }
    }
    return (playerCard == oppCard) ? 0 : (playerCard > oppCard) ? pot[!curPlayer] : -pot[curPlayer];
}

int CFR::update_pot(std::vector<int>& pot, const int curPlayer, const int action, const int lastAction, const int passedStreets) {
    int bet = 0;
    if (action == 0b011) {
        bet = 2 * (passedStreets + 1);
        if (lastAction == 0b011) bet *= 2;
    }
    else if (action == 0b010) bet = pot[!curPlayer] - pot[curPlayer];
    pot[curPlayer] += bet;
    return bet;
}

// TODO: reorganize variable placements to make more sense
double CFR::mccfr(const int targetPlayer, const unsigned int iteration, const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, int passedStreets, int sinceChance, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset & (0b1 << sinceChance * ACTION_LEN) - 1;
    const int lastAction = lastActions & 0b111;

    // add street cards to infoset
    for (int i = 0; i < passedStreets; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= streetCards[i] + 1;
    }
    
    // return payoff if raise-call or check-check, fold (i.e. on terminal nodes)
    if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
        // add next street card to infoset
        if (passedStreets < NUM_STREETS) {
            infoset <<= CARD_LEN;
            infoset ^= streetCards[passedStreets] + 1;
            sinceChance = 0;
            ++passedStreets;
        }
        else return terminal_util(curPlayer, pot, holeCards, streetCards, passedStreets);
    }
    else if (lastAction == 0b100) return pot[!curPlayer];
    ++sinceChance;

    // add 'curPlayer' hole card to infoset
    infoset <<= CARD_LEN;
    infoset ^= holeCards[curPlayer] + 1;

    Node* node = this->nodes[infoset];
    if (node == nullptr) {
        node = new Node(lastActions, lastAction, passedStreets);
        this->nodes[infoset] = node;
    }

    const double* nodeStrat = node->strategy();
    const int numActions = node->num_actions();
    const int* actions = node->get_actions();

    const double iterWeight = double(iteration) / (iteration + 1000000);
    node->update_sum(iteration, iterWeight);

    // remove hole cards and street cards; prepare for next action
    infoset >>= passedStreets * CARD_LEN + CARD_LEN;
    infoset <<= ACTION_LEN;

    if (curPlayer != targetPlayer) {
        // sample a random action based on the current strategy
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);
        const int action = actions[dist(this->rng)];

        update_pot(pot, curPlayer, action, lastAction, passedStreets);
        return -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
    }

    double nodeUtil = 0.0;
    std::vector<double> cfUtils(numActions);
    for (int i = 0; i < numActions; ++i) {
        const int action = actions[i];
        const int bet = update_pot(pot, curPlayer, action, lastAction, passedStreets);

        cfUtils[i] = -mccfr(targetPlayer, iteration, holeCards, streetCards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
        nodeUtil += cfUtils[i] * nodeStrat[i];
        pot[curPlayer] -= bet;
    }

    double* cumulRegrets = node->cumul_regrets();
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;

    return nodeUtil;
}
