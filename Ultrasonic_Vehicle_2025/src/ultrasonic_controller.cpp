/*===================
* ultrasonic_controller.cpp
=====================*/

#include "ultrasonic_controller.h"

ultrasonic_controller* ultrasonic_controller::instance = nullptr;

ultrasonic_controller::ultrasonic_controller()
{
    leftPin  = ULTRASONIC_RX_LEFT_PIN;
    rightPin = ULTRASONIC_RX_RIGHT_PIN;

    leftCaptured  = false;
    rightCaptured = false;
    diffReady     = false;
    eventLocked   = false;
    eventSilence  = false;
    eventEndTime  = 0;

    instance = this;
}

void ultrasonic_controller::init()
{
    pinMode(leftPin, INPUT);
    pinMode(rightPin, INPUT);

    attachInterrupt(digitalPinToInterrupt(leftPin),  leftISR,  FALLING);
    attachInterrupt(digitalPinToInterrupt(rightPin), rightISR, FALLING);
}

/*
 * ISR 设计原则：
 * - 一次超声事件内，只允许记录“第一次下降沿”
 * - 一旦 diffReady = true，本事件后续所有下降沿全部忽略
 */

void IRAM_ATTR ultrasonic_controller::leftISR()
{
    if (!instance) return;
    if (instance->eventLocked) return;

    if (!instance->leftCaptured)
    {
        instance->leftFirstTime = micros();
        instance->leftCaptured = true;

        if (instance->rightCaptured)
        {
            instance->diffReady   = true;
            instance->eventLocked = true;
        }
    }
}

void IRAM_ATTR ultrasonic_controller::rightISR()
{
    if (!instance) return;
    if (instance->eventLocked) return;

    if (!instance->rightCaptured)
    {
        instance->rightFirstTime = micros();
        instance->rightCaptured = true;

        if (instance->leftCaptured)
        {
            instance->diffReady   = true;
            instance->eventLocked = true;
        }
    }
}

bool ultrasonic_controller::available()
{
    return diffReady && !eventSilence;
}

int16_t ultrasonic_controller::getTimeDiff()
{
    if (!diffReady) return 0;

    noInterrupts();
    long diff = (long)leftFirstTime - (long)rightFirstTime;
    leftCaptured  = false;
    rightCaptured = false;
    diffReady     = false;
    eventLocked   = false;
    interrupts();

    // 限幅（安全）
    if (diff > ULTRASONIC_MAX_TIME_DIFFERENCE) diff = ULTRASONIC_MAX_TIME_DIFFERENCE;
    else if (diff < -ULTRASONIC_MAX_TIME_DIFFERENCE) diff = -ULTRASONIC_MAX_TIME_DIFFERENCE;
    
    // 死区
    if (abs(diff) <= ULTRASONIC_TIMEDIFF_DEADZONE) return 0;  

    eventSilence = true;
    eventEndTime = millis() + ULTRASONIC_SILENCE_TIME;

    // 将时间差映射到 Q15 整数范围
    int32_t q15 = (diff * 32767L) / ULTRASONIC_MAX_TIME_DIFFERENCE;

    return (int16_t)q15;
}

void ultrasonic_controller::checkSilence()
{
    // 检查静默期是否结束
    if (eventSilence && millis() >= eventEndTime)
    {
        eventSilence = false;  // 结束静默期，允许下一次事件
    }
}