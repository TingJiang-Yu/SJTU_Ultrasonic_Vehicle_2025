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
    int leftAPin, leftBPin;
    int rightAPin, rightBPin;

    int baseSpeed;
    int turnSpeed;

    void applyOutput();
    void driveMotor(int pinA, int pinB, int speed);
};

#endif
