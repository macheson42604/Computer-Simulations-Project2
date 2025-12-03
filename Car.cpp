#include <iostream>

#include "Car.hpp"
#include "Crosswalk.h"
#include "Distribution.h"
#include "Trace.h"

using namespace std;


// Constructor
Car::Car(double time, Direction travelDir, int nC) {
    this->travelDir = travelDir;
    enterTime = time;

    // calculate speed (mph -> ft/s) no need to keep speed at mph
    // speed_fts = speed_mph * (5280 / 3600)
    speed = get_uniform(Cross::VJ_A, Cross::VJ_B, autoTraceStream) * (5280.0 / 3600.0);

    // calculate optimal time (if the car traveled at its constant speed)
    // time elapsed = total distance / speed
    optimalTime = time + (Cross::TOTAL_DRIVE_DIST / speed);
    id = nC;
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
    if ((!isStoppedTimeNeg) && (actualTime - optimalTime < 0)) {
        // DEBUG
        cout << "id: " << id << " | stopped: " << isStopped << " | actual: " << actualTime << " | optimal: " << optimalTime << endl;


        cerr << "Error: car actual time is less than optimal time" << endl;
        exit(1);
    }
    return actualTime - optimalTime;
}


int Car::get_id() {
    return id;
}

bool Car::get_is_stopped_neg() {
    return isStoppedTimeNeg;
}

bool Car::get_left() {
    return left;
}

// SETTERS

// we technically don't need to set isStopped to false again because the car will already be popped off the queue by then
void Car::set_stopped() {
    isStopped = true;
}

void Car::set_actual_time(double aT) {
    actualTime = aT;
}

// only ever set to true (defaulted to false)
void Car::set_is_stopped_neg() {
    isStoppedTimeNeg = true;
}

void Car::set_left() {
    left = true;
}