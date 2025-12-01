#ifndef EVENT_HPP
#define EVENT_HPP

#include "Person.hpp"
#include "Car.hpp"

using namespace std;


/*
PERSON: EVENT
    Purpose: Contain information about events and event types

    Private:
        - processTime (double): variable
            simulation time on when to process event
        - type (EventType enum): variable
            type of event that's being processed
        - assocPerson (Person*): variable
            person object associated with PersonEnterEvent, PersonArriveEvent, CheckMinEvent types
        - assocCar (Car*): variable
            car object associated with CarEnterEvent, CarArriveEvent types

    Public:
        - get_process_time() (double): method
            returns processTime
        - get_type() (EventType enum): method
            returns type
        - get_assoc_person() (Person*): method
            returns assocPerson if event is the correct type to contain Person object
        - get_assoc_car() (Car*): method
            returns assocCar if event is the correct type of contain Car object
        - set_assoc_person(Person*) (void): method
            sets associated Person if event is the correct type
        - set_assoc_car(Car*) (void): method
            sets associated Car if event is the correct type
        - operator>(Event& other) (bool): comparison operator
            compares by process time then enum time
        


*/

enum EventType: char {
    NewGreenEvent,
    ExpGreenEvent,
    YellowEvent,
    RedEvent,
    PersonEnterEvent,
    PersonArriveEvent,
    CarEnterEvent,
    CarArriveEvent,
    CheckMinEvent
};


class Event {
    private:
        double processTime;
        enum EventType type;
        Person* assocPerson;
        Car* assocCar;

    public:
        Event(const double, const EventType);
        Event(const double, const EventType, const Person*);
        Event(const double, const EventType, const Car*);

        // GETTERS
        double get_process_time();
        EventType get_type();
        Person* get_assoc_person();
        Car* get_assoc_car();

        // SETTERS
        void set_assoc_person(const Person*);
        void set_assoc_car(const Car*);

        // comparison operator for priority queue
        bool operator>(const Event& other) {
            if (processTime != other.get_process_time()) {
                return processTime > other.get_process_time();
            }
            // if the process times are the same, compare by the types of the event
            // TODO: REORDER THE ENUM DEFINITION TO BE IN THE CORRECT PRIORITY
            return type > other.get_type();
        }
        

};

#endif