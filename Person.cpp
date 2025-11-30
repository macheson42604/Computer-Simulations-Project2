#include "Person.hpp"
#include "Crosswalk.h"
#include "Distribution.h"

using namespace std;


// Constructor
Person::Person(double time, Direction travelDir) {
    this->travelDir = travelDir;
    enterTime = time;

    // calculate speed (ft/s)
    speed = get_uniform(Cross::VK_A, Cross::VK_B);

    // calculate the arrival time using the distance from simulation enter to button
    // distance = 1/2 block + 1 street + 1/2 block = 1 block + 1 street
    arrTime = enterTime + (Cross::B + Cross::S) / speed;
};


double Person::calc_cross_time() {
    // time = distance / speed
    return Cross::W / speed;
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