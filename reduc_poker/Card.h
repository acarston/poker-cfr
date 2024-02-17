#ifndef CARD_H
#define CARD_H

struct Card {
	int rank = 0;
	int suit = 0;

	Card() {};
	Card(const int rank, const int suit) : rank(rank), suit(suit) {};

	bool operator>(const Card& other) const { return this->rank > other.rank; }
	bool operator<(const Card& other) const { return this->rank < other.rank; }
	bool operator==(const Card& other) const { return this->rank == other.rank; }
	bool operator==(const int rank) const { return this->rank == rank; }
	bool operator!=(const Card& other) const { return !(*this == other); }
	int operator-(const Card& other) const { return this->rank - other.rank; }
	Card& operator++() { ++this->rank; return *this; }
};

#endif