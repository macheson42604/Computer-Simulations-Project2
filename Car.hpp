#ifndef CAR_HPP
#define CAR_HPP

enum Direction : char; 

using namespace std;

/*
CAR: CLASS
    Purpose: Contain attributes of a Car such as simulation enter time, speed

    Private:
        - enterTime (double): variable
            simulation time that car entered the simulation
        - speed (double): variable
            speed of the car from Uniform() distribution
        - travelDir (Direction enum): variable
            travel of the current 
        - isStopped (bool): variable
            indicates if the car has or hasn't stopped at a red light
        - actualTime (double): variable
            simulation time when the end of car actually leaves the simulation
        - optimalTime (double): variable
            simulation time when the end of car leaves the simulation if the car stayed at its constant speed


    Public:
        - get_enter_time() (double): method
            returns enterTime
        - get_speed() (double): method
            returns speed
        - get_direction() (Direction enum): method
            returns travelDir
        - get_optimal_time() (double): method
            returns optimalTime
        - get_actual_time() (double): method
            returns actualTime
        - calc_delay() (double): method
            returns delayed time

        - set_stopped() (void): method
            returns isStopped
        - set_actual_time(double): method
            calculated the actual time car leaves simulation and sets to actualTime

*/

class Car {
    private:
        double enterTime;
        double speed;
        Direction travelDir;
        bool isStopped = false; //all cars are initially moving
        double actualTime = -1; //initialized to -1 to show that the actual time hasn't been calculated yet
        double optimalTime;
        int id;
        bool isStoppedTimeNeg = false; //edge case: if stopped time becomes negative in calc, we technically allow actualTime < optimalTime 
        bool left = false;

    public:
        Car(double, Direction, int);
        double calc_delay();

        // GETTERS
        double get_enter_time();
        double get_speed();
        Direction get_direction();
        bool get_stopped();
        double get_actual_time();
        double get_optimal_time();
        int get_id();
        bool get_is_stopped_neg();
        bool get_left();

        // SETTERS
        void set_stopped();
        void set_actual_time(double);
        void set_is_stopped_neg();
        void set_left();
        


};

#endif