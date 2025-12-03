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

// QUESTION - CHECK ALL THE REQUIREMENTS IN THE DOC WITH KEITH (IN NOTES.MD)

/*
========================================================================================================
PUBLIC VARIABLES
========================================================================================================
*/


/*
- simClock (double): variable
    current time of the simulation
- isPressed (bool): variable
    checks if the button has already been pressed by a pedestrian during a NewGreen / ExpGreen
- currLight (lightType enum): variable
    current light signal
- personQueue (vector<Person>): variable
    the queue of people waiting at the crosswalk when the light is not Red
- carQueue (vector<Car>): variable
    the queue of cars waiting at the crosswalk when the light is Red
- eventList (priority_queue<Event?>):variable
    the priority queue of events to process with corresponding simulation clock times
- numWalked (int): variable
    the number of people that walked during a red light (up to 20)
- redEndTime (double): variable
    the simClock that the current red light will end (only set and updated when the light turns red)

- numPeople (int): variable
    current number of pedestrians in the simulation
- numCars (int): variable
    current number of cars in the simulation
- Q (int): variable
    Q = total number of autos, Q = total number of pedestrians the simulation should generate

- mean_DA (double): variable
    mean delay time of automobiles (calculated with Welfords)
- mean_DA (double): variable 
    mean delay tiime of pedestrians (calculated with Welfords)
- v_A (double): variable
    sample sum of squared deviations of automobile delays (calculated with Welfords) 
    NOTE: the final value to report should be variance s2_A = v_A / Q
*/

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

// Statistics Variables (these will be outputted at the end)
// Using Welford's Equations (initialize mean )
double mean_DA = 0;
double mean_DP = 0;
double v_A = 0; //NOTE: the final value to report should be variance s2_A = v_A / Q

/*
========================================================================================================
MAIN FUNCTION: RUN CROSSWALK SIMULATION
========================================================================================================
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
    if (eventList.empty()) {
        cerr << "Error: eventList is empty" << endl;
        exit(1);
    }
    // get first event
    Event curEvent = eventList.top();
    eventList.pop();
    // set simulation clock time to that of the event
    simClock = curEvent.get_process_time();

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
        simClock = curEvent.get_process_time();
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

    check_carQueue();
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

    // car logic
    check_carQueue();
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
WALK: process people in the queue to walk during a red light
*/
// remainTime = remaining time in the red light
void walk(double remainTime) {
    // index of current person we're checking
    int currInd = 0;
    // P-7: up to 20 pedestrians can cross in 1 cycle of the Red light
    // we need to have a counter because if there is a person that arrives to the light later in the red light and can make it they should be able to pass
    // iterate through the list of people and see if their walking time will be able to reach 
    while (!personQueue.empty() && numWalked < Cross::MAX_WALK_NUM && currInd < (int)personQueue.size()) {
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

// called only during processing start of red event and new green event
// TODO: silly me forgor to iterate the carInd
void check_carQueue() {
    int carInd = 0;
    while (carInd < (int)carQueue.size() && !carQueue.empty() ) { 
        if (currLight == NewGreen) {
            if (check_must_stop(carQueue[carInd])) {carQueue[carInd].set_stopped();}
            if (carQueue[carInd].get_stopped()) { // Cars that arrived during the previous green or yellow lights and were told to stop or if they arrived during red and needed to stop
                calc_actual_time(carQueue[carInd]);
            } else {
                // cars that didn't stop will have same actual time as optimal time
                carQueue[carInd].set_actual_time(carQueue[carInd].get_optimal_time());
            }

            // all cars leaving get these updated
            update_car_stats();
            carQueue.erase(carQueue.begin() + carInd); // pop off, yaaaAAs queen, you go gurl

        } else if (currLight == Red) {
            if (check_must_stop(carQueue[carInd])) {
                carQueue[carInd].set_stopped();
            } 

            carInd ++;

        } else { // paranoia else statement that should never technically be called
            cerr << "Error: check_carQueue() method was called on a " << currLight << " light." << endl;
            exit(1);
        }

    }
}

// actual time = before crosswalk time + deceleration time + wait time + acceleration time + after crosswalk time
// before crosswalk time = before distance * speed
// deceleration time = acceleration time = [FILL]
// after crosswalk time = after distance * speed
// before distance = Cross::DRIVE_CROSS_FRONT - braking distance
// after distance = Cross::DRIVE_CROSS_FRONT + braking distance = AKA Cross::DRIVE_CROSS_END - braking distance
void calc_actual_time(Car& car) {
    // Find distances
    double changingSpeedDist = (car.get_speed() * car.get_speed()) / (2 * Cross::ACC); // the distance the car travels while accelerating or deccelerating
    double constBeforeDist = Cross::DRIVE_CROSS_FRONT - changingSpeedDist; // the distance the car travels while going a constant speed
    double constAfterDist = Cross::DRIVE_CROSS_END - changingSpeedDist;

    // Convert distances to time durations
    double changingSpeedTime = car.get_speed() / Cross::ACC;
    double constBeforeTime = constBeforeDist / car.get_speed();
    double constAfterTime = constAfterDist / car.get_speed();

    // this only works if this is only called at a new green light when the car drives off
    // if this is a green light, the simClock time will be the end of the cars wait time
    double stoppedTime = simClock - (car.get_enter_time() + changingSpeedTime + constBeforeTime);

    // Set the new actual time
    double actualTime = (changingSpeedTime * 2) + constBeforeTime + constAfterTime + stoppedTime;
    car.set_actual_time(actualTime);
}

bool check_must_stop(Car& car) {
    // distance traveled = speed * time elapsed
    double distTraveled = car.get_speed() * (simClock - car.get_enter_time());
    if (distTraveled >= Cross::DRIVE_CROSS_END) {
        return false;
    }

    return true;
}




/*
========================================================================================================
STATISTICS
========================================================================================================
*/


void update_car_stats() {

}

void update_person_stats() {

}

void output_stats() {

}