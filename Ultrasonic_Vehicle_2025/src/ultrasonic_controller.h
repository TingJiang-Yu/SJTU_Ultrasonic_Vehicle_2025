#ifndef ULTRASONIC_CONTROLLER_H
#define ULTRASONIC_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class ultrasonic_controller
{
private:
    uint8_t leftPin;
    uint8_t rightPin;

    volatile unsigned long leftTime;
    volatile unsigned long rightTime;
    volatile bool leftDetected;
    volatile bool rightDetected;

    // 中断服务函数
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();

    static ultrasonic_controller* instance;

public:
    ultrasonic_controller();

    void init();

    // 是否获得了一次完整的左右检测
    bool available();

    // 获取左右到达时间差（微秒）
    int16_t getTimeDiff();

    // 手动复位（一般不需要外部调用）
    void reset();

    // 供外部访问的时间戳（调试用）
    unsigned long getLeftTime() const { return leftTime; }
    unsigned long getRightTime() const { return rightTime; }
};

#endif
