#ifndef CARD_H
#define CARD_H

struct Card {
	int rank;
	int suit;

	Card(const int rank, const int suit) : rank(rank), suit(suit) {};

	bool operator>(const Card& other) const { return this->rank > other.rank; }
	bool operator==(const Card& other) const { return this->rank == other.rank; }
	bool operator==(const int rank) const { return this->rank == rank; }
	bool operator!=(const Card& other) const { return !(*this == other); }
	int operator-(const Card& other) const { return this->rank - other.rank; }
};

#endif