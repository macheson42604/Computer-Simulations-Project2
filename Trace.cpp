#include <iostream>
#include <fstream>
#include <string>

using namespace std;

ifstream autoTraceStream;
ifstream pedTraceStream;
ifstream buttonTraceStream;

ifstream open_trace_file(string traceFile) {
    ifstream traceFileStream;
    traceFileStream.open(traceFile);
    if (!traceFileStream.is_open()) {
        cerr << "Error: could not open trace file: " << traceFile << endl;
        exit(1);
    }

    return traceFileStream;
}

double get_traceValue() {
    // get value
    double traceValue = -1;
    if (!(traceFileStream >> traceValue)) {
        cerr << "Error: get_traceValue() executed incorrectly" << endl;
        exit(1);
    }

    // First check that we have a value to give
    if (traceFileStream.eof()) {
        cerr << "Error: not enough values in trace file" << endl;
        exit(1);
    }
    return traceValue;
 }