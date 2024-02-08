#include "Trainer.h"

Trainer::Trainer() {
    for (int i = 0; i < DECK_SIZE; ++i) {
        for (int j = 0; j < DECK_REPEATS; ++j) this->deck.push_back(i);
    }
}

/* use CFR starting with a predetermined random deal for a number of iterations */
void Trainer::train(const unsigned int iterations) {
    this->iterations = iterations;

    for (unsigned int i = 0; i < iterations; ++i) {
        shuffle();
        const int targetPlayer = (i % 2 == 0) ? 0 : 1;
        std::vector<int> pot(2, 1);
        bot.mccfr(targetPlayer, i + 1, deal[0], deal[1], pot);
    }
}

void Trainer::display_strats() const {
    std::cout << "Average utility of the root node:\n" << (rootNodeUtil / iterations) << "\n\n";
    std::cout << "Node Strategies:\n";

    std::unordered_map<int, std::string> options { { 0b001, "check" }, { 0b010, "call" }, { 0b011, "raise" }, { 0b100, "fold" } };

    double* strategy = nullptr;
    int* actions = nullptr;
    for (auto it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        auto infoset = it->first;
        auto& node = it->second;

        int holeCard = infoset & 0b11;
        infoset >>= 2;
        int streetCard = 0;
        if (node->passed_streets() > 0) {
            streetCard = infoset & 0b11;
            infoset >>= 2;
        }

        std::cout << "hole card: " << holeCard << "  ";
        std::cout << "street card: " << streetCard << "  ";
        std::cout << "ACTIONS: ";

        while (infoset) {
            std::cout << options[infoset & 0b111] << " ";
            infoset >>= 3;
        }

        strategy = node->avg_strategy();
        actions = node->get_actions();

        std::cout << " STRATEGY: ";
        for (int i = 0; i < node->num_actions(); ++i) std::cout << options[actions[i]] << ": " << strategy[i] << "  ";

        // std::cout << " REGRETS: "; // TEST
        // for (int i = 0; i < node->num_actions(); ++i) std::cout << node->cumul_regrets()[i] << "  "; // TEST
        // std::cout << " STRATEGY: ";
        // for (int i = 0; i < node->num_actions(); ++i) std::cout << options[actions[i]] << ": " << node->strategy()[i] << "  ";

        std::cout << "\n";
    }
    std::cout << "\n\nThis program was trained for " << iterations << " iterations." << "\n\n";
}

void Trainer::cont_explore(const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, int passedStreets, int sinceChance, int infoset, int numPastActions) {
    const int curPlayer = (numPastActions % 2 == 0) ? 0 : 1;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset & (0b1 << sinceChance * ACTION_LEN) - 1;
    const int lastAction = lastActions & 0b111;

    for (int i = 0; i < passedStreets; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= streetCards[i] + 1;
    }

    std::cout << "POT:  " << pot[curPlayer] + pot[!curPlayer] << "\n";

    // if raise-call or check-check, fold
    if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
        // if (lastActions == 0b011010 || lastActions == 0b001001) {
        if (passedStreets < NUM_STREETS) {
            infoset <<= CARD_LEN;
            infoset ^= streetCards[passedStreets] + 1;
            sinceChance = 0;
            ++passedStreets;
        }
        else {
            std::cout << "PLAYER 1 PAYOFF:  " << bot.terminal_util(curPlayer, pot, holeCards, streetCards, passedStreets) << "\n\n\n";
            return;
        }

        // return terminal_util(curPlayer, pot, holeCards, streetCards, passedStreets); // TEST
    }
    else if (lastAction == 0b100) {
        std::cout << "PLAYER 1 PAYOFF:  " << pot[!curPlayer] << "\n\n\n";
        return;
    }
    ++sinceChance;

    infoset <<= CARD_LEN;
    infoset ^= holeCards[curPlayer] + 1;

    std::unordered_map<int, std::string> options { { 0b001, "check" }, { 0b010, "call" }, { 0b011, "raise" }, { 0b100, "fold" } };
    std::unordered_map<int, std::string> cards { { 0b001, "Jack" }, { 0b010, "Queen" }, { 0b011, "King" } };

    std::cout << "INFOSET:  ";
    int infosetCpy = infoset;
    for (int i = 0; i < passedStreets + 1; ++i) {
        std::cout << cards[infosetCpy & 0b11] << " ";
        infosetCpy >>= 2;
    }
    while (infosetCpy) {
        std::cout << options[infosetCpy & 0b111] << " ";
        infosetCpy >>= 3;
    }

    std::cout << "\nOPTIONS:  ";
    Node* node = bot.nodes[infoset];
    int* actions = node->get_actions();
    double* strategy = node->avg_strategy();
    for (int i = 0; i < node->num_actions(); ++i) std::cout << options[actions[i]] << ": " << strategy[i] << "  ";

    infoset >>= passedStreets * CARD_LEN + CARD_LEN;
    infoset <<= ACTION_LEN;

    bool found = false;
    std::string actionStr;
    int action = 0;
    std::cout << "\n\n";
    while (!found) {
        std::cin >> actionStr;

        for (auto& i : options) {
            if (i.second == actionStr) {
                action = i.first;
                found = true;
                break;
            }
        }
    }
    std::cout << "\n";

    int bet = 0;
    if (action == 0b011) {
        bet = 2 * (passedStreets + 1);
        if (lastAction == 0b011) bet *= 2;
    }
    else if (action == 0b010) bet = pot[!curPlayer] - pot[curPlayer];
    pot[curPlayer] += bet;

    cont_explore(holeCards, streetCards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
}

void Trainer::explore() {
    while (true) {
        shuffle();
        std::vector<int> pot(2, 1);
        cont_explore(deal[0], deal[1], pot);
    }
}

// TODO: fisher-yates shuffle
void Trainer::shuffle() {
    std::shuffle(deck.begin(), deck.end(), rng);
    for (int i = 0; i < NUM_HOLE_CARDS; ++i) deal[0][i] = deck[i];
    for (int i = 0; i < NUM_STREET_CARDS; ++i) deal[1][i] = deck[i + NUM_HOLE_CARDS];
}