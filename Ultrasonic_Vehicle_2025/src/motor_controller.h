/*===================
* motor_controller.h
=====================*/

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class motor_controller {
public:
    motor_controller();
    void init();

    // 设置转向分量（PID 输出）
    void setTurn(int turn);

    // 搜索模式：原地旋转
    void search(int speed);

    void stop();

private:
    uint8_t leftPin;
    uint8_t rightPin;

    int baseSpeed;
    int turnSpeed;

    unsigned long lastPwmUpdate;

    void applyOutput();
    void driveMotor(int pin, int speed);
};

#endif
