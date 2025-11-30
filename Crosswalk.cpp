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
vector<Person> personQueue; // initialize as empty
queue<Car> carQueue; // initialize as empty
// priority_queue<Event> eventList;

int numWalked = 0; // initialize to 0 - no one has walked yet

/*
MAIN FUNCTION: RUN CROSSWALK SIMULATION
*/
int main () {
    

    return 0;
}

/*
PROCESS NEW GREEN EVENT
*/
void process_new_green() {
    // set current light to NewGreen
    currLight = NewGreen;
    // set the number of people that crossed on the red light back to 0 in order to prepare for the next light cycle
    numWalked = 0;


    return;
}

/* 
PROCESS EXP GREEN EVENT
*/
void process_exp_green() {
    // set current light to ExpGreen
    currLight = ExpGreen;
    return;
}

/* 
PROCESS YELLOW EVENT
*/
void process_yellow() {
    // set current light to Yellow
    currLight = Yellow;
    return;
}

/* 
PROCESS RED EVENT
*/
void process_red() {
    // set current light to Red
    currLight = Red;

    // allow queue of people to walk
    walk(Cross::RED);

    return;
}


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
    return;
}

/*
WALK: DETERMINE WHICH PEOPLE WALK
*/
// remainTime = remaining time in the red light
void walk(double remainTime) {
    // index of current person we're checking
    int currInd = 0;
    // up to 20 people 
    // we need to have a counter because if there is a person that arrives to the light later in the red light and can make it they should be able to pass
    // iterate through the list of people and see if their walking time will be able to reach 
    while (!personQueue.empty() && numWalked < Cross::MAX_WALK && currInd < (int)personQueue.size()) {
        if (personQueue[currInd].calc_cross_time() < remainTime) {
            // remove the person from the queue
            personQueue.erase(personQueue.begin() + currInd);
            // increase counter of people that have crossed
            numWalked++;

        }
    }
}