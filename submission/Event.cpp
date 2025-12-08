#include <string>
#include <iostream>

#include "Event.hpp"
#include "Person.hpp"
#include "Car.hpp"
#include "Crosswalk.h"

using namespace std;

// CONSTRUCTORS

Event::Event(const double time, const EventType type) {
    processTime = time;
    this->type = type;
    assocPerson = nullptr;
    assocCar = nullptr;
};

Event::Event(const double time, const EventType type, Person* attachPerson) {
    processTime = time;
    this->type = type;
    if (type == PersonEnterEvent || type == PersonArriveEvent || type == CheckMinEvent) {
        assocPerson = attachPerson;
    }
    else {
        cerr << "Error: cannot initialized Event object because event type does not allow for associated pedestrian" << endl;
        exit(1);
    }
}

Event::Event(const double time, const EventType type, Car* attachCar) {
    processTime = time;
    this->type = type;
    if (type == CarEnterEvent) {
        assocCar = attachCar;
    }
    else {
        cerr << "Error: cannot initialized Event object because event type does not allow for associated car" << endl;
        exit(1);
    }
}

// GETTERS

double Event::get_process_time() {
    return processTime;
}

EventType Event::get_type() {
    return type;
}

Person* Event::get_assoc_person() {
    if (type == PersonEnterEvent || type == PersonArriveEvent || type == CheckMinEvent) {
        if (assocPerson == nullptr) {
            cerr << "Error: no associated pedestrian with this event" << endl;
            exit(1);
        }
        else {
            return assocPerson;
        }
    }
    else {
        cerr << "Error: invalid event type - should not contain an associated pedestrian" << endl;
        exit(1);
    }
}

Car* Event::get_assoc_car() {
    if (type == CarEnterEvent) {
        if (assocCar == nullptr) {
            cerr << "Error: no associated car with this event" << endl;
            exit(1);
        }
        else {
            return assocCar;
        }
    }
    else {
        cerr << "Error: invalid event type - should not contain an associated car" << endl;
        exit(1);
    }
}

string Event::get_name() {
    switch (type) {
        case EventType::NewGreenEvent:
            return "New Green";
        case EventType::ExpGreenEvent:
            return "Exp Green";
        case EventType::YellowEvent:
            return "Yellow";
        case EventType::RedEvent:
            return "Red";
        case EventType::PersonEnterEvent:
            return "P Enter";
        case EventType::PersonArriveEvent:
            return "P Arrive";
        case EventType::CarEnterEvent:
            return "C Enter";
        case EventType::CheckMinEvent:
            return "Check Min";
    }

    return "NONONONONO";
}

// SETTERS

void Event::set_assoc_person(Person* attachPerson) {
    if (type == PersonEnterEvent || type == PersonArriveEvent || type == CheckMinEvent) {
        assocPerson = attachPerson;
    }
    else {
        cerr << "Error: invalid event type - cannot add associated pedestrian" << endl;
        exit(1);
    }
}

void Event::set_assoc_car(Car* attachCar) {
    if (type == CarEnterEvent) {
        assocCar = attachCar;
    }
    else {
        cerr << "Error: invalid event type - cannot add associated car" << endl;
        exit(1);
    }
}
