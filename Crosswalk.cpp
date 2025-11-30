#include <iostream>
#include <queue>

#include "Crosswalk.h"
#include "Person.hpp"
#include "Car.hpp"
#include "Event.hpp"
#include "Distribution.h"
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

/*
MAIN FUNCTION: RUN CROSSWALK SIMULATION
*/
int main () {
    

    return 0;
}

/*
PROCESS NEW GREEN EVENT
*/


/* 
PROCESS EXP GREEN EVENT
*/

/* 
PROCESS YELLOW EVENT
*/

/* 
PROCESS RED EVENT
*/



/*
PROCESS CHECK MIN EVENT
*/
void process_check_min(Person fplb) {
    // (FPLB) first person left behind = first person in the queue AFTER the red light turns green
    
    // when checking if the FPLB needs to press the button after 1 minute, there are 3 cases
    
    // CASE 1: someone pressed the button when currLight was NewGreen and currLight is now Red
    // check the person is not in queue
    
    // CASE 2: someone pressed the button when currLight was ExpGreen and currLight is now Yellow

    // CASE 3: no one has pressed the button and currLight is stil ExpGreen
}