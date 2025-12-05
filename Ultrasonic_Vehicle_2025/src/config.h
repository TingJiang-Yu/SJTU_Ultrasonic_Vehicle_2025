// Config.h - ESP32版本
#ifndef CONFIG_H
#define CONFIG_H

// ================= 电机控制引脚 =================
// ESP32推荐使用带PWM功能的引脚
#define MOTOR_LEFT_A_PIN 16     // GPIO16 - 左轮电机A
#define MOTOR_LEFT_B_PIN 17     // GPIO17 - 左轮电机B
#define MOTOR_RIGHT_A_PIN 18    // GPIO18 - 右轮电机A
#define MOTOR_RIGHT_B_PIN 19    // GPIO19 - 右轮电机B

// ================= 超声波引脚 =================
// ESP32所有引脚都支持外部中断
#define ULTRASONIC_TX1_PIN 25   // GPIO25 - 超声波发射1
#define ULTRASONIC_TX2_PIN 26   // GPIO26 - 超声波发射2
#define ULTRASONIC_RX1_PIN 27   // GPIO27 - 超声波接收1（外部中断）
#define ULTRASONIC_RX2_PIN 14   // GPIO14 - 超声波接收2（外部中断）

// ================= PWM配置 =================
// ESP32使用LEDC控制器
#define PWM_FREQUENCY 8000      // PWM频率（Hz）
#define PWM_RESOLUTION 8        // PWM分辨率（8位 = 0-255）
#define PWM_CHANNEL_LEFT_A 0    // 左轮电机A PWM通道
#define PWM_CHANNEL_LEFT_B 1    // 左轮电机B PWM通道
#define PWM_CHANNEL_RIGHT_A 2   // 右轮电机A PWM通道
#define PWM_CHANNEL_RIGHT_B 3   // 右轮电机B PWM通道

// ================= 系统参数 =================
#define MAX_SPEED 255          // 最大PWM值
#define MIN_SPEED 0            // 最小PWM值
#define BASE_SPEED 150         // 基础速度

// ================= 超声波参数 =================
#define ULTRASONIC_FREQUENCY 40000    // 40kHz
#define ULTRASONIC_BURST_COUNT 8      // 每次发射8个周期
#define ULTRASONIC_TIMEOUT_US 30000   // 30ms超时（约5米）

// ================= 硬件定时器配置 =================
#define TIMER_NUMBER 0         // 使用硬件定时器0
#define TIMER_DIVIDER 80       // 分频系数（80MHz/80 = 1MHz = 1μs分辨率）
#define TIMER_ALARM_VALUE 25   // 25μs中断（40kHz）

// ================= PID参数 =================
#define PID_KP 1.0
#define PID_KI 0.1
#define PID_KD 0.05

// ================= 调试标志 =================
#define DEBUG_ENABLED true
#define SERIAL_BAUDRATE 115200

// ================= ESP32特定配置 =================
#define CORE_0 0
#define CORE_1 1

// 任务优先级
#define ULTRASONIC_TASK_PRIORITY 3
#define MOTOR_TASK_PRIORITY 2
#define PID_TASK_PRIORITY 2

// 堆栈大小（字节）
#define ULTRASONIC_TASK_STACK 4096
#define MOTOR_TASK_STACK 2048
#define PID_TASK_STACK 2048

// ================= 引脚功能检查 =================
/*
ESP32引脚注意事项：
1. GPIO 6-11: 用于连接Flash，不建议使用
2. GPIO 34-39: 仅输入，无内部上拉
3. GPIO 0, 2, 12, 15: 上电时有特殊状态
4. 推荐PWM引脚: 2, 4, 12-19, 21-23, 25-27, 32-33
*/

#endif // CONFIG_H