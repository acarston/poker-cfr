#include "Trainer.h"

Trainer::Trainer() {
    for (int i = 0; i < SUITS; ++i) {
        for (int j = 0; j < RANKS; ++j) objDeck[i * RANKS + j].set(j + 2, i);
    }
    for (int i = 0; i < NUM_CARDS; ++i) deck[i] = &objDeck[i];

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        deal[i] = new Card*[NUM_HOLE_CARDS];
        boards[i] = new Card*[BOARD_SIZE];
    }
    deal[NUM_PLAYERS] = new Card*[NUM_STREET_CARDS];
}

Trainer::~Trainer() {
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        delete[] deal[i];
        delete[] boards[i];
    }
    delete[] deal[NUM_PLAYERS];
}

void Trainer::shuffle() {
    // TODO: fisher-yates partial shuffle instead
    std::shuffle(deck, deck + NUM_CARDS, rng);
    int dealt = 0;

    for (int i = 0; i < NUM_PLAYERS; ++i) {
        for (int j = 0; j < NUM_HOLE_CARDS; ++j, ++dealt) {
            Card* card = deck[dealt];
            deal[i][j] = card;
            boards[i][j] = card;
        }
    }

    for (int i = 0; i < NUM_STREET_CARDS; ++i, ++dealt) {
        Card* card = deck[dealt];
        deal[NUM_PLAYERS][i] = card;
        for (int j = 0; j < NUM_PLAYERS; ++j) boards[j][NUM_HOLE_CARDS + i] = card;
    }
}

void Trainer::train(const unsigned int iterations) {
    this->iterations = iterations;

    for (unsigned int i = 1; i < iterations + 1; ++i) {
        shuffle();
        const int targetPlayer = (i % 2 == 0) ? 0 : 1;
        std::vector<int> pot{ 2, 1 };
        bot.mccfr(targetPlayer, i, deal, boards, pot);
    }
}

// DEPRECATE, doesn't work anyway
void Trainer::display_strats() const {
    std::cout << "Node Strategies:\n";

    std::unordered_map<int, std::string> options{ { 0b001, "check" }, { 0b010, "call" }, { 0b011, "raise" }, { 0b100, "fold" } };

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
        std::cout << "\n";
    }
    std::cout << "\n\nThis program was trained for " << iterations << " iterations." << "\n\n";
}

void Trainer::cont_explore(Card*** const deal, Card*** const boards, std::vector<int> pot, int passedStreets, int sinceChance, BigUInt infoset, int numPastActions) {
    const bool isEvenAction = numPastActions % 2 == 0;
    const int curPlayer = (passedStreets > 0) ? isEvenAction : !isEvenAction;
    const int lastActions = (sinceChance > 1 || passedStreets == 0) ? infoset & 0b111111 : infoset & (0b1 << sinceChance * ACTION_LEN) - 1;
    const int lastAction = lastActions & 0b111;

    for (int i = 0; i < passedStreets; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= deal[NUM_PLAYERS][i]->id;
    }

    std::cout << "POT:  " << pot[curPlayer] + pot[!curPlayer] << "\n";

    if ((lastActions == 0b011010 || lastActions == 0b001001) && sinceChance > 1) {
        if (passedStreets < NUM_STREETS) {
            infoset <<= CARD_LEN;
            infoset ^= deal[NUM_PLAYERS][passedStreets]->id;
            sinceChance = 0;
            ++passedStreets;
        }
        else {
            std::cout << "PLAYER 1 PAYOFF:  " << CFR::terminal_util(curPlayer, pot, boards) << "\n\n\n";
            return;
        }
    }
    else if (lastAction == 0b100) {
        std::cout << "PLAYER 1 PAYOFF:  " << pot[!curPlayer] << "\n\n\n";
        return;
    }
    ++sinceChance;

    for (int i = 0; i < NUM_HOLE_CARDS; ++i) {
        infoset <<= CARD_LEN;
        infoset ^= deal[curPlayer][i]->id;
    }

    std::unordered_map<int, std::string> options{ { 0b001, "check" }, { 0b010, "call" }, { 0b011, "raise" }, { 0b100, "fold" } };
    std::unordered_map<int, Card> cards;
    for (int i = 1; i <= SUITS; ++i) {
        for (int j = 1; j <= RANKS; ++j) {
            cards[i * j] = Card(j + 1, i - 1);
        }
    }

    std::cout << "INFOSET:  ";
    BigUInt infosetCpy(infoset);
    for (int i = 0; i < passedStreets + 2; ++i) {
        std::cout << "(" << cards[infosetCpy & 0b111111].rank  << ", " << cards[infosetCpy & 0b111111].suit << ")" << " ";
        infosetCpy >>= 6;
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

    CFR::update_pot(pot, curPlayer, action, lastAction, passedStreets);
    cont_explore(deal, boards, pot, passedStreets, sinceChance, infoset ^ action, numPastActions + 1);
}

void Trainer::explore() {
    while (true) {
        shuffle();
        std::vector<int> pot{ 2, 1 };
        cont_explore((Card***)deal, boards, pot);
    }
}