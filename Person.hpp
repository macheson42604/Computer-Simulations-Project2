#ifndef PERSON_HPP
#define PERSON_HPP

using namespace std;


/*
PERSON: CLASS
    Purpose: Contain attributes of a Person such as simulation enter time, arrival time, speed

    Private:
        - enterTime (double): variable
            simulation time that person entered the simulation
        - arrTime (double): variable
            simulation time that hte person will reach the button
        - speed (double): variable
            speed of the pedestrian from Uniform() distribution

    Public:
        - calc_cross_time() (double): method
            calculates the time it would take the given pedestrian to cross the crosswalk

*/


class Person {
    Person(double);
private:
    double enterTime;
    double arrTime;
    double speed;

public:
    double calc_cross_time();

};

#endif