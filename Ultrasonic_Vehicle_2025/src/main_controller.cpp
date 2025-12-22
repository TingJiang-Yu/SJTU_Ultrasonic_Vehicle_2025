#include "main_controller.h"

#define SIGNAL_TIMEOUT_MS 300
#define SEARCH_TURN_SPEED 80

main_controller::main_controller()
{
    pid = new pid_controller();
    motor = new motor_controller();
    ultrasonic = new ultrasonic_controller();

    dir_d = PID_DESIRED_DATA;
    dir_t = 0.0f;
    dir_output = 0.0f;

    lastSignalTime = 0;
    state = SEARCHING;
}

bool main_controller::init()
{
    motor->init();
    ultrasonic->init();

    pid->init(&dir_d, &dir_output);
    pid->SetK(PID_KP, PID_KI, PID_KD);
    pid->SetL(PID_MAX_INTEGRAL, PID_MIN_OUTPUT, PID_MAX_OUTPUT);

    return true;
}

bool main_controller::update()
{
    unsigned long now = millis();

    if (ultrasonic->available())
    {
        long timeDiff = ultrasonic->getTimeDiff();
        timeDiff = constrain(timeDiff, -MAX_TIME_DIFFERENCE, MAX_TIME_DIFFERENCE);

        dir_t = (float)timeDiff;
        pid->Calc(dir_t);

        motor->setTurn((int)dir_output);

        lastSignalTime = now;
        state = FOLLOWING;
    }
    else
    {
        if (state == FOLLOWING && (now - lastSignalTime > SIGNAL_TIMEOUT_MS))
        {
            state = SEARCHING;
        }

        if (state == SEARCHING)
        {
            motor->search(SEARCH_TURN_SPEED);
        }
    }

    return true;
}
