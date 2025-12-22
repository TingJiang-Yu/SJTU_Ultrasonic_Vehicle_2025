#include "ultrasonic_controller.h"

ultrasonic_controller* ultrasonic_controller::instance = nullptr;

ultrasonic_controller::ultrasonic_controller()
{
    leftPin  = ULTRASONIC_RX_LEFT_PIN;
    rightPin = ULTRASONIC_RX_RIGHT_PIN;

    leftTime = 0;
    rightTime = 0;
    leftDetected = false;
    rightDetected = false;

    instance = this;
}

void ultrasonic_controller::init()
{
    pinMode(leftPin, INPUT);
    pinMode(rightPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(leftPin),  leftISR,  RISING);
    attachInterrupt(digitalPinToInterrupt(rightPin), rightISR, RISING);
}

void IRAM_ATTR ultrasonic_controller::leftISR()
{
    if (instance && !instance->leftDetected)
    {
        instance->leftTime = micros();
        instance->leftDetected = true;
    }
}

void IRAM_ATTR ultrasonic_controller::rightISR()
{
    if (instance && !instance->rightDetected)
    {
        instance->rightTime = micros();
        instance->rightDetected = true;
    }
}

bool ultrasonic_controller::available()
{
    return leftDetected && rightDetected;
}

long ultrasonic_controller::getTimeDiff()
{
    long diff = 0;

    if (leftDetected && rightDetected)
    {
        noInterrupts();
        diff = (long)leftTime - (long)rightTime;
        leftDetected = false;
        rightDetected = false;
        interrupts();
    }

    return diff;
}

void ultrasonic_controller::reset()
{
    noInterrupts();
    leftDetected = false;
    rightDetected = false;
    interrupts();
}
