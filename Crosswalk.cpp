#include <iostream>
#include <queue>

#include "Crosswalk.h"
#include "Person.hpp"
#include "Car.hpp"
#include "Event.hpp"
#include "Distribution.cpp"
#include "Trace.h"

using namespace std;

// PUBLIC VARIABLES
enum LightType {
    NewGreen,
    ExpGreen,
    Yellow,
    Red
};

double simClock = 0.0;
bool isPressed = false;
enum LightType currLight = ExpGreen; // begin the simulation on ExpGreen
queue<Person> personQueue; // initialize as empty
queue<Car> carQueue; // initialize as empty
// priority_queue<Event> eventList;

int main () {
    

    return 0;
}