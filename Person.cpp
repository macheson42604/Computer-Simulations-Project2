#include <iostream>

#include "Person.hpp"
#include "Crosswalk.h"
#include "Distribution.h"
#include "Trace.h"

using namespace std;


// Constructor
Person::Person(double time, Direction travelDir, int nP) {
    this->travelDir = travelDir;
    enterTime = time;

    // calculate speed (ft/s)
    speed = get_uniform(Cross::VK_A, Cross::VK_B, pedTraceStream);

    // calculate the arrival time using the distance from simulation enter to button
    // distance = 1/2 block + 1 street + 1/2 block = 1 block + 1 street
    arrTime = enterTime + (Cross::B + Cross::S) / speed;

    // calculate optimal time = enterTime + elapsedTime
    // time elapsed = total distance / constant speed
    optimalTime = enterTime + (Cross::TOTAL_WALK_DIST / speed);

    // give id
    id = nP;
};


double Person::calc_cross_time() {
    // time = distance / speed
    return Cross::S / speed;
}


double Person::calc_delay() {
    if (actualTime == -1) {
        cerr << "Error: actual time person leaves simulation is not set" << endl;
        exit(1);
    }
    if ((actualTime - optimalTime) < -0.000001) {
        cerr << "actualTime: " << actualTime << endl;
        cerr << "optimalTime: " << optimalTime << endl;
        cerr << "Error: person actual time is less than optimal time" << endl;
        exit(1);
    }
    return actualTime - optimalTime;
}


void Person::update_actual_time(double simClock) {
    // simClock = time when person is about to cross the crosswalk
    // QUESTION - confirmation: person considered to have left simulation immediately after crossing crosswalk?
    actualTime = simClock + this->calc_cross_time();
}



// GETTERS

double Person::get_enter_time() {
    return enterTime;
}

double Person::get_arr_time() {
    return arrTime;
}

double Person::get_speed() {
    return speed;
}

Direction Person::get_direction() {
    return travelDir;
}

double Person::get_actual_time() {
    return actualTime;
}

double Person::get_optimal_time() {
    return optimalTime;
}

int Person::get_id() {
    return id;
}