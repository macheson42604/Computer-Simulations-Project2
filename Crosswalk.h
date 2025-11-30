#ifndef CROSSWALK_H
#define CROSSWALK_H

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

    Public:
        - LightType (enum): variable
            crosswalk light type: NewGreen, ExpGreen, Yellow, Red
        - simClock (double): variable
            current time of the simulation
        - isPressed (bool): variable
            checks if the button has already been pressed by a pedestrian during a NewGreen / ExpGreen
        - currLight (lightType): variable
            current light signal
        - personQueue (queue<Person>): variable
            the queue of people waiting at the crosswalk when the light is not Red
        - carQueue (queue<Car>): variable
            the queue of cars waiting at the crosswalk when the light is Red
        - eventList (priority_queue<Event?>):variable
            the priority queue of events to process with corresponding simulation clock times

*/

namespace Cross {
    constexpr int B = 330;
    constexpr int W = 24;
    constexpr int S = 46;
    constexpr int RED = 18;
    constexpr int YELLOW = 8;
    constexpr int GREEN = 25;
    constexpr int LAMBDA_P = 3;
    constexpr int LAMBDA_A = 4;
    constexpr int L = 9;
    constexpr int ACC = 10;
    constexpr int VJ_A = 25;
    constexpr int VJ_B = 35;
    constexpr double VK_A = 2.6;
    constexpr double VK_B = 4.1;
    constexpr double MIN_DOUBLE = std::numeric_limits<double>::denorm_min();
}


#endif