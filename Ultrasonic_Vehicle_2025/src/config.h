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
#define ULTRASONIC_RX_LEFT_PIN  27   // GPIO27 - 左超声波接收（外部中断）
#define ULTRASONIC_RX_RIGHT_PIN 14   // GPIO14 - 右超声波接收（外部中断）

// ================= 时间测量参数 =================
#define TIME_RESOLUTION_US 1         // 时间分辨率1μs
#define MAX_TIME_DIFFERENCE 10000    // 最大时间差10ms（约3.4米距离差）
#define SPEED_OF_SOUND 0.0343        // 声速（cm/μs）

// ================= 位置计算参数 =================
#define RECEIVER_SPACING 10.0        // 两个接收器间距（cm）
#define ANGLE_MAX 60.0               // 最大可检测角度（度）

// ================= 系统参数 =================
#define MAX_SPEED 255          // 最大PWM值
#define MIN_SPEED 0            // 最小PWM值
#define BASE_SPEED 150         // 基础速度

// ================= 超声波参数 =================
#define ULTRASONIC_FREQUENCY 40000    // 40kHz
#define ULTRASONIC_BURST_COUNT 8      // 每次发射8个周期
#define ULTRASONIC_TIMEOUT_US 30000   // 30ms超时（约5米）

// ================= 超声波ADC配置 =================
#define ADC_LEFT_PIN 32        // GPIO32 - 左侧超声波ADC输入
#define ADC_RIGHT_PIN 33       // GPIO33 - 右侧超声波ADC输入
#define ADC_RESOLUTION 12      // ADC分辨率 (12位 = 0-4095)
#define ADC_ATTENUATION ADC_11db  // ADC衰减（提高测量范围）

// ================= 信号处理参数 =================
#define SIGNAL_NOISE_FLOOR 100  // 噪声阈值
#define SIGNAL_SATURATION 4000  // 信号饱和阈值
#define SIGNAL_MIN_VALID 150    // 最小有效信号

// ================= 距离校准参数 =================
#define DISTANCE_CALIBRATION_NEAR 30.0   // 30cm处的校准值
#define DISTANCE_CALIBRATION_FAR 100.0   // 100cm处的校准值
#define DISTANCE_MAX_RANGE 500.0         // 最大测量距离 (cm)
#define DISTANCE_MIN_RANGE 10.0          // 最小测量距离 (cm)

// ================= 跟随控制参数 =================
#define FOLLOW_TARGET_DISTANCE 50.0      // 目标跟随距离 (cm)
#define FOLLOW_MAX_DISTANCE 200.0        // 最大跟随距离 (cm)
#define FOLLOW_DIRECTION_DEADZONE 0.15   // 方向死区

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