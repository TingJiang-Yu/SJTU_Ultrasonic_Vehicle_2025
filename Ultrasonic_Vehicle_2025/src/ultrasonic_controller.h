#ifndef ULTRASONIC_CONTROLLER_H
#define ULTRASONIC_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

class ultrasonic_controller {
private:
    // 时间测量变量
    volatile uint32_t leftRiseTime;     // 左接收器上升沿时间
    volatile uint32_t rightRiseTime;    // 右接收器上升沿时间
    volatile bool leftDetected;         // 左接收器检测到信号
    volatile bool rightDetected;        // 右接收器检测到信号
    volatile bool measurementReady;     // 测量完成标志
    
    // 滤波和校准
    static const int HISTORY_SIZE = 10; // 历史数据缓冲区
    int32_t timeDiffHistory[HISTORY_SIZE];
    int historyIndex;
    int32_t filteredTimeDiff;           // 滤波后的时间差
    
    // 校准参数
    int32_t timeOffset;                 // 时间校准偏移（硬件延迟差）
    uint32_t lastMeasurementTime;       // 上次测量时间
    bool isCalibrated;                  // 是否已校准
    
public:
    ultrasonic_controller();
    
    // 初始化超声波控制器
    void init();
    
    // 开始测量
    void startMeasurement();
    
    // 更新测量数据（需要在主循环中调用）
    void update();
    
    // 获取时间差（μs），正值表示左先收到，负值表示右先收到
    int32_t getTimeDifference();
    
    // 获取方向角度（度），负值表示左，正值表示右
    float getDirectionAngle();
    
    // 获取方向偏差 (-1.0 到 1.0)
    float getDirectionBias();
    
    // 检查是否有有效信号
    bool hasValidSignal();
    
    // 手动触发校准
    void calibrate();
    
private:
    // 中断服务函数（必须声明为静态）
    static void leftRiseISR();
    static void rightRiseISR();
    
    // 静态指针用于在ISR中访问实例
    static ultrasonic_controller* instance;
    
    // 私有方法
    void processMeasurement();
    void resetMeasurement();
    int32_t applyFilter(int32_t newValue);
    float calculateAngleFromTimeDiff(int32_t timeDiff);
    
    // 硬件定时器配置
    void initTimer();
};

// 全局实例声明
extern ultrasonic_controller ultrasonic;

#endif // ULTRASONIC_CONTROLLER_H