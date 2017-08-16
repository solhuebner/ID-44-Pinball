#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "elements.h"
#include "ball.h"

Ball ball;

void stateMenuPlay()
//void stateGamePrepareLevel()
{
  ball = Ball(123, 219);
  gameState = STATE_GAME_NEXT_LEVEL;
};


void stateGameNextLevel()
{
  level++;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  checkInputs();
  ball.update();
  camY = max(min(232, round(ball.getPos().y) - 32), 0);
  sprites.drawSelfMasked(0, -camY, sprBoard, 0);
  ball.draw();
};

void stateGamePause()
{
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};

void stateGameOver()
{
  
};

void gameUpdatePhysics() {
  ball.physicsUpdate();
}

#endif
