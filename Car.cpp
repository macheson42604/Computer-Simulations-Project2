#include <iostream>

#include "Car.hpp"
#include "Crosswalk.h"
#include "Distribution.h"
#include "Trace.h"

using namespace std;


// Constructor
Car::Car(double time, Direction travelDir) {
    this->travelDir = travelDir;
    enterTime = time;

    // calculate speed (mph -> ft/s) no need to keep speed at mph
    // speed_fts = speed_mph * (5280 / 3600)
    speed = get_uniform(Cross::VJ_A, Cross::VJ_B, autoTraceStream) * (5280 / 3600);

    // calculate optimal time (if the car traveled at its constant speed)
    // time elapsed = total distance / speed
    optimalTime = time + (Cross::TOTAL_DRIVE_DIST / speed);
    
};



// GETTERS

double Car::get_enter_time() {
    return enterTime;
}

double Car::get_speed() {
    return speed;
}

Direction Car::get_direction() {
    return travelDir;
}

bool Car::get_stopped() {
    return isStopped;
}

double Car::get_actual_time() {
    return actualTime;
}

double Car::get_optimal_time() {
    return optimalTime;
}

double Car::calc_delay() {
    if (actualTime == -1) {
        cerr << "Error: actual time car leaves simulation is not set" << endl;
        exit(1);
    }
    if (actualTime - optimalTime < 0) {
        cerr << "Error: actual time is less than optimal time" << endl;
        exit(1);
    }
    return actualTime - optimalTime;
}


// SETTERS

// we technically don't need to set isStopped to false again because the car will already be popped off the queue by then
void Car::set_stopped() {
    isStopped = true;
}

void Car::set_actual_time(double aT) {
    actualTime = aT;
}