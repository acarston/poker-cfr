#include "Trainer.cpp"

int main() {
    //work good seperately, disater together. Set up test that iterates through like I do on py, see if still correct.
    //if yes, something wrong with CFR, if no, something wrong with my sampling
    Trainer trainer;
    int numIterations = 100000;

    trainer.Train(numIterations);
    trainer.DisplayNodeStrategies(numIterations);

    return 0;
};