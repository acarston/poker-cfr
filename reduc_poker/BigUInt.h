#ifndef BIG_U_INT_H
#define BIG_U_INT_H

#include <stdexcept>

class BigUInt {
private:
	const static int ULLONG_BITSIZE = 64;
	unsigned long long* bigUInt = nullptr;
	std::size_t size = 0;

public:
	BigUInt(const unsigned long long uInt, const std::size_t size) {
		if (size < ULLONG_BITSIZE) throw std::invalid_argument("invalid size; too small: " + size);
		if (size % ULLONG_BITSIZE != 0) throw std::invalid_argument("invalid size; not a multiple of 64: " + size);

		this->size = size / ULLONG_BITSIZE;
		bigUInt = new unsigned long long[this->size];
		bigUInt[0] = uInt;
	}
	~BigUInt() { delete[] bigUInt; }

	bool operator==(const BigUInt& other) const {
		for (std::size_t i = 0; i < size; ++i) {
			if (this->bigUInt[i] != other.bigUInt[i]) return false;
		}
		return true;
	}

	BigUInt& operator^=(const unsigned int x) {
		bigUInt[0] ^= x;
		return *this;
	}

	unsigned long long operator&(const unsigned int x) const {
		return bigUInt[0] & x;
	}

	BigUInt& operator>>=(const unsigned int x) {
		bigUInt[0] >>= x;

		for (std::size_t i = 1; i < size; ++i) {
			if (!bigUInt[i]) return *this;

			const int mask = int(pow(2, x)) - 1;
			const unsigned long long remaining = bigUInt[i] & mask;

			bigUInt[i - 1] += remaining << ULLONG_BITSIZE - x;
			bigUInt[i] >>= x;
		}

		return *this;
	}

	BigUInt& operator<<=(const unsigned int x) {
		unsigned long long remaining = bigUInt[0] >> ULLONG_BITSIZE - x;

		bigUInt[0] -= remaining << ULLONG_BITSIZE - x;
		bigUInt[0] <<= x;

		const int mask = int(pow(2, x)) - 1;
		for (std::size_t i = 1; i < size; ++i) {
			if (!remaining) return *this;

			const unsigned long long nextRemaining = bigUInt[i] >> ULLONG_BITSIZE - x;

			bigUInt[i] -= nextRemaining << ULLONG_BITSIZE - x;
			bigUInt[i] <<= x;
			bigUInt[i] &= remaining;

			remaining = nextRemaining;
		}

		return *this;
	}

	struct Hash {
		std::size_t operator()(const BigUInt& other) const noexcept {
			size_t hash = std::hash<int>()(other.bigUInt[0]);
			for (size_t i = 1; i < other.size; ++i) hash ^= other.bigUInt[i];
		}
	};
};

#endif