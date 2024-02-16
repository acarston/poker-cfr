//#include "Trainer.h"
//
//int main() {
//	Trainer trainer;
//	trainer.train(10000000);
//	trainer.display_strats();
//}

#include <algorithm>
#include <iostream>
#include <random>
#include <queue>

struct Card {
	int rank;
	int suit;

	Card(const int rank, const int suit) : rank(rank), suit(suit) {};

	bool operator>(const Card& other) const { return this->rank > other.rank; }
	bool operator<(const Card& other) const { return this->rank < other.rank; }
	bool operator==(const Card& other) const { return this->rank == other.rank; }
	bool operator==(const int rank) const { return this->rank == rank; }
	bool operator!=(const Card& other) const { return !(*this == other); }
	int operator-(const Card& other) const { return this->rank - other.rank; }
};

void clear_queue(std::queue<Card*>& q) {
	std::queue<Card*> empty;
	std::swap(q, empty);
}

void shuffle(Card** deck, Card*** deal, Card*** boards, std::mt19937& rng) {
	const int NUM_HOLE_CARDS = 2, NUM_PLAYERS = 2, NUM_STREET_CARDS = 5;

	std::shuffle(deck, deck + 52, rng);
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

int compare_hands(Card** hand1, Card** hand2) {
	for (int i = 0; i < 5; ++i) {
		if (hand1[i] == hand2[i]) continue;
		return (hand1[i] > hand2[i]) ? 0 : 1;
	}
	return -1;
}

int rank_hand(Card** cards) {
	const int handSize = 5;
	
	bool flush = true;
	bool straight = true;

	std::sort(cards, cards + handSize, [](const Card* a, const Card* b) { return *a > *b; });

	for (int i = 0; i < handSize; ++i) std::cout << "(" << cards[i]->rank << ", " << cards[i]->suit << ") ";
	std::cout << std::endl;

	for (int i = 0; i < handSize - 1; ++i) {
		if (cards[0]->suit != cards[i + 1]->suit) flush = false;
		if (*cards[i] - *cards[i + 1] != 1) straight = false;
		if (!flush && !straight) break;
	}
	if (*cards[0] == 14 && *cards[1] == 5 && *cards[2] == 4 && *cards[3] == 3 && *cards[4] == 2) straight = true;

	std::queue<std::queue<Card*>> frequencies[handSize]{};
	std::queue<Card*> tmpQueue;
	for (int i = 0; i < handSize - 1; ++i) {
		std::cout << "CARDS: " << cards[i]->rank << " " << cards[i + 1]->rank << std::endl;
		if (*cards[i] == *cards[i + 1]) tmpQueue.push(cards[i]);
		else {
			tmpQueue.push(cards[i]);
			frequencies[tmpQueue.size()].push(tmpQueue);
			clear_queue(tmpQueue);
		}
	}
	tmpQueue.push(cards[handSize - 1]);
	frequencies[tmpQueue.size()].push(tmpQueue);

	int rank = 1;
	if (flush && straight) rank = 9;
	else if (!frequencies[4].empty()) rank = 8;
	else if (!frequencies[3].empty() && !frequencies[2].empty()) rank = 7;
	else if (flush) rank = 6;
	else if (straight) rank = 5;
	else if (!frequencies[3].empty()) rank = 4;
	else if (!frequencies[2].size() > 1) rank = 3;
	else if (!frequencies[2].empty()) rank = 2;

	int cardsIdx = 0;
	for (int i = handSize - 1; i >= 0; --i) {
		std::queue<std::queue<Card*>>& cur = frequencies[i];
		while (!cur.empty()) {
			std::queue<Card*>& nested = cur.front();
			while (!nested.empty()) {
				cards[cardsIdx] = nested.front();
				nested.pop();
				++cardsIdx;
			}
			cur.pop();
		}
	}

	for (int i = 0; i < handSize; ++i) std::cout << "(" << cards[i]->rank << ", " << cards[i]->suit << ") ";
	std::cout << std::endl;

	return rank;
}

int terminal_util(const int curPlayer, int* pot, Card*** boards) {
	const int NUM_PLAYERS = 2;
	const int HAND_SIZE = 5;

	int bestRank[NUM_PLAYERS]{};
	Card* bestHand[NUM_PLAYERS][HAND_SIZE]{};
	for (int i = 0; i < NUM_PLAYERS; ++i) {
		do {
			std::cout << "PERMUTATION:  ";
			for (int j = 0; j < 7; ++j) {
				std::cout << (boards[i][j])->rank << " ";
			}
			std::cout << std::endl;

			int rank = rank_hand(boards[i]);
			if (rank > bestRank[i] || (rank == bestRank[i] && compare_hands(boards[i], bestHand[i]) == 1)) {
				bestRank[i] = rank;
				std::copy(boards[i], boards[i] + HAND_SIZE, bestHand[i]);
			}
			std::sort(boards[i], boards[i] + 7, [](const Card* a, const Card* b) { return *a < *b; }); // REMOVE
		} while (std::next_permutation(boards[i], boards[i] + 7, [](const Card* a, const Card* b) { return *a < *b; }));
	}

	for (int i = 0; i < NUM_PLAYERS; ++i) {
		std::cout << "Player " << i + 1 << " - Best Rank: " << bestRank[i] << ", Best Hand: ";
		for (int j = 0; j < HAND_SIZE; ++j) {
			std::cout << "(" << bestHand[i][j]->rank << ", " << bestHand[i][j]->suit << ") ";
		}
		std::cout << std::endl;
	}

	if (bestRank[0] != bestRank[1]) return (bestRank[0] > bestRank[1]) ? pot[!curPlayer] : -pot[curPlayer];
	int curPlayerBetter = compare_hands(bestHand[0], bestHand[1]);
	return (curPlayerBetter == 1) ? pot[!curPlayer] : (curPlayerBetter == 0) ? -pot[curPlayer] : 0;
}

//
//void test_hand_ranking(const char* hand_name, Card** cards) {
//	std::cout << "Testing hand: " << hand_name << std::endl;
//	std::cout << "Hand Rank: " << rank_hand(cards) << std::endl;
//	std::cout << std::endl;
//}
//
//int main() {
//	// Pre-made hands for testing
//	Card* straight_flush[] = { new Card(10, 0), new Card(11, 0), new Card(12, 0), new Card(13, 0), new Card(14, 0) }; // Straight Flush (10 to Ace of Spades)
//	Card* four_of_a_kind[] = { new Card(10, 0), new Card(10, 1), new Card(10, 2), new Card(10, 3), new Card(2, 0) }; // Four of a Kind (Four 10s and a 2)
//	Card* full_house[] = { new Card(10, 0), new Card(10, 1), new Card(10, 2), new Card(2, 0), new Card(2, 1) }; // Full House (Three 10s and Two 2s)
//	Card* flush[] = { new Card(2, 0), new Card(4, 0), new Card(6, 0), new Card(8, 0), new Card(10, 0) }; // Flush (All Hearts)
//	Card* straight[] = { new Card(2, 0), new Card(3, 1), new Card(4, 2), new Card(5, 3), new Card(6, 0) }; // Straight (2 to 6)
//	Card* three_of_a_kind[] = { new Card(7, 0), new Card(7, 1), new Card(7, 2), new Card(5, 0), new Card(3, 1) }; // Three of a Kind (Three 7s)
//	Card* two_pair[] = { new Card(8, 0), new Card(8, 1), new Card(5, 0), new Card(5, 1), new Card(2, 0) }; // Two Pair (Two 8s and Two 5s)
//	Card* one_pair[] = { new Card(9, 0), new Card(9, 1), new Card(3, 0), new Card(2, 0), new Card(7, 1) }; // One Pair (Two 9s)
//
//	// Test each hand
//	test_hand_ranking("Straight Flush", straight_flush);
//	test_hand_ranking("Four of a Kind", four_of_a_kind);
//	test_hand_ranking("Full House", full_house);
//	test_hand_ranking("Flush", flush);
//	test_hand_ranking("Straight", straight);
//	test_hand_ranking("Three of a Kind", three_of_a_kind);
//	test_hand_ranking("Two Pair", two_pair);
//	test_hand_ranking("One Pair", one_pair);
//
//	// Free memory for cards
//	for (int i = 0; i < 5; ++i) {
//		delete straight_flush[i];
//		delete four_of_a_kind[i];
//		delete full_house[i];
//		delete flush[i];
//		delete straight[i];
//		delete three_of_a_kind[i];
//		delete two_pair[i];
//		delete one_pair[i];
//	}
//
//	std::random_device dev;
//	std::mt19937 rng{ dev() };
//
//	const int SUITS = 4;
//	const int RANKS = 13;
//	constexpr int NUM_CARDS = SUITS * RANKS;
//	const int NUM_PLAYERS = 2;
//	const int NUM_HOLE_CARDS = 2;
//	const int NUM_STREET_CARDS = 5;
//	constexpr int BOARD_SIZE = NUM_HOLE_CARDS + NUM_STREET_CARDS;
//
//	//TODO: memory leaks
//	Card* deck[NUM_CARDS];
//	for (int i = 0; i < SUITS; ++i) {
//		for (int j = 0; j < RANKS; ++j) deck[i] = new Card(j, i);
//	}
//
//	Card** deal[NUM_PLAYERS + 1];
//	for (int i = 0; i < NUM_PLAYERS; ++i) deal[i] = new Card*[NUM_HOLE_CARDS];
//	deal[NUM_PLAYERS] = new Card*[NUM_STREET_CARDS];
//
//	Card** boards[NUM_PLAYERS];
//  for (int i = 0; i < NUM_PLAYERS; ++i) boards[i] = new Card * [BOARD_SIZE];
//}

//int main() {
//	std::random_device dev;
//	std::mt19937 rng{ dev() };
//
//	const int SUITS = 4;
//	const int RANKS = 13;
//	constexpr int NUM_CARDS = SUITS * RANKS;
//	const int NUM_PLAYERS = 2;
//	const int NUM_HOLE_CARDS = 2;
//	const int NUM_STREET_CARDS = 5;
//	constexpr int BOARD_SIZE = NUM_HOLE_CARDS + NUM_STREET_CARDS;
//
//	Card* deck[NUM_CARDS];
//	for (int i = 0; i < SUITS; ++i) {
//		for (int j = 0; j < RANKS; ++j) deck[i * RANKS + j] = new Card(j + 2, i);
//	}
//
//	for (Card* card : deck) {
//		std::cout << "(" << card->rank << ", " << card->suit << ") " << "\n";
//	}
//
//	Card** deal[NUM_PLAYERS + 1];
//	for (int i = 0; i < NUM_PLAYERS; ++i) deal[i] = new Card*[NUM_HOLE_CARDS];
//	deal[NUM_PLAYERS] = new Card*[NUM_STREET_CARDS];
//
//	Card** boards[NUM_PLAYERS];
//	for (int i = 0; i < NUM_PLAYERS; ++i) boards[i] = new Card*[BOARD_SIZE];
//
//	shuffle(deck, deal, boards, rng);
//
//	std::cout << "Dealt Cards:" << std::endl;
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		std::cout << "Player " << i + 1 << ": ";
//		for (int j = 0; j < NUM_HOLE_CARDS; ++j) {
//			std::cout << "(" << deal[i][j]->rank << ", " << deal[i][j]->suit << ") ";
//		}
//		std::cout << std::endl;
//	}
//
//	std::cout << "Board Cards:" << std::endl;
//	for (int i = 0; i < NUM_STREET_CARDS; ++i) {
//		std::cout << "(" << deal[NUM_PLAYERS][i]->rank << ", " << deal[NUM_PLAYERS][i]->suit << ") ";
//	}
//	std::cout << std::endl;
//
//	int pot[NUM_PLAYERS] = { 100, 100 };
//	int curPlayer = 0;
//	int result = terminal_util(curPlayer, pot, boards);
//	std::cout << "Result: " << result << std::endl;
//
//	return 0;
//}

class 

#include <iostream>
#include <vector>

using namespace std;

// Function to generate combinations using a loop
void generateCombinations(vector<int>& elements) {
	int n = elements.size();
	int k = 5;

	// Initialize combination with first k elements
	vector<int> combination(k);
	for (int i = 0; i < k; ++i) {
		combination[i] = elements[i];
	}

	while (true) {
		// Print the combination
		for (int num : combination) {
			cout << num << " ";
		}
		cout << endl;

		// Find the first index j such that elements[j] is not at its maximum value
		int j = k - 1;
		while (j >= 0 && combination[j] == elements[n - k + j]) {
			j--;
		}

		// If no such index exists, we're done
		if (j < 0) {
			break;
		}

		// Increment combination[j], and set all subsequent elements to the next value after combination[j]
		combination[j]++;
		for (int l = j + 1; l < k; ++l) {
			combination[l] = combination[l - 1] + 1;
		}
	}
}

int main() {
	vector<int> elements = { 1, 2, 3, 4, 5, 6, 7 };

	generateCombinations(elements);

	return 0;
}

