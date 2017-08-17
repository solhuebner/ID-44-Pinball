#ifndef _FLIPPER_
#define _FLIPPER_

#include <math.h>
#include "ball.h"

extern Ball ball;

//#define ANGLE_START   205.00 * PI() / 180
//#define ANGLE_END     150.00 * PI() / 180
//#define ROT_SPEED     5.00 * PI() / 180

#define ANGLE_START     3.577
#define ANGLE_END       2.618
#define ROT_ACCEL       0.005
#define FLIPPER_LENGTH  22

class Flipper {
public:
  boolean flipping;

  Flipper(int x, int y, int8_t rotation_direction) {
    rotDirection = rotation_direction;
    rotSpeed = 0;
    flipping = false;
    rotation = ANGLE_START;
    baseX = x;
    baseY = y;
  }

  void draw() {
    arduboy.drawLine(baseX, baseY - camY,
                    baseX + (rotDirection * cos(rotation) * FLIPPER_LENGTH),
                    baseY - (sin(rotation) * FLIPPER_LENGTH) - camY, BLACK
                    );
  }

  void update() {

  }

  void physicsUpdate() {
    if (flipping)
      rotSpeed += ROT_ACCEL;
    else
      rotSpeed -= ROT_ACCEL;

    rotation -= rotSpeed;
    if (rotation < ANGLE_END) {
      rotSpeed = 0;
      rotation = ANGLE_END;
    }
    if (rotation > ANGLE_START) {
      rotSpeed = 0;
      rotation = ANGLE_START;
    }

    vec2 col = checkCollision();
    if (col != vec2(0, 0)) {
      //Serial.print("Collision. rotSpeed: ");
      //Serial.print(rotSpeed);
      ball.reflect(getNormal());
      vec2 imp = getNormal() * getBaseDistance() * rotSpeed * 0.5;
      ball.setPos(col + getNormal());
      //Serial.print(", impulse: ");
      imp.print();
      //Serial.println();
      ball.impulse(imp);
      rotSpeed = 0;
    }
  }

  vec2 checkCollision() {
    tipX = baseX + (rotDirection * cos(rotation) * FLIPPER_LENGTH);
    tipY = baseY - (sin(rotation) * FLIPPER_LENGTH);

    float bx = ball.getPos().x;
    float by = ball.getPos().y;
    // Check horizontal component of collision
    if (bx > min(baseX, tipX) && bx < max(baseX, tipX)) {
      // Check vertical component of collision
      float liney = getLineY(bx);
      if (by >= liney && by < liney + 5) {
        return vec2(bx, liney);
      }
    }
    return vec2(0, 0);
  }

  float getSlope() {
    return (float(tipY - baseY) / float(tipX - baseX));
  }

  float getLineY(float x) {
    return (getSlope() * (x - baseX) + baseY);
  }

  vec2 getNormal() {
    return vec2(rotDirection * cos(rotation - PI/2), -sin(rotation - PI/2));
  }

  float getBaseDistance() {
    return sqrt(pow(ball.getPos().x - baseX, 2) + pow(getLineY(ball.getPos().y) - baseY, 2));
  }
  
protected:
  int8_t rotDirection;
  float rotSpeed;
  float rotation;
  int baseX;
  int baseY;
  int tipX;
  int tipY;
};

#endif
