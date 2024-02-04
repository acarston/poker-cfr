#include "Trainer.h"

/* use CFR starting with a predetermined random deal for a number of iterations */
void Trainer::train(const unsigned int iterations) {
    this->iterations = iterations;

    const int deckSize = 3, deckRepeats = 2, numHoleCards = 2, numStreetCards = 1;
    std::vector<int> deck = get_deck(deckSize, deckRepeats);
    // std::vector<std::vector<int>> dealPerms = get_deal_perms(deck);
    // std::vector<int> randIndexes = get_rand_indexes(dealPerms.size() - 1);
    std::vector<std::vector<std::vector<int>>> deals = get_deals(deck, numHoleCards, numStreetCards);

    for (unsigned int i = 0; i < iterations; ++i) {
        const int targetPlayer = (i % 2 == 0) ? 0 : 1;
        bot.mccfr(targetPlayer, i + 1, deals[i][0], deals[i][1]);
    }
};

void Trainer::display_strats() const {
    std::cout << "Average utility of the root node:\n" << (rootNodeUtil / iterations) << "\n\n";
    std::cout << "Node Strategies:\n";

    double* strategy = nullptr;
    for (auto it = bot.nodes.begin(); it != bot.nodes.end(); ++it) {
        strategy = it->second->avg_strategy();
        std::cout << it->first << ": ";
        std::cout << "bet " << strategy[0] << ", pass " << strategy[1] << "\n";
    }
    std::cout << "\n\nThis program was trained for " << iterations << " iterations." << std::endl;
};

std::vector<int> Trainer::get_deck(const int size, const int repeats) const {
    std::vector<int> deck;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < repeats; ++j) deck.push_back(i);
    }
    return deck;
};

// TODO: deprecate
std::vector<std::vector<int>> Trainer::get_deal_perms(const std::vector<int>& deck) const {
    std::vector<std::vector<int>> dealPerms;

    for (int i = deck[0]; i < deck.size() - 1; ++i) {
        for (int j = i + 1; j < deck.size(); ++j) {
            dealPerms.push_back({ i, j });
        }
    }
    for (int i = deck.size() - 1; i > 0; --i) {
        for (int j = i - 1; j > 0 - 1; --j) {
            dealPerms.push_back({ i, j });
        }
    }

    return dealPerms;
};

// /* generate a vector of random indexes with a size equal to the number of iterations */
// std::vector<int> Trainer::get_rand_indexes(const int range) const {
//     std::vector<int> indexes(iterations);
//
//     std::random_device dev;
//     std::mt19937_64 rng(dev());
//     std::uniform_int_distribution<int> dist(0, range);
//
//     for (int i = 0; i < iterations; ++i) indexes[i] = dist(rng);
//
//     return indexes;
// };

std::vector<std::vector<std::vector<int>>> Trainer::get_deals(std::vector<int>& cards, const int numHoleCards, const int numStreetCards) const {
    std::vector<std::vector<std::vector<int>>> deals(iterations, std::vector<std::vector<int>>(2));
    
    std::random_device dev;
    std::mt19937_64 rng(dev());

    for (int i = 0; i < iterations; ++i) {
        std::shuffle(cards.begin(), cards.end(), rng);
        for (int j = 0; j < numHoleCards + numStreetCards; ++j) {
            if (j < numHoleCards) deals[i][0].push_back(cards[j]);
            else deals[i][1].push_back(cards[j]);
        }
    }

    return deals;
}