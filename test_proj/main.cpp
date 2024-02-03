#include <array>
#include <unordered_map>
#include <iostream>

//int main() {
//	typedef std::array<int, 3> infosets;
//	std::unordered_map<infosets, int> nodes;
//}

int main() {
	//int infoset = 0;
	//infoset = infoset << 2;
	//int mask = 2;
	//infoset = infoset ^ mask;

	int infoset = 7;
	infoset = infoset >> 2;
	infoset = infoset << 2;
	std::cout << infoset;
}