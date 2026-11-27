#ifndef MYTEMPLATES_H
#define MYTEMPLATES_H

#include <cmath>

// Returns the absolute difference in scalar values between two objects
template <typename T>
double scalar_difference(const T& obj1, const T& obj2) {
    return fabs(obj1.getScalarValue() - obj2.getScalarValue());
}

// Returns true if two objects are equal
template <typename T>
bool equals(const T& obj1, const T& obj2) {
    return obj1 == obj2;
}

#endif
