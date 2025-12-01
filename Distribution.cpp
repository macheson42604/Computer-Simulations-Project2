#include <cmath>
#include <fstream>

#include "Trace.h"

using namespace std;

double get_uniform(double a, double b, ifstream& traceFileStream) {
    return get_traceValue(traceFileStream) * (b-a) + a;
}

double get_exponential(double lambda, ifstream& traceFileStream) {
    return -(1/lambda) * log(get_traceValue(traceFileStream));
}