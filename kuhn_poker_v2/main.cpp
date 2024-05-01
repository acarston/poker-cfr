//#include "Trainer.h"
//
//int main() {
//	Trainer trainer;
//	trainer.train(10000000);
//	trainer.display_strats();
//}

//#include <algorithm>
//#include <iostream>
//#include <random>
//#include <queue>
//
//struct Card {
//	int rank = 0;
//	int suit = 0;
//
//	Card() {};
//	Card(const int rank, const int suit) : rank(rank), suit(suit) {};
//
//	bool operator>(const Card& other) const { return this->rank > other.rank; }
//	bool operator<(const Card& other) const { return this->rank < other.rank; }
//	bool operator==(const Card& other) const { return this->rank == other.rank; }
//	bool operator==(const int rank) const { return this->rank == rank; }
//	bool operator!=(const Card& other) const { return !(*this == other); }
//	int operator-(const Card& other) const { return this->rank - other.rank; }
//	Card& operator++() { ++this->rank; return *this; }
//};
//
//class BoardPermutation : std::iterator<std::input_iterator_tag, Card**> {
//private:
//	Card** board = nullptr;
//	bool done = false;
//	const int HAND_SIZE = 5;
//	const int LAST_IDX = 4;
//	int i = LAST_IDX;
//	int j = LAST_IDX;
//
//public:
//	BoardPermutation(Card** board) : board(board) {};
//
//	explicit operator bool() const { return !done; }
//	reference operator*() { return board; }
//
//	BoardPermutation& operator++() {
//		if (i < 0 && j < 0) done = true;
//
//		else if (j <= i || i == LAST_IDX) {
//			std::swap(board[i], board[HAND_SIZE]);
//			j = HAND_SIZE - 1;
//			--i;
//		}
//
//		else {
//			std::swap(board[j], board[HAND_SIZE + 1]);
//			--j;
//		}
//
//		return *this;
//	}
//
//	BoardPermutation operator++(int) {
//		BoardPermutation copy(*this);
//		++*this;
//		return copy;
//	}
//};
//
//void clear_queue(std::queue<Card*>& q) {
//	std::queue<Card*> empty;
//	std::swap(q, empty);
//}
//
//void shuffle(Card** deck, Card*** deal, Card*** boards, std::mt19937& rng) {
//	const int NUM_HOLE_CARDS = 2, NUM_PLAYERS = 2, NUM_STREET_CARDS = 5;
//
//	// TODO: fisher-yates partial shuffle instead
//	std::shuffle(deck, deck + 52, rng);
//	int dealt = 0;
//
//	std::vector<std::vector<Card*>> test(2, std::vector<Card*>(7));
//	Card* test2 = boards[0][1];
//	Card* test3 = deal[0][1];
//
//	//test
//	std::cout << std::endl;
//
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		for (int j = 0; j < NUM_HOLE_CARDS; ++j, ++dealt) {
//			Card* card = deck[dealt];
//			deal[i][j] = card;
//			boards[i][j] = card;
//
//			test2 = boards[0][1];
//			test3 = deal[0][1];
//
//			test[i][j] = card;
//
//			std::cout << "(" << boards[i][j]->rank << ", " << boards[i][j]->suit << ")";
//			std::cout << "(" << test[i][j]->rank << ", " << test[i][j]->suit << ")";
//
//			//test 
//			/*for (int k = 0; k < j + 1; ++k) std::cout << "(" << boards[i][k]->rank << ", " << boards[i][k]->suit << ")";
//			std::cout << std::endl;*/
//		}
//	}
//	//test
//	std::cout << std::endl;
//	std::cout << "(" << boards[0][1]->rank << ", " << boards[0][1]->suit << ")";
//	std::cout << "(" << boards[1][0]->rank << ", " << boards[1][0]->suit << ")";
//	std::cout << std::endl;
//
//	//test
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		for (int j = 0; j < 2; ++j) {
//			std::cout << "(" << boards[i][j]->rank << ", " << boards[i][j]->suit << ")";
//			std::cout << "(" << test[i][j]->rank << ", " << test[i][j]->suit << ")";
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//
//	for (int i = 0; i < NUM_STREET_CARDS; ++i, ++dealt) {
//		Card* card = deck[dealt];
//		deal[NUM_PLAYERS][i] = card;
//		for (int j = 0; j < NUM_PLAYERS; ++j) {
//			boards[j][NUM_HOLE_CARDS + i] = card;
//			test[j][NUM_HOLE_CARDS + i] = card;
//		}
//	}
//
//	//test
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		for (int j = 0; j < 7; ++j) {
//			std::cout << "(" << boards[i][j]->rank << ", " << boards[i][j]->suit << ")";
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//}
//
//int compare_hands(Card** hand1, Card** hand2) {
//	for (int i = 0; i < 5; ++i) {
//		if (*hand1[i] == *hand2[i]) continue;
//		return (*hand1[i] > *hand2[i]) ? 1 : 0;
//	}
//	return -1;
//}
//
//int rank_hand(Card** cards) {
//	const int handSize = 5;
//	
//	bool flush = true;
//	bool straight = true;
//
//	std::sort(cards, cards + handSize, [](const Card* a, const Card* b) { return *a > *b; });
//
//	for (int i = 0; i < handSize; ++i) std::cout << "(" << cards[i]->rank << ", " << cards[i]->suit << ") ";
//	std::cout << std::endl;
//
//	for (int i = 0; i < handSize - 1; ++i) {
//		if (cards[0]->suit != cards[i + 1]->suit) flush = false;
//		if (*cards[i] - *cards[i + 1] != 1) straight = false;
//		if (!flush && !straight) break;
//	}
//	if (*cards[0] == 14 && *cards[1] == 5 && *cards[2] == 4 && *cards[3] == 3 && *cards[4] == 2) straight = true;
//
//	std::queue<std::queue<Card*>> frequencies[handSize]{};
//	std::queue<Card*> tmpQueue;
//	for (int i = 0; i < handSize - 1; ++i) {
//		if (*cards[i] == *cards[i + 1]) tmpQueue.push(cards[i]);
//		else {
//			tmpQueue.push(cards[i]);
//			frequencies[tmpQueue.size()].push(tmpQueue);
//			clear_queue(tmpQueue);
//		}
//	}
//	tmpQueue.push(cards[handSize - 1]);
//	frequencies[tmpQueue.size()].push(tmpQueue);
//
//	int rank = 1;
//	if (flush && straight) rank = 9;
//	else if (!frequencies[4].empty()) rank = 8;
//	else if (!frequencies[3].empty() && !frequencies[2].empty()) rank = 7;
//	else if (flush) rank = 6;
//	else if (straight) rank = 5;
//	else if (!frequencies[3].empty()) rank = 4;
//	else if (frequencies[2].size() > 1) rank = 3;
//	else if (!frequencies[2].empty()) rank = 2;
//
//	int cardsIdx = 0;
//	for (int i = handSize - 1; i >= 0; --i) {
//		std::queue<std::queue<Card*>>& cur = frequencies[i];
//		while (!cur.empty()) {
//			std::queue<Card*>& nested = cur.front();
//			while (!nested.empty()) {
//				cards[cardsIdx] = nested.front();
//				nested.pop();
//				++cardsIdx;
//			}
//			cur.pop();
//		}
//	}
//
//	for (int i = 0; i < handSize; ++i) std::cout << "(" << cards[i]->rank << ", " << cards[i]->suit << ") ";
//	std::cout << std::endl;
//
//	return rank;
//}
//
//int terminal_util(const int curPlayer, int* pot, Card*** boards) {
//	const int NUM_PLAYERS = 2;
//	const int HAND_SIZE = 5;
//
//	int bestRank[NUM_PLAYERS]{};
//	Card* bestHand[NUM_PLAYERS][HAND_SIZE]{};
//	Card* curHand[HAND_SIZE];
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		BoardPermutation permutation(boards[i]);
//
//		while (permutation) {
//			std::copy(boards[i], boards[i] + HAND_SIZE, curHand);
//
//			std::cout << "PERMUTATION:  ";
//			for (int j = 0; j < 7; ++j) {
//				std::cout << (boards[i][j])->rank << " ";
//			}
//			std::cout << "PLAYER:  " << i << std::endl;
//
//			int rank = rank_hand(curHand);
//			if (rank > bestRank[i] || (rank == bestRank[i] && compare_hands(curHand, bestHand[i]) == 1)) {
//				bestRank[i] = rank;
//				std::copy(curHand, curHand + HAND_SIZE, bestHand[i]);
//			}
//
//			++permutation;
//		}
//	}
//
//	for (int i = 0; i < NUM_PLAYERS; ++i) {
//		std::cout << "Player " << i + 1 << " - Best Rank: " << bestRank[i] << ", Best Hand: ";
//		for (int j = 0; j < HAND_SIZE; ++j) {
//			std::cout << "(" << bestHand[i][j]->rank << ", " << bestHand[i][j]->suit << ") ";
//		}
//		std::cout << std::endl;
//	}
//
//	if (bestRank[curPlayer] != bestRank[!curPlayer]) return (bestRank[curPlayer] > bestRank[!curPlayer]) ? pot[!curPlayer] : -pot[curPlayer];
//	int curPlayerBetter = compare_hands(bestHand[curPlayer], bestHand[!curPlayer]);
//	return (curPlayerBetter == 1) ? pot[!curPlayer] : (curPlayerBetter == 0) ? -pot[curPlayer] : 0;
//}
//
////
////void test_hand_ranking(const char* hand_name, Card** cards) {
////	std::cout << "Testing hand: " << hand_name << std::endl;
////	std::cout << "Hand Rank: " << rank_hand(cards) << std::endl;
////	std::cout << std::endl;
////}
////
////int main() {
////	// Pre-made hands for testing
////	Card* straight_flush[] = { new Card(10, 0), new Card(11, 0), new Card(12, 0), new Card(13, 0), new Card(14, 0) }; // Straight Flush (10 to Ace of Spades)
////	Card* four_of_a_kind[] = { new Card(10, 0), new Card(10, 1), new Card(10, 2), new Card(10, 3), new Card(2, 0) }; // Four of a Kind (Four 10s and a 2)
////	Card* full_house[] = { new Card(10, 0), new Card(10, 1), new Card(10, 2), new Card(2, 0), new Card(2, 1) }; // Full House (Three 10s and Two 2s)
////	Card* flush[] = { new Card(2, 0), new Card(4, 0), new Card(6, 0), new Card(8, 0), new Card(10, 0) }; // Flush (All Hearts)
////	Card* straight[] = { new Card(2, 0), new Card(3, 1), new Card(4, 2), new Card(5, 3), new Card(6, 0) }; // Straight (2 to 6)
////	Card* three_of_a_kind[] = { new Card(7, 0), new Card(7, 1), new Card(7, 2), new Card(5, 0), new Card(3, 1) }; // Three of a Kind (Three 7s)
////	Card* two_pair[] = { new Card(8, 0), new Card(8, 1), new Card(5, 0), new Card(5, 1), new Card(2, 0) }; // Two Pair (Two 8s and Two 5s)
////	Card* one_pair[] = { new Card(9, 0), new Card(9, 1), new Card(3, 0), new Card(2, 0), new Card(7, 1) }; // One Pair (Two 9s)
////
////	// Test each hand
////	test_hand_ranking("Straight Flush", straight_flush);
////	test_hand_ranking("Four of a Kind", four_of_a_kind);
////	test_hand_ranking("Full House", full_house);
////	test_hand_ranking("Flush", flush);
////	test_hand_ranking("Straight", straight);
////	test_hand_ranking("Three of a Kind", three_of_a_kind);
////	test_hand_ranking("Two Pair", two_pair);
////	test_hand_ranking("One Pair", one_pair);
////
////	// Free memory for cards
////	for (int i = 0; i < 5; ++i) {
////		delete straight_flush[i];
////		delete four_of_a_kind[i];
////		delete full_house[i];
////		delete flush[i];
////		delete straight[i];
////		delete three_of_a_kind[i];
////		delete two_pair[i];
////		delete one_pair[i];
////	}
////
////	std::random_device dev;
////	std::mt19937 rng{ dev() };
////
////	const int SUITS = 4;
////	const int RANKS = 13;
////	constexpr int NUM_CARDS = SUITS * RANKS;
////	const int NUM_PLAYERS = 2;
////	const int NUM_HOLE_CARDS = 2;
////	const int NUM_STREET_CARDS = 5;
////	constexpr int BOARD_SIZE = NUM_HOLE_CARDS + NUM_STREET_CARDS;
////
////	//TODO: memory leaks
////	Card* deck[NUM_CARDS];
////	for (int i = 0; i < SUITS; ++i) {
////		for (int j = 0; j < RANKS; ++j) deck[i] = new Card(j, i);
////	}
////
////	Card** deal[NUM_PLAYERS + 1];
////	for (int i = 0; i < NUM_PLAYERS; ++i) deal[i] = new Card*[NUM_HOLE_CARDS];
////	deal[NUM_PLAYERS] = new Card*[NUM_STREET_CARDS];
////
////	Card** boards[NUM_PLAYERS];
////  for (int i = 0; i < NUM_PLAYERS; ++i) boards[i] = new Card * [BOARD_SIZE];
////}
//
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
//	Card objDeck[NUM_CARDS];
//	Card* deck[NUM_CARDS];
//
//	for (int i = 0; i < SUITS; ++i) {
//		for (int j = 0; j < RANKS; ++j) {
//			const int idx = i * RANKS + j;
//			objDeck[idx].rank = j + 2;
//			objDeck[idx].suit = i;
//		}
//	}
//	for (int i = 0; i < NUM_CARDS; ++i) deck[i] = &objDeck[i];
//
//	for (Card* card : deck) {
//		std::cout << "(" << card->rank << ", " << card->suit << ") " << "\n";
//	}
//
//	Card** deal[NUM_PLAYERS + 1];
//	for (int i = 0; i < NUM_PLAYERS; ++i) deal[i] = new Card*[NUM_HOLE_CARDS];
//	deal[NUM_PLAYERS] = new Card*[NUM_STREET_CARDS];
//
//	//Card objBoards[NUM_PLAYERS][BOARD_SIZE];
//	//Card* boards[NUM_PLAYERS][BOARD_SIZE];
//
//	//for (int i = 0; i < NUM_PLAYERS; ++i) {
//	//	for (int j = 0; j < BOARD_SIZE; ++j) {
//	//		boards[i][j] = &objBoards[i][j];
//	//	}
//	//}
//
//	Card** boards[NUM_PLAYERS];
//	for (int i = 0; i < NUM_PLAYERS; ++i) boards[i] = new Card*[7];
//
//	shuffle(deck, deal, (Card***)boards, rng);
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
//	std::cout << std::endl << std::endl;
//
//	int pot[NUM_PLAYERS] = { 100, 100 };
//	int curPlayer = 0;
//	int result = terminal_util(curPlayer, pot, (Card***)boards);
//	std::cout << "Result: " << result << std::endl;
//}
//
////int main() {
////	Card* hand1[]{ new Card(7, 3), new Card(7, 1), new Card(13, 0), new Card(11, 0), new Card(3, 3) };
////	Card* hand2[]{ new Card(7, 3), new Card(7, 1), new Card(11, 3), new Card(10, 2), new Card(6, 3) };
////	int test = compare_hands(hand1, hand2);
////	std::cout << test << std::endl;
////}
//
//
//
////#include <iostream>
////#include <vector>
////
////using namespace std;
////
////void printVec(const vector<int>& v) {
////	for (const int el : v) std::cout << el << " ";
////	std::cout << std::endl;
////}
////
////void printCombos(vector<int>& elements) {
////	const int r = 5;
////
////	printVec(elements);
////	for (int i = r - 1; i >= 0; --i) {
////		std::swap(elements[i], elements[r]);
////		printVec(elements);
////		for (int j = r - 1; j >= i; --j) {
////			std::swap(elements[j], elements[r + 1]);
////			printVec(elements);
////		}
////	}
////}
////
////// Function to generate combinations using a loop
////void generateCombinations(vector<int>& elements) {
////	int n = elements.size();
////	int k = 5;
////
////	// Initialize combination with first k elements
////	vector<int> combination(k);
////	for (int i = 0; i < k; ++i) {
////		combination[i] = elements[i];
////	}
////
////	while (true) {
////		// Print the combination
////		for (int num : combination) {
////			cout << num << " ";
////		}
////		cout << endl;
////
////		// Find the first index j such that elements[j] is not at its maximum value
////		int j = k - 1;
////		while (j >= 0 && combination[j] == elements[n - k + j]) {
////			j--;
////		}
////
////		// If no such index exists, we're done
////		if (j < 0) {
////			break;
////		}
////
////		// Increment combination[j], and set all subsequent elements to the next value after combination[j]
////		combination[j]++;
////		for (int l = j + 1; l < k; ++l) {
////			combination[l] = combination[l - 1] + 1;
////		}
////	}
////}
////
////int main() {
////	vector<int> elements = { 2,3,5,9,10,12,14 };
////	Card* els[] = { new Card(2, 0), new Card(3, 0),new Card(5, 0),new Card(9, 0),new Card(10, 0),new Card(12, 0),new Card(14, 0) };
////	// elements = { 1, 2, 3, 4, 5, 6, 7 };
////
////	printCombos(elements);
////	std::cout << std::endl << std::endl;
////	BoardPermutation perm(els);
////	while (perm) {
////		++perm;
////	}
////
////	return 0;
////}

#include "BigUInt.h"

