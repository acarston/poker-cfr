#include "Trainer.h"

int main() {
	Trainer trainer;
	trainer.train(10000000);
	trainer.display_strats();
	trainer.explore();
}