#ifndef CROSSWALK_H
#define CROSSWALK_H

#include "Person.hpp"
#include "Car.hpp"

#include <limits>

using namespace std;

/*
CROSSWALK: NOT A CLASS
    Purpose: Organize movement of people and cards during different traffic light colors

    Constants:
        - B (int): 330 ft - width of residential block
        - W (int): 24 ft - width of crosswalk
        - S (int): 46 ft - width of street
        - RED (int): 18 s - time traffic signal is red
        - YELLOW (int): 8 s - time traffic signal is yellow
        - GREEN (int): 35 s - min. time traffic signal is green after red
        - LAMBDA_P (int): 3 peds/min - arrival rate of pedestrians (Exponential(LAMBDA_P) for interarrivals) - LAMBDA_P in each direction, 2*LAMBDA_P overall in simulation
        - LAMBDA_A (int): 4 autos/min - arrival rate of automobiles (Exponential(LAMBDA_A) for interarrivals) - LAMBDA_A in each direction, 2*LAMBDA_A overall in simulation
        - L (int): 9 ft: length of automobile
        - ACC (int): 10 ft/s^2: auto acceleration / deceleration
        - VJ_A (int): 25 - a value in Uniform(a, b) for pedestrian speed
        - VJ_B (int): 35 - b value in Uniform(a, b) for pedestrian speed
        - VK_A (double): 2.6 - a value in Uniform(a, b) for auto speed
        - VK_B (double): 4.1 - b value in Uniform(a, b) for auto speed
        - MIN_DOUBLE (double): closest positive double value to 0
        - MAX_WALK_NUM (int): 20 peds - number of people maximum that can cross on a red light
        - TOTAL_WALK_DIST (double): total distance person must travel = 0.5 block + 1 street + 0.5 block + 1 street
        - DRIVE_CROSS_END (double): end of car is at end of crosswalk = 3.5 blocks + 3 streets + 0.5 sidewalk width + 1 car length
        - DRIVE_CROSS_FRONT (double): front of car is at start of crosswalk = 3.5 blocks + 3 streets - 0.5 sidewalk width
        - TOTAL_DRIVE_DIST (double): total distance car must travel = 7 blocks + 6 streets
*/


// Constants
namespace Cross {
    constexpr int B = 330;
    constexpr int W = 24;
    constexpr int S = 46;
    constexpr int RED = 18;
    constexpr int YELLOW = 8;
    constexpr int GREEN = 35;
    constexpr int LAMBDA_P = 3;
    constexpr int LAMBDA_A = 4;
    constexpr int L = 9;
    constexpr int ACC = 10;
    constexpr int VJ_A = 25;
    constexpr int VJ_B = 35;
    constexpr double VK_A = 2.6;
    constexpr double VK_B = 4.1;
    constexpr double MIN_DOUBLE = std::numeric_limits<double>::denorm_min();
    constexpr int MAX_WALK_NUM = 20;
    // total distance person must travel = 0.5 block + 1 street + 0.5 block + 1 street
    constexpr double TOTAL_WALK_DIST = Cross::B + (Cross::S * 2);
    // end of car is at end of crosswalk = 3.5 blocks + 3 streets + 0.5 sidewalk width + 1 car length
    constexpr double DRIVE_CROSS_END = (Cross::B * 3.5) + (Cross::S * 3) + (Cross::W * 0.5) + Cross::L; 
    // front of car is at start of crosswalk = 3.5 blocks + 3 streets - 0.5 sidewalk width
    constexpr double DRIVE_CROSS_FRONT = (Cross::B * 3.5) + (Cross::S * 3) - (Cross::W * 0.5);
    // total distance car must travel = 7 blocks + 6 streets + 1 car length
    constexpr double TOTAL_DRIVE_DIST = (Cross::B * 7) + (Cross::S * 6) + Cross::L;
}



/*
Public:
    - LightType (enum): variable
        crosswalk light type: NewGreen, ExpGreen, Yellow, Red
    - Direction (enum): variable
        direction the pedestrian / automobile is bound: east, west
*/

// Enums
enum LightType: char {
    NewGreen,
    ExpGreen,
    Yellow,
    Red
};

enum Direction: char {
    East,
    West
};

// Methods
void process_new_green();
void process_exp_green();
void process_yellow();
void process_red();
void process_person_enter(Person*);
void process_person_arrive(Person*);
bool should_press(int);
void process_check_min(Person*);
void walk(double remainTime);
void process_car_enter(Car*);
void check_carQueue();
void calc_actual_time(Car&);
bool check_must_stop(Car&);
void update_car_stats();
void update_person_stats();
void output_stats();

#endif