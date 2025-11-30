#include <cmath>

#include "Trace.h"

using namespace std;

double get_uniform(double a, double b) {
    return get_traceValue() * (b-a) + a;
}

double get_exponential(double lambda) {
    return -(1/lambda) * log(get_traceValue());
}