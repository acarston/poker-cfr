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
		std::fill(bigUInt + 1, bigUInt + this->size, 0);
	}
	//~BigUInt() { if (bigUInt != nullptr) delete[] bigUInt; }

	bool operator==(const BigUInt& other) const {
		for (std::size_t i = 0; i < size; ++i) {
			if (this->bigUInt[i] != other.bigUInt[i]) return false;
		}
		return true;
	}

	explicit operator bool() const {
		for (std::size_t i = 0; i < size; ++i) {
			if (!this->bigUInt[i]) return true;
		}
		return false;
	}

	BigUInt operator^(const unsigned int x) const {
		BigUInt copy(*this);
		copy ^= x;
		return copy;
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
		bool done = false;
		for (std::size_t i = 0; i < size; ++i) {
			unsigned long long mask = bigUInt[i] >> ULLONG_BITSIZE - x;

			if (!mask && !bigUInt[i]) continue;
			else done = true;

			bigUInt[i] -= mask << ULLONG_BITSIZE - x;
			bigUInt[i] <<= x; 

			for (std::size_t j = i + 1; j < size; ++j) {
				if (!mask) break;

				const unsigned long long nextMask = bigUInt[j] >> ULLONG_BITSIZE - x;

				bigUInt[j] -= nextMask << ULLONG_BITSIZE - x;
				bigUInt[j] <<= x;
				bigUInt[j] ^= mask;

				mask = nextMask;
			}

			if (done) return *this;
		}

		return *this;
	}

	struct Hash {
		std::size_t operator()(const BigUInt& obj) const noexcept {
			std::size_t hash = 17;
			const std::size_t prime = 31;

			for (std::size_t i = 0; i < obj.size; ++i) {
				hash = hash * prime + std::hash<unsigned long long>{}(obj.bigUInt[i]);
			}

			return hash;
		}
	};
};

#endif