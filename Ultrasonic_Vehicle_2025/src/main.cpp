#include <Arduino.h>
#include "main_controller.h"

main_controller ctrl;

void setup() 
{
    ctrl.init();
}

void loop() 
{
    ctrl.update();
}
