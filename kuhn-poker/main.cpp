#include "Trainer.cpp"

int main() {
    Trainer trainer;
    trainer.numIterations = 1000000;

    trainer.Train();
    trainer.DisplayNodeStrategies();

    return 0;
};