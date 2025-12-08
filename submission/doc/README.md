Welford's Equations
- Standard Means: 
    - Car Mean (variable = mean_DA): Crosswalk.cpp, line 562, function update _car_stats(Car&)
    - Pedestrian Mean (variable = mean_DP): Crosswalk.cpp, line 575, function update_person_stats(Person*)
- Sum of Squared Deviations:
    - Car Variance (variable = v_A): Crosswalk.cpp, line 566, function update_car_stats(Car&);
- Variance:
    - Car Deviation (variable = v_A / Q): Crosswalk.cpp, line 591, function outputs_stats

Exponential Interarrival Times
- Call of exponential function:
    - Cars: Crosswalk.cpp, line 444, function process_car_enter(Car*)
    - Pedestrians: Crosswalk.cpp, line 292, process_person_enter(Peson*)
- Exponential Function: Distribution.cpp, line 13, function get_exponential(double, ifstream&)

Event List
- Declared: Crosswalk.cpp, line 64, variable = eventList of type priority_queue
- Comparison Operator: Event.hpp, line 82
- Events Added
    - Person Enter: initialize on line 124, 139, also in process_person_enter() line 295 (only two PersonEnter events can exist at a time since two are initialized and each event only triggers one new PersonEnter event when processed)
        - Clarification: This event describes when a new pedestrian enters the simulation
    - Person Arrive: created during process_person_enter() line 299
        - Clarification: This event describes when a pedestrian arrived at the crosswalk button
    - Car Enter: initialized on line 131, 146, also in process_car_enter() line 447 (only two CarEnter events can exist at a time since two are initialized and each event only triggers one new CarEnter event when processed)
    - ExpGreen: created during new process_new_green() line 209
    - Yellow: created during process_exp_green() line 240 and in process_person_arrive() line 321 and in process_check_min() line 287
    - Red: created during process_yellow() line 253
    - New Green: created during proces_red() line 267
    - CheckMin:
        - process_new_green() line 218
        - process_person_arrive() line 327
- Event Removal
    - All removed in main function, at the start of the while loop line 162