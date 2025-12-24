/*===================
* main.cpp
=====================*/

#include <Arduino.h>
#include "main_controller.h"

main_controller mc;

void setup() 
{
    Serial.begin(SERIAL_BAUDRATE);
    mc.init();
}

void loop() 
{
    mc.update();
}
