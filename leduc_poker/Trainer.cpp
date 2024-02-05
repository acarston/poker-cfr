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
        bot.mccfr(targetPlayer, i + 1, deal[0], deal[1]);
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

        std::cout << " REGRETS: "; // TEST
        for (int i = 0; i < node->num_actions(); ++i) std::cout << node->cumul_regrets()[i] << "  "; // TEST
        // std::cout << " STRATEGY: ";
        // for (int i = 0; i < node->num_actions(); ++i) std::cout << options[actions[i]] << ": " << node->strategy()[i] << "  ";

        std::cout << "\n";
    }
    std::cout << "\n\nThis program was trained for " << iterations << " iterations." << std::endl;
}

void Trainer::shuffle() {
    std::shuffle(deck.begin(), deck.end(), rng);
    for (int i = 0; i < NUM_HOLE_CARDS; ++i) deal[0][i] = deck[i];
    for (int i = 0; i < NUM_STREET_CARDS; ++i) deal[1][i] = deck[i + NUM_HOLE_CARDS];
}