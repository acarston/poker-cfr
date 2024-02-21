#include "Trainer.h"
#include "CFR.h"

CFR::BoardPermutation& CFR::BoardPermutation::operator++() {
    if (i < 0 && j < 0) done = true;

    else if (j <= i || i == LAST_IDX) {
        std::swap(board[i], board[HAND_SIZE]);
        j = HAND_SIZE - 1;
        --i;
    }

    else {
        std::swap(board[j], board[HAND_SIZE + 1]);
        --j;
    }

    return *this;
}

CFR::BoardPermutation CFR::BoardPermutation::operator++(int) {
    BoardPermutation copy(*this);
    ++*this;
    return copy;
}

void CFR::clear_queue(std::queue<Card*>& q) {
    std::queue<Card*> empty;
    std::swap(q, empty);
}

int CFR::compare_hands(Card** hand1, Card** hand2) {
    for (int i = 0; i < HAND_SIZE; ++i) {
        if (*hand1[i] == *hand2[i]) continue;
        return (*hand1[i] > *hand2[i]) ? 1 : 0;
    }
    return -1;
}

int CFR::rank_hand(Card** const hand) {
    bool flush = true;
    bool straight = true;

    std::sort(hand, hand + HAND_SIZE, [](const Card* a, const Card* b) { return *a > *b; });

    for (int i = 0; i < HAND_SIZE - 1; ++i) {
        if (hand[0]->suit != hand[i + 1]->suit) flush = false;
        if (*hand[i] - *hand[i + 1] != 1) straight = false;
        if (!flush && !straight) break;
    }
    if (*hand[0] == 14 && *hand[1] == 5 && *hand[2] == 4 && *hand[3] == 3 && *hand[4] == 2) straight = true;

    std::queue<std::queue<Card*>> frequencies[HAND_SIZE]{};
    std::queue<Card*> tmpQueue;
    for (int i = 0; i < HAND_SIZE - 1; ++i) {
        if (*hand[i] == *hand[i + 1]) tmpQueue.push(hand[i]);
        else {
            tmpQueue.push(hand[i]);
            frequencies[tmpQueue.size()].push(tmpQueue);
            clear_queue(tmpQueue);
        }
    }
    tmpQueue.push(hand[HAND_SIZE - 1]);
    frequencies[tmpQueue.size()].push(tmpQueue);

    int rank = 1;
    if (flush && straight) rank = 9;
    else if (!frequencies[4].empty()) rank = 8;
    else if (!frequencies[3].empty() && !frequencies[2].empty()) rank = 7;
    else if (flush) rank = 6;
    else if (straight) rank = 5;
    else if (!frequencies[3].empty()) rank = 4;
    else if (frequencies[2].size() > 1) rank = 3;
    else if (!frequencies[2].empty()) rank = 2;

    int cardsIdx = 0;
    for (int i = HAND_SIZE - 1; i >= 0; --i) {
        std::queue<std::queue<Card*>>& cur = frequencies[i];
        while (!cur.empty()) {
            std::queue<Card*>& nested = cur.front();
            while (!nested.empty()) {
                hand[cardsIdx] = nested.front();
                nested.pop();
                ++cardsIdx;
            }
            cur.pop();
        }
    }

    return rank;
}

int CFR::terminal_util(const int curPlayer, const std::vector<int>& pot, Card*** const boards) {
    int bestRank[NUM_PLAYERS]{};
    Card* bestHand[NUM_PLAYERS][HAND_SIZE]{};
    Card* curHand[HAND_SIZE];

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        BoardPermutation permutation(boards[i]);

        while (permutation) {
            std::copy(boards[i], boards[i] + HAND_SIZE, curHand);

            int rank = rank_hand(curHand);
            if (rank > bestRank[i] || (rank == bestRank[i] && compare_hands(curHand, bestHand[i]) == 1)) {
                bestRank[i] = rank;
                std::copy(curHand, curHand + HAND_SIZE, bestHand[i]);
            }

            ++permutation;
        }
    }

    if (bestRank[curPlayer] != bestRank[!curPlayer]) return (bestRank[curPlayer] > bestRank[!curPlayer]) ? pot[!curPlayer] : -pot[curPlayer];
    int curPlayerBetter = compare_hands(bestHand[curPlayer], bestHand[!curPlayer]);
    return (curPlayerBetter == 1) ? pot[!curPlayer] : (curPlayerBetter == 0) ? -pot[curPlayer] : 0;
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

double CFR::mccfr(const int targetPlayer, const unsigned int iteration, Card*** const deal, Card*** const boards, std::vector<int> pot, int passedStreets, int sinceChance, unsigned long long infoset, int numPastActions) {
    const bool isEvenAction = numPastActions % 2 == 0;
    const int curPlayer = (passedStreets > 0) ? !isEvenAction : isEvenAction;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset & (0b1 << sinceChance * ACTION_LEN) - 1;
    const int lastAction = lastActions & 0b111;

    // TODO: flop gives three cards
    // add street cards to infoset
    if (passedStreets) {
        for (int i = 0; i < 3; ++i) {
            infoset <<= CARD_LEN;
            infoset ^= deal[NUM_PLAYERS][i]->id;
        }
        for (int i = 0; i < passedStreets - 1; ++i) {
            infoset <<= CARD_LEN;
            infoset ^= deal[NUM_PLAYERS][i + 3]->id;
        }
    }

    // TODO: allow 3 raises per street
    // return payoff if raise-call or check-check, fold (i.e. on terminal nodes)
    if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
        // add next street card to infoset
        if (passedStreets < NUM_STREETS) {
            if (passedStreets == 0) {
                for (int i = 0; i < 3; ++i) {
                    infoset <<= CARD_LEN;
                    infoset ^= deal[NUM_PLAYERS][passedStreets + i]->id;
                }
            }
            else {
                infoset <<= CARD_LEN;
                infoset ^= deal[NUM_PLAYERS][passedStreets]->id;
            }
            sinceChance = 0;
            ++passedStreets;
        }
        else return terminal_util(curPlayer, pot, boards);
    }
    else if (lastAction == 0b100) return pot[!curPlayer];
    ++sinceChance;

    // add curPlayer's hole cards to infoset
    for (int i = 0; i < NUM_HOLE_CARDS; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= deal[curPlayer][i]->id;
    }

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
    infoset >>= (passedStreets == 0) ? NUM_HOLE_CARDS * CARD_LEN : (passedStreets + 2 + NUM_HOLE_CARDS) * CARD_LEN;
    infoset <<= ACTION_LEN;

    if (curPlayer != targetPlayer) {
        node->update_sum(iteration, iterWeight);

        // sample a random action based on the current strategy
        std::discrete_distribution<int> dist(nodeStrat, nodeStrat + numActions);
        const int action = actions[dist(this->rng)];

        update_pot(pot, curPlayer, action, lastAction, passedStreets);
        return -mccfr(targetPlayer, iteration, deal, boards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
    }

    double nodeUtil = 0.0;
    double* cfUtils = new double[numActions];
    for (int i = 0; i < numActions; ++i) {
        const int action = actions[i];
        const int bet = update_pot(pot, curPlayer, action, lastAction, passedStreets);

        // TODO: pot needs to be copied instead of a pointer
        cfUtils[i] = -mccfr(targetPlayer, iteration, deal, boards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
        nodeUtil += cfUtils[i] * nodeStrat[i];
        pot[curPlayer] -= bet;
    }

    double* cumulRegrets = node->cumul_regrets();
    for (int i = 0; i < numActions; ++i) cumulRegrets[i] += (cfUtils[i] - nodeUtil) * iterWeight;
    delete[] cfUtils;

    return nodeUtil;
}