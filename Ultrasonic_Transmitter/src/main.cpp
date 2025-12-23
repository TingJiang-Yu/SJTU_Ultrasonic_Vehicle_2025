#include <Arduino.h>
#include "driver/mcpwm.h"

#define ULTRASONIC_PWM_GPIO     4
#define ULTRASONIC_FREQ_HZ      40000

#define BURST_ON_MS             10
#define BURST_OFF_MS            10

static bool burstOn = false;
static unsigned long lastSwitchTime = 0;

void setup()
{
    Serial.begin(115200);

    // 1. 绑定 GPIO 到 MCPWM
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, ULTRASONIC_PWM_GPIO);

    // 2. 配置 PWM
    mcpwm_config_t pwm_config = {
        .frequency = ULTRASONIC_FREQ_HZ,
        .cmpr_a = 0.0,              // 初始不发射
        .cmpr_b = 0.0,
        .duty_mode = MCPWM_DUTY_MODE_0,
        .counter_mode = MCPWM_UP_COUNTER,
    };

    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    mcpwm_set_duty_type(
        MCPWM_UNIT_0,
        MCPWM_TIMER_0,
        MCPWM_OPR_A,
        MCPWM_DUTY_MODE_0
    );

    lastSwitchTime = millis();
}

void loop()
{
    unsigned long now = millis();

    if (burstOn)
    {
        // 正在发射
        if (now - lastSwitchTime >= BURST_ON_MS)
        {
            // 关闭 PWM（静默）
            mcpwm_set_duty(
                MCPWM_UNIT_0,
                MCPWM_TIMER_0,
                MCPWM_OPR_A,
                0.0
            );

            burstOn = false;
            lastSwitchTime = now;
        }
    }
    else
    {
        // 静默状态
        if (now - lastSwitchTime >= BURST_OFF_MS)
        {
            // 开启 40kHz PWM
            mcpwm_set_duty(
                MCPWM_UNIT_0,
                MCPWM_TIMER_0,
                MCPWM_OPR_A,
                50.0
            );

            burstOn = true;
            lastSwitchTime = now;
        }
    }
}
