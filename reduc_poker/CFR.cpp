#include "CFR.h"

int CFR::rank_hand(Card* cards) {
    const int handSize = 5;

    bool flush = true;
    bool straight = true;

    std::sort(cards, cards + handSize, [](Card& a, Card& b) { return a > b; });
    for (int i = 0; i < handSize - 1; ++i) {
        if (cards[0].suit != cards[i + 1].suit) flush = false;
        if (cards[i] - cards[i + 1] != 1) straight = false;
        if (!flush && !straight) break;
    }
    if (cards[0] == 14 && cards[1] == 5 && cards[2] == 4 && cards[3] == 3 && cards[4] == 2) straight = true;

    int frequencies[handSize]{};
    int kind = 1;
    for (int i = 0; i < handSize - 1; ++i) {
        if (cards[i] == cards[i + 1]) ++kind;
        else {
            ++frequencies[kind];
            kind = 1;
        }
    }
    ++frequencies[kind];

    if (flush && straight) return 8;
    if (frequencies[4]) return 7;
    if (frequencies[3] && frequencies[2]) return 6;
    if (flush) return 5;
    if (straight) return 4;
    if (frequencies[3]) return 3;
    if (frequencies[2] > 1) return 2;
    if (frequencies[2]) return 1;
    return 0;
}

// int CFR::terminal_util(const int curPlayer, std::vector<int>& pot, const std::vector<std::vector<Card>>& holeCards)

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
    if (!(action == 0b010 || action == 0b011)) return 0;

    int bet = pot[!curPlayer] - pot[curPlayer];
    if (action == 0b011) {
        if (passedStreets < 2) bet += 2;
        else bet += 4;
    }
    pot[curPlayer] += bet;
    return bet;
}

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

    // remove hole cards and street cards; prepare for next action
    infoset >>= passedStreets * CARD_LEN + CARD_LEN;
    infoset <<= ACTION_LEN;

    if (curPlayer != targetPlayer) {
        node->update_sum(iteration, iterWeight);

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