#include "Trainer.h"

int main() {
	Trainer trainer;
	trainer.train(100000);
	trainer.display_strats();
	trainer.explore();
}