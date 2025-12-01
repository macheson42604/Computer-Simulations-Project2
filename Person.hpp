#ifndef PERSON_HPP
#define PERSON_HPP

enum Direction : char; 
//#include "Crosswalk.cpp"

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
        - travelDir (Direction enum): variable
            travel of the current 


    Public:
        - calc_cross_time() (double): method
            calculates the time it would take the given pedestrian to cross the crosswalk
        - get_enter_time() (double): method
            returns enterTime
        - get_arr_time() (double): method
            returns arrTime
        - get_speed() (double): method
            returns speed
        - get_direction() (Direction enum): method
            returns travelDir

*/


class Person {
    private:
        double enterTime;
        double arrTime;
        double speed;
        Direction travelDir;

    public:
        Person(double, Direction);
        double calc_cross_time();

        // GETTERS
        double get_enter_time();
        double get_arr_time();
        double get_speed();
        Direction get_direction();

};

#endif