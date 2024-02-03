#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
public:
	Node();
	~Node();

	int get_num_actions() const { return this->numActions; }
	double* get_cumul_regrets() const { return this->cumulRegrets; }

	double* strategy();
	void update_sum(const unsigned int iteration, const double iterWeight);
	double* avg_strategy();

	static void normalize(double* arr, int n);

private:
	std::string history;

	int numActions = 0;
	double* cumulRegrets = nullptr;
	double* strat = nullptr;
	double* stratSum = nullptr;
	double* avgStrat = nullptr;

	const static int WARMUP = 100000;
};

#endif