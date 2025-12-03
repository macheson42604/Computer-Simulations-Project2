#include <cmath>
#include <fstream>

#include "Trace.h"

using namespace std;

double get_uniform(double a, double b, ifstream& traceFileStream) {
    return get_traceValue(traceFileStream) * (b-a) + a;
}

// QUESTION - ARE THESE CORRECT DISTRIBUTIONS?
double get_exponential(double lambda, ifstream& traceFileStream) {
    // Definition 3.1.1 return(-mu * log(1.0 - Random()))
    return (-1.0) * (1.0/lambda) * log(1.0 - get_traceValue(traceFileStream));
}