#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <string>
#include <fstream>

using namespace std;

/*
DISTRIBUTION: NOT A CLASS
    Purpose: Retrieve random variables from various distributions utilizing random variates from Trace.cpp

    Public: 
        - get_uniform(double a, double b) (double): method
            returns random variable from uniform distribution
        - get_exponential(double lambda) (double): method
            returns rnadom varible from exponential distribution

*/

double get_uniform(double a, double b, ifstream&);
double get_exponential(double lambda, ifstream&);

#endif
