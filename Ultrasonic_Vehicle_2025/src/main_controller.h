/*===================
* main_controller.h
=====================*/

#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <Arduino.h>
#include "pid_controller.h"
#include "motor_controller.h"
#include "ultrasonic_controller.h"

class main_controller {
public:
    main_controller();
    bool init();
    bool update();

private:
    pid_controller* pid;
    motor_controller* motor;
    ultrasonic_controller* ultrasonic;

    float dir_d;
    float dir_t;
    float dir_output;

    unsigned long lastSignalTime;

    enum State {
        SEARCHING,
        FOLLOWING
    } state;
};

#endif
