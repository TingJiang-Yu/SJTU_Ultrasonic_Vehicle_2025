// test_motor.ino - 电机测试程序
#include "motor_controller.h"

MotorController motor;

void setup() {
    Serial.begin(115200);
    delay(1000);

    motor.init();
}

void loop() {
    motor.forward(50);
    delay(1000);
    // 空闲
}
