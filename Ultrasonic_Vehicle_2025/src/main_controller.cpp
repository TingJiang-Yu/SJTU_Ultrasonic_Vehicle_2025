/*===================
* main_controller.cpp
=====================*/

#include "main_controller.h"

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

    ultrasonic->checkSilence();

    if (ultrasonic->available())
    {
        int16_t timeDiff = ultrasonic->getTimeDiff();

        double dirNorm = (double)timeDiff / 32767.0;
        dirNorm = constrain(dirNorm, -1.0, 1.0);

        if (fabs(dirNorm) < PID_DEAD_ZONE) dirNorm = 0.0;

        // 低通滤波
        dir_t = LPF_ALPHA * dirNorm + (1.0f - LPF_ALPHA) * dir_t;

        pid->Calc(dir_t);

        int turn_output = (int)(dir_output * MOTOR_MAX_TURN_SPEED);

        Serial.print("car_state: ");
        Serial.println(state);
        Serial.print("time_diff: ");
        Serial.println(timeDiff);
        Serial.print("dir_norm: ");
        Serial.println(dirNorm);
        Serial.print("dir_t: ");
        Serial.println(dir_t);
        Serial.print("turn_output: ");
        Serial.println(turn_output);
        Serial.println();

        motor->setTurn(turn_output);

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

            Serial.println("car_state: SEARCHING");
        }
            
    }

    return true;
}

