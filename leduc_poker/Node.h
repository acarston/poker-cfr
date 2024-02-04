#ifndef NODE_H
#define NODE_H

#include <string>

class Node {
public:
	Node(const int lastActions, const int lastAction);
	~Node();

	int num_actions() const { return this->numActions; }
	int* get_actions() const { return this->actions; }
	double* cumul_regrets() const { return this->cumulRegrets; }

	double* strategy();
	void update_sum(const unsigned int iteration, const double iterWeight);
	double* avg_strategy();

	static void normalize(double* arr, int n);

private:
	int numActions = 0;
	int* actions = nullptr;
	double* cumulRegrets = nullptr;
	double* strat = nullptr;
	double* stratSum = nullptr;
	double* avgStrat = nullptr;

	const static int WARMUP = 100000;
};

#endif