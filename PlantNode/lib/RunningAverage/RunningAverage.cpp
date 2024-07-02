#include "RunningAverage.h"

/*
Create RunningAverage buffer, which is used to store values and get average of the stored values.

Parameters:
int size = size of the buffer.
*/
RunningAverage::RunningAverage(int size) {
    _values = (float*) malloc(size * sizeof(float));
    if(!_values) {
        std::cout << "Memory allocation failed!";
        exit(1);
    }
    _size = size;
    _oldest = _values;
    _count = 0;
}


RunningAverage::~RunningAverage() {
    free(_values);
}


/*
Add new value to the buffer. If full replace oldest.
*/
void RunningAverage::add(int value) {
    // If _oldest points to last adress of _values, reset _oldest back to first element of _values.
    if(_oldest == &_values[_size]) _oldest = &_values[0];

    // Increase _count every addition of values until same as _size.
    if(_count < _size) _count++;

    // Replace oldest value with new value.
    *_oldest = value;
    _oldest++;

}


/*
Get average of the values in the buffer.
*/
float RunningAverage::getAverage() {
    // If there is no values added, return 0.
    if(_count < 1) return 0;

    float sum = 0;
    for(int i = 0; i < _count; i++) {
        sum = sum + _values[i];
    }
    return sum / _count;
}