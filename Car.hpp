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


    Public:
        - get_enter_time() (double): method
            returns enterTime
        - get_speed() (double): method
            returns speed
        - get_direction() (Direction enum): method
            returns travelDir

*/

class Car {
    private:
        double enterTime;
        double speed;
        Direction travelDir;
        bool stopped = false;

    public:
        Car(double, Direction);

        // GETTERS
        double get_enter_time();
        double get_speed();
        Direction get_direction();
        bool get_stopped();

        // SETTERS
        void set_stopped();
        


};

#endif