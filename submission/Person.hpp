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
        - actualTime (double): variable
            simulation time when the end of person actually leaves the simulation
        - optimalTime (double): variable
            simulation time when the end of person leaves the simulation if the person stayed at their constant speed


    Public:
        - calc_cross_time() (double): method
            calculates the time it would take the given pedestrian to cross the crosswalk
        - update_actual_time (double): method
            calculates the actual time the person will leave the simulation and sets actualTime
        - calc_delay() (double): method
            calculates actualTime - optimalTime

        
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
        double actualTime = -1; //initialized to -1 to show that the actual time hasn't been calculated yet
        double optimalTime;
        int id;

    public:
        Person(double, Direction, int);
        double calc_cross_time();
        void update_actual_time(double);
        double calc_delay();

        // GETTERS
        double get_enter_time();
        double get_arr_time();
        double get_speed();
        Direction get_direction();
        double get_actual_time();
        double get_optimal_time();
        int get_id();


};

#endif