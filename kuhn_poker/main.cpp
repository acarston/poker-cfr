#include "Trainer.h"

int main() {
    Trainer trainer;
    trainer.numIterations = 1000000000;

    trainer.TrainTest();
    trainer.DisplayNodeStrategies();

    return 0;
};