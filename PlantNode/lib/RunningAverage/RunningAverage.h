#ifndef RUNNING_AVERAGE_H
#define RUNNING_AVERAGE_H

#include <iostream>

class RunningAverage {
    private:
        float *_values;
        float *_oldest;
        int _size;
        int _count;

    public:
        RunningAverage(int size);

        ~RunningAverage();

        void add(int value);

        float getAverage();

        int getCount() {return _count;};

        bool isFull() {return _count == _size;};
};

#endif