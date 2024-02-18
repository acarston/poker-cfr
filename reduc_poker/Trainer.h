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
    const static int CARD_LEN = 6;
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

    void shuffle();

public:
    Trainer();
    ~Trainer();

    void train(const unsigned int iterations);
    void display_strats() const;
    void cont_explore(const Card*** const deal, Card*** const boards, int* const pot, int passedStreets = 0, int sinceChance = 0, int infoset = 0, int numPastActions = 0);
    void explore();
};

#endif