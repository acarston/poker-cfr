//#include <array>
//#include <unordered_map>
//#include <iostream>
//
////int main() {
////	typedef std::array<int, 3> infosets;
////	std::unordered_map<infosets, int> nodes;
////}
//
//int main() {
//	//int infoset = 0;
//	//infoset = infoset << 2;
//	//int mask = 2;
//	//infoset = infoset ^ mask;
//
//	int infoset = 7;
//	infoset = infoset >> 2;
//	infoset = infoset << 2;
//	std::cout << infoset;
//}

#include <iomanip>
#include <iostream>
#include <map>
#include <random>

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d({ 0.5, 0.5 });
    std::map<int, int> map;

    for (int n = 0; n < 1e4; ++n)
        ++map[d(gen)];

    /*for (const auto& [num, count] : map)
        std::cout << num << " generated " << std::setw(4) << count << " times\n";*/

    for (const auto& item : map) std::cout << item.first << " generated " << std::setw(4) << item.second << " times\n";
}