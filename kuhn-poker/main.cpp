#include "Trainer.cpp"

int main() {
    Trainer trainer;
    int numIterations = 100000;

    trainer.Train(numIterations);
    trainer.DisplayNodeStrategies(numIterations);

    return 0;
};