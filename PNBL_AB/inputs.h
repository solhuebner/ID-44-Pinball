#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "ball.h"

extern Ball ball;

void checkInputs()
{
  if (arduboy.justPressed(A_BUTTON)) {
    ball.impulse(vec2(0, -10));
  }
  if (arduboy.justPressed(B_BUTTON)) {
    Serial.print("Position: ");
    ball.print();
  }
}


#endif
