#ifndef NODE2_H
#define NODE2_H

#include <string>

class Node2 {
public:
	Node2(const std::string& history);
	~Node2();

	int get_num_actions() const { return this->numActions; }
	char* get_actions() const { return this->actions; }
	double* get_cumul_regrets() const { return this->cumulRegrets; }

	double* strategy(unsigned int iteration, double iterWeight);
	void update_sum(const int iteration, const double iterWeight);
	double* avg_strategy();

	static void normalize(double* arr, int n);

private:
	std::string history;

	int numActions = 0;
	char* actions = nullptr;
	double* cumulRegrets = nullptr;
	double* strat = nullptr;
	double* stratSum = nullptr;
	double* avgStrat = nullptr;

	const static int WARMUP = 100000;
};

#endif


