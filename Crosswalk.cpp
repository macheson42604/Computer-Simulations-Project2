#include <iostream>
#include <queue>
#include <string>
#include <fstream>

#include "Crosswalk.h"
#include "Person.hpp"
#include "Car.hpp"
#include "Event.hpp"
#include "Distribution.h"
#include "Trace.h"


using namespace std;

// PUBLIC VARIABLES
double simClock = 0.0;
bool isPressed = false;
enum LightType currLight = ExpGreen; // begin the simulation on ExpGreen
vector<Person> personQueue; // initialize as empty
queue<Car> carQueue; // initialize as empty
// priority_queue<Event> eventList;

int numWalked = 0; // initialize to 0 - no one has walked yet

// Simulation End Indicators
int numPeople = 0;
int numCars = 0;

// Command line inputs
int Q = -1;
string AUTO_RAND;
string PED_RAND;
string BUTTON_RAND;

/*
MAIN FUNCTION: RUN CROSSWALK SIMULATION
*/
int main (int argc, char* argv[]) {
    // Read in command line
    if (argc != 5) {
        cerr << "Error: Not enough or too many command line arguments, please try again" << endl;
        return 1;
    }

    // Check that Q was correctly input
    try {
        Q = stoi(argv[1]);

        if (Q <= 0) {
            throw (Q);
        }
    } catch (const invalid_argument& e) {
        cerr << "Error: Invalid argument for Q: " << e.what() << endl;
    } catch (const out_of_range& e) {
        cerr << "Error: Out of range for Q: " << e.what() << endl;
    } catch (int input) {
        cerr << "Error: Q must be greater than 0. You input " << input << endl;
    }

    // Check tracefile for autos
    AUTO_RAND = argv[2];

    return 0;
}

/*
========================================================================================================
LIGHT RELATED EVENTS
========================================================================================================
*/

/*
PROCESS NEW GREEN EVENT
*/
void process_new_green() {
    // set current light to NewGreen
    currLight = NewGreen;
    // set the number of people that crossed on the red light back to 0 in order to prepare for the next light cycle
    numWalked = 0;


}

/* 
PROCESS EXP GREEN EVENT
*/
void process_exp_green() {
    // set current light to ExpGreen
    currLight = ExpGreen;
}

/* 
PROCESS YELLOW EVENT
*/
void process_yellow() {
    // set current light to Yellow
    currLight = Yellow;
}

/* 
PROCESS RED EVENT
*/
void process_red() {
    // set current light to Red
    currLight = Red;
    
    // allow queue of people to walk
    walk(Cross::RED);

}


/*
========================================================================================================
PEDESTRIAN RELATED EVENTS
========================================================================================================
*/

/*
PROCESS PERSON ENTER EVENT
*/
void process_person_enter(Direction travelDir) {
    // create new person object
    Person newPerson(simClock, travelDir);

    // create a new event to have another pedestrian enter the simulation
    double nextEnterTime = simClock + get_exponential(Cross::LAMBDA_P, pedTraceStream);
    // TODO: add to event calendar

    // TODO: create a new event fro when this pedestrian arrives at the crosswalk 
    // use the newPerson.get_arr_time() function
    // add to event calendar
}


/*
PROCESS PERSON ARRIVAL EVENT
*/
void processs_person_arrive(Person arrPerson) {
    // add the person to the list of pedestrians waiting at crosswalk
    personQueue.push_back(arrPerson);

    // check the light to determine behavior
    // P-6A: press the button only if signal is a NO WALK state (aka as long as the light is not Red)
    if (currLight != Red) { 
        // determine if the person is going to push the button
        if (should_press(personQueue.size())) {
            // set pressed button to true
            isPressed = true;

            // process yellow event if the green light is already expired
            if (currLight == ExpGreen) {
                // TODO: add yellow event immediately (process time = simClock + Cross::MIN_DOUBLE )
            }
        }
    }

    // if the pedestrian is arriving in the middle of the red, we need to check if it's possible for them to cross in the remaining time
    else {
        // TODO: CHANGE FROM 0 TO CORRECT TIME
        // to find remaining time, we can either (1) access a global variable for the sim clock time of when the Red Light will end (aka when Newgreen will begin) or (2) look at the event calendar for the first ExpGreen event and retrieve the simulation process time
        walk(0);
    }
    
}

// should_press(): helper function to satisfy P-6A
bool should_press(int n) {
    if (n == 0) {
        // P(0) = 15/16
        return get_traceValue(buttonTraceStream) < (15.0 / 16.0);
    }
    else if (n > 0) {
        // P(n) = 1/(n+1)
        return get_traceValue(buttonTraceStream) < (1.0 / (n + 1));
    }
    else {
        cerr << "Error: cannot pass (n) number of pedestrians as less than 0" << endl;
        exit(1);
    }

}


/*
PROCESS CHECK MIN EVENT
*/
// P-6B: any pedestrian halted by NO WALK signal for >=1 min will push walk button
// for simplicity we only keep track of the first person in the arrived queue to press the button ()
void process_check_min(Person fplb) {
    // (FPLB) first person left behind = first person in the queue AFTER the red light turns green
    
    // when checking if the FPLB needs to press the button after 1 minute, there are 3 cases
    
    // CASE 1: someone pressed the button when currLight was NewGreen and currLight is now Red
    // check the person is not in queue
    
    // CASE 2: someone pressed the button when currLight was ExpGreen and currLight is now Yellow

    // CASE 3: no one has pressed the button and currLight is stil ExpGreen
    return;
}

// QUESTION: Edge case - if person 1-20 arrived at button and didn't press, then person 21 arrives and presses button (immediately turns yellow), then all 20 first people were able to cross and person 21 wasn't able to cross (by this point 8+18=26 seconds have passed), does their 1 minute timer run out after (60-26=34) more seconds or do we restart the 1 minute timer when the light goes back from red to newgreen? - I'm guessing it's the first option (which would change our logic)



/*
WALK: DETERMINE WHICH PEOPLE WALK
*/
// remainTime = remaining time in the red light
void walk(double remainTime) {
    // index of current person we're checking
    int currInd = 0;
    // P-7: up to 20 pedestrians can cross in 1 cycle of the Red light
    // we need to have a counter because if there is a person that arrives to the light later in the red light and can make it they should be able to pass
    // iterate through the list of people and see if their walking time will be able to reach 
    while (!personQueue.empty() && numWalked < Cross::MAX_WALK && currInd < (int)personQueue.size()) {
        if (personQueue[currInd].calc_cross_time() < remainTime) {
            // remove the person from the queue
            personQueue.erase(personQueue.begin() + currInd);
            // increase counter of people that have crossed
            numWalked++;
        }
        else {
            // if the person at the front of the queue can't cross, move to the next person until we check that nobody else can pass or the 20 people limit is reached
            currInd++;
        }
    }
}