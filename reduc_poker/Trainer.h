#ifndef TRAINER_H
#define TRAINER_H

#include <random>
#include <iostream>
#include <algorithm>
#include <bitset>
#include <unordered_map>

#include "CFR.h"
#include "Card.h"

class Trainer {
private:
    CFR bot;

    const static int NUM_PLAYERS = 2;
    const static int NUM_HOLE_CARDS = 2;
    const static int NUM_STREET_CARDS = 5;
    const static int BOARD_SIZE = NUM_HOLE_CARDS + NUM_STREET_CARDS;
    // const int DECK_SIZE = 3;
    // const int DECK_REPEATS = 2;
    const static int CARD_LEN = 2;
    const static int ACTION_LEN = 3;
    const static int NUM_STREETS = 3;
    const static int SUITS = 4;
    const static int RANKS = 13;
    const static int NUM_CARDS = SUITS * RANKS;

    int iterations = 0;

    std::random_device dev;
    std::mt19937_64 rng{ dev() };

    Card objDeck[NUM_CARDS];
    Card* deck[NUM_CARDS];
    Card** deal[NUM_PLAYERS + 1];
    Card** boards[NUM_PLAYERS];

    // std::vector<int> deck;
    // std::vector<std::vector<int>> deal{ { 0, 0 }, { 0 } };

    void shuffle();

public:
    Trainer();
    ~Trainer();

    void train(const unsigned int iterations);
    void display_strats() const;
    void cont_explore(const std::vector<int>& holeCards, const std::vector<int>& streetCards, std::vector<int> pot, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);
    void explore();
};

#endif