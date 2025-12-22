#include "motor_controller.h"

motor_controller::motor_controller()
    : leftAPin(MOTOR_LEFT_A_PIN),
      leftBPin(MOTOR_LEFT_B_PIN),
      rightAPin(MOTOR_RIGHT_A_PIN),
      rightBPin(MOTOR_RIGHT_B_PIN),
      baseSpeed(BASE_SPEED),
      turnSpeed(0)
{}

void motor_controller::init()
{
    pinMode(leftAPin, OUTPUT);
    pinMode(leftBPin, OUTPUT);
    pinMode(rightAPin, OUTPUT);
    pinMode(rightBPin, OUTPUT);
    stop();
}

void motor_controller::setTurn(int turn)
{
    turnSpeed = constrain(turn, -MAX_SPEED, MAX_SPEED);
    applyOutput();
}

void motor_controller::applyOutput()
{
    int leftSpeed  = baseSpeed + turnSpeed;
    int rightSpeed = baseSpeed - turnSpeed;

    leftSpeed  = constrain(leftSpeed,  -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);

    driveMotor(leftAPin, leftBPin, leftSpeed);
    driveMotor(rightAPin, rightBPin, rightSpeed);
}

void motor_controller::search(int speed)
{
    // 原地旋转：左右反向
    driveMotor(leftAPin, leftBPin,  speed);
    driveMotor(rightAPin, rightBPin, -speed);
}

void motor_controller::driveMotor(int pinA, int pinB, int speed)
{
    if (speed > 0) {
        analogWrite(pinA, speed);
        analogWrite(pinB, 0);
    } else if (speed < 0) {
        analogWrite(pinA, 0);
        analogWrite(pinB, -speed);
    } else {
        analogWrite(pinA, 0);
        analogWrite(pinB, 0);
    }
}

void motor_controller::stop()
{
    analogWrite(leftAPin, 0);
    analogWrite(leftBPin, 0);
    analogWrite(rightAPin, 0);
    analogWrite(rightBPin, 0);
}
