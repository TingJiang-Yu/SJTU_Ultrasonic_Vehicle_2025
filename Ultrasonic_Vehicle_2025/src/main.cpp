#include <Arduino.h>
#include "main_controller.h"

main_controller ctrl;

void setup() 
{
    Serial.begin(SERIAL_BAUDRATE);
    ctrl.init();
}

void loop() 
{
    ctrl.update();
}
