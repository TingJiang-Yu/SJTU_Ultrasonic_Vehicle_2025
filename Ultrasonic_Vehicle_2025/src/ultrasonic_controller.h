/*===================
* ultrasonic_controller.h
=====================*/

#ifndef ULTRASONIC_CONTROLLER_H
#define ULTRASONIC_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class ultrasonic_controller
{
public:
    ultrasonic_controller();
    void init();

    // 是否已经捕获到“一次完整且未消费的 timeDiff”
    bool available();

    // 读取本次事件的时间差（只会成功一次）
    int16_t getTimeDiff();

    void checkSilence();

private:
    uint8_t leftPin;
    uint8_t rightPin;

    // 首次下降沿时间戳
    volatile unsigned long leftFirstTime;
    volatile unsigned long rightFirstTime;

    // 是否已捕获首沿
    volatile bool leftCaptured;
    volatile bool rightCaptured;

    // 本次超声事件是否已产出结果
    volatile bool diffReady;

    // 用于防止同一事件内重复触发
    volatile bool eventLocked;

    volatile unsigned long eventEndTime;   // 记录静默期结束的时间
    volatile bool eventSilence;             // 控制静默期

    static ultrasonic_controller* instance;

    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
};

#endif
