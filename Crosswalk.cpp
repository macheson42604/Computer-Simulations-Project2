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
vector<Person*> personQueue; // initialize as empty
vector<Car> carQueue; // initialize as empty
priority_queue<Event, vector<Event>, greater<Event>> eventList;

int numWalked = 0; // initialize to 0 - no one has walked yet
double redEndTime = -1; //cannot use until processing first Red event

// Simulation End Indicators
int numPeople = 0;
int numCars = 0;
int Q = -1;

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

    // Check tracefiles
    autoTraceStream = open_trace_file(argv[2]);
    pedTraceStream = open_trace_file(argv[3]);
    buttonTraceStream = open_trace_file(argv[4]);

    // Load the first events
    if (Q >= 1) {
        // East Person 
        double enterTime1 = get_exponential(Cross::LAMBDA_P, pedTraceStream);
        Person* firstEastPerson = new Person(enterTime1, East);
        eventList.push(Event(enterTime1, PersonEnterEvent, firstEastPerson));
        numPeople ++;

        // East Car
        double enterTime3 = get_exponential(Cross::LAMBDA_A, autoTraceStream);
        Car* firstEastCar = new Car(enterTime3, East);
        eventList.push(Event(enterTime3, CarEnterEvent, firstEastCar));
        numCars ++;

        if (Q >= 2) {
            // West Person
            double enterTime2 = get_exponential(Cross::LAMBDA_P, pedTraceStream);
            Person* firstWestPerson = new Person(enterTime2, West);
            eventList.push(Event(enterTime2, PersonEnterEvent, firstWestPerson));
            numPeople ++;

            // West Car
            double enterTime4 = get_exponential(Cross::LAMBDA_A, autoTraceStream);
            Car* firstWestCar = new Car(enterTime4, West);
            eventList.push(Event(enterTime4, CarEnterEvent, firstWestCar));
            numCars ++;
        }
    }

    // Initialize first event to be processed
    Event curEvent = eventList.top();
    eventList.pop();

    // Main code loop
    while (!eventList.empty()) {
        if (curEvent.get_type() == PersonEnterEvent) {
            process_person_enter(curEvent.get_assoc_person());
        } else if (curEvent.get_type() == PersonArriveEvent) {
            process_person_arrive(curEvent.get_assoc_person());
        } else if (curEvent.get_type() == CarEnterEvent) {
            process_car_enter(curEvent.get_assoc_car());
        } else if (curEvent.get_type() == NewGreenEvent) {
            process_new_green();
        } else if (curEvent.get_type() == ExpGreenEvent) {
            process_exp_green();
        } else if (curEvent.get_type() == YellowEvent) {
            process_yellow();
        } else if (curEvent.get_type() == RedEvent) {
            process_red();
        } else if (curEvent.get_type() == CheckMinEvent) {
            process_check_min(curEvent.get_assoc_person());
        }

        // get next event
        curEvent = eventList.top();
        eventList.pop();
    }

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

    // add ExpGreen event to occur right after the new green finishes
    eventList.push(Event(simClock + Cross::GREEN, ExpGreenEvent));

    // P-6B: add CheckMin event corresponding to the first person left behind if this person exists (aka if PersonQueue is not empty)
    if (!personQueue.empty()) {
        eventList.push(Event(simClock + 60, CheckMinEvent, personQueue[0]));

        // P-6C: loop through all the people in the queue and check if they will push the button with probability P(n=0)
        for (size_t i = 0; i < personQueue.size(); i++) {
            if (should_press(0)) {
                isPressed = true;
                break; // no need to continue checking if the other pedestrians will press the button (even though technically in the simulation all the pedestrians should have this probability)
            }
        }
    }

}

/* 
PROCESS EXP GREEN EVENT
*/
void process_exp_green() {
    // set current light to ExpGreen
    currLight = ExpGreen;

    // check if the button has been pushed in order to add the Yellow Event, otherwise do nothing (the Yellow Event will be added by the next person who pressed the button)
    if (isPressed) {
        eventList.push(Event(simClock + Cross::MIN_DOUBLE, YellowEvent));
    }
}

/* 
PROCESS YELLOW EVENT
*/
void process_yellow() {
    // set current light to Yellow
    currLight = Yellow;

    // add Red event right after yellow light time finishes
    eventList.push(Event(simClock + Cross::YELLOW, RedEvent));

    // car logic
    int carInd = 0;
    while (carInd < (int)carQueue.size() && carQueue.size() > 0) {
        if (drive(carQueue[carInd], Cross::YELLOW)) { // can they make it?
            carQueue.erase(carQueue.begin() + carInd);
        } else {
            carQueue[carInd].set_stopped();
            carInd++;
        }
    }
}

/* 
PROCESS RED EVENT
*/
void process_red() {
    // set current light to Red
    currLight = Red;
    // set isPressed back to false as no pedestrians will be pressing the button when the light is red (P-6A)
    isPressed = false;


    // add New Green event right after red light time finished
    redEndTime = simClock + Cross::RED;
    eventList.push(Event(redEndTime, NewGreenEvent));

    // TODO: add in person logic
    
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

void process_person_enter(Person* currPerson) {
    // create a new event to have another pedestrian enter the simulation
    if (Q >= numPeople) {
        numPeople ++;
        double nextEnterTime = simClock + get_exponential(Cross::LAMBDA_P, pedTraceStream);
        // create new person object
        Person* newPerson = new Person(nextEnterTime, currPerson->get_direction());
        eventList.push(Event(nextEnterTime, PersonEnterEvent, newPerson));
    }

    // create a new event fro when this pedestrian arrives at the crosswalk 
    eventList.push(Event(currPerson->get_arr_time(), PersonArriveEvent, currPerson));
}


/*
PROCESS PERSON ARRIVAL EVENT
*/
void process_person_arrive(Person* arrPerson) {
    // add the person to the list of pedestrians waiting at crosswalk
    personQueue.push_back(arrPerson);

    // check the light to determine behavior
    // P-6A: press the button only if signal is a NO WALK state (aka as long as the light is not Red)
    if (currLight != Red) { 
        // determine if the person is going to push the button
        if (should_press(personQueue.size())) {
            // set pressed button to true
            isPressed = true;

            // process yellow event immediately if the green light is already expired
            if (currLight == ExpGreen) {
                eventList.push(Event(simClock + Cross::MIN_DOUBLE, YellowEvent));
            }
        }
    }

    // if the pedestrian is arriving in the middle of the red, we need to check if it's possible for them to cross in the remaining time
    else {
        if (redEndTime > simClock) {
            walk(redEndTime - simClock); //remaining time = time that red light ends - current time
        }
        else {
            cerr << "Error: redEndTime incorrectly set" << endl;
        }
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
// for simplicity we only keep track of the first person in the arrived queue to press the button (as that will be the first button push that really counts if the button hasn't already been pushed)
void process_check_min(Person* fplb) {
    // (FPLB) first person left behind = first person in the queue AFTER the red light turns green
    
    // when checking if the FPLB needs to press the button after 1 minute, there are 3 cases
    // CASE 1: someone pressed the button when currLight was NewGreen and currLight is now Red
    // check the person is not in queue
    // CASE 2: someone pressed the button when currLight was ExpGreen and currLight is now Yellow
    // CASE 3: no one has pressed the button and currLight is stil ExpGreen
    // if (person in queue && !(isPressed)) { set isPressed to true and add in appropriate next event }
    bool inQueue = false;
    for (Person* p: personQueue) {
        if (p == fplb) {
            inQueue = true;
            break;
        }
    }
    
    if (inQueue && !isPressed) {
        isPressed = true;
        if (currLight == ExpGreen) {
            eventList.push(Event(simClock + Cross::MIN_DOUBLE, ExpGreenEvent));
        }
    }
    return;
}

// QUESTION: Edge case - if person 1-20 arrived at button and didn't press, then person 21 arrives and presses button (immediately turns yellow), then all 20 first people were able to cross and person 21 wasn't able to cross (by this point 8+18=26 seconds have passed), does their 1 minute timer run out after (60-26=34) more seconds or do we restart the 1 minute timer when the light goes back from red to newgreen? - I think it's the second one according to P-6B clarification



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
        if (personQueue[currInd]->calc_cross_time() < remainTime) {
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



/*
========================================================================================================
CAR RELATED EVENTS
========================================================================================================
*/

/*
PROCESS CAR ENTER EVENT
*/
void process_car_enter(Car* currCar) {
    // create a new event to have another car enter the simulation
    if (Q >= numCars) {
        numCars ++;
        double nextEnterTime = simClock + get_exponential(Cross::LAMBDA_A, autoTraceStream);
        // create new car object
        Car* newCar = new Car(nextEnterTime, currCar->get_direction());
        eventList.push(Event(nextEnterTime, CarEnterEvent, newCar));
    }
}

/*
DRIVE: Determine which cars can cross in the current light time
*/ // time = duration of current light
bool drive(Car car, double time) {
    double speedMPH = car.get_speed();

    // Convert the input miles per hour to the needed feet per second
    double speedFPS = ((speedMPH * 5280) / 60) / 60; // 5280 ft in a mile, 60 minutes in an hour, 60 seconds in a minute

    // Find how far they can travel since they arrived till the end of the yellow light
    //double distNeeded = (Cross::B * 3) + (Cross::B / 2) + (Cross::W / 2) + Cross::L;
    double distTravelled = speedFPS * ((simClock + time) - car.get_enter_time());

    // decide
    if (distTravelled >= Cross::MAX_DRIVE) {
        return true;
    }

    return false;
}

void calc_delay(Car car, double time) {
    
}