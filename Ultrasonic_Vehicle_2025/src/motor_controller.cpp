#include "motor_controller.h"

motor_controller::motor_controller()
    : leftPin(MOTOR_LEFT_PIN),
      rightPin(MOTOR_RIGHT_PIN),
      baseSpeed(MOTOR_BASE_SPEED),
      turnSpeed(0),
      lastPwmUpdate(0)
{}

void motor_controller::init()
{
    pinMode(leftPin, OUTPUT);
    pinMode(rightPin, OUTPUT);

    // B 端在硬件上已拉高
    stop();
}

void motor_controller::setTurn(int turn)
{
    // turn 是“速度差分量”，不做反相
    turnSpeed = constrain(turn, -MOTOR_MAX_SPEED, MOTOR_MAX_SPEED);
    applyOutput();
}

void motor_controller::applyOutput()
{
    unsigned long now = millis();
    if (now - lastPwmUpdate < PWM_UPDATE_INTERVAL_MS) return;
    lastPwmUpdate = now;

    int leftSpeed  = baseSpeed + turnSpeed;
    int rightSpeed = baseSpeed - turnSpeed;

    leftSpeed  = constrain(leftSpeed,  0, MOTOR_MAX_SPEED);
    rightSpeed = constrain(rightSpeed, 0, MOTOR_MAX_SPEED);

    driveMotor(leftPin, leftSpeed);
    driveMotor(rightPin, rightSpeed);
}

void motor_controller::search(int speed)
{
    unsigned long now = millis();
    if (now - lastPwmUpdate < PWM_UPDATE_INTERVAL_MS) return;
    lastPwmUpdate = now;

    speed = constrain(speed, 0, MOTOR_MAX_SPEED);

    // 原地旋转：一侧快，一侧慢（而不是反向）
    driveMotor(leftPin,  speed);
    driveMotor(rightPin, 0);
}

void motor_controller::driveMotor(int pin, int speed)
{
    // speed: 0 ~ MAX_SPEED
    speed = constrain(speed, 0, MOTOR_MAX_SPEED);

    // 反相 PWM：speed 越大，LOW 占空比越大
    int pwmValue = MOTOR_MAX_SPEED - speed;

    analogWrite(pin, pwmValue);
}

void motor_controller::stop()
{
    // speed = 0 → pwm = MAX_SPEED → A 恒 HIGH → 停
    analogWrite(leftPin,  MOTOR_MAX_SPEED);
    analogWrite(rightPin, MOTOR_MAX_SPEED);
}
