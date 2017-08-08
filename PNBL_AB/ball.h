#ifndef _BALL_
#define _BALL_

#include "globals.h"
#include "board.h"
#include "vec2.h"
#include <math.h>

#define ACCURACY    1
#define EMPTY       0.0

extern Arduboy2Base arduboy;
extern Sprites sprites;

#define sign(x) (x < 0) ? -1 : 1

class Ball {
public:
  Ball(int x, int y, vec2 initial_velocity) : velocity(initial_velocity) {
      pos = vec2(x, y);
      
      iteration = 0;
  }
  Ball(int x, int y) : Ball(x, y, vec2(0, 0)) {}
  Ball() : Ball(0, 0, vec2(0, 0)) {}

  void draw() {
    sprites.drawPlusMask(round(pos.x) - 2, round(pos.y) - 2 - camY, sprBall, 0);
  }

  void update() {
    velocity += gravity;
    /*Serial.println("Before approach: ");
    Serial.print("position: ");
    pos.print();
    Serial.println("");
    Serial.print("velocity: ");
    velocity.print();
    Serial.println();*/
    approachTarget();
    /*Serial.println("After approach: ");
    Serial.print("position: ");
    pos.print();
    Serial.println("");
    Serial.print("velocity: ");
    velocity.print();
    Serial.println();*/

    if (pos.y > 234 ) {
      pos = vec2(123, 219);
      velocity = vec2(0, 0);
    }
  }

  vec2 checkCollision(int x, int y) {
    boolean collision = false;
    vec2 reflection_vector(0, 0);
    if (!mapGetCollisionPoint(x, y, comBoard)) {
      collision = true;
      reflection_vector += mapGetNormalVector(x, y, comBoard);
    }
    //reflection_vector.convertNormal();
    if (collision && reflection_vector.getMagnitude() == EMPTY)
      return vec2 (0, 1);
    return reflection_vector;
  }

  void moveFromCollision() {
    boolean found = false;
    for (int k = -1; k <= 1; k++) {
      for (int j = -1; j <= 1; j++) {
        if (!found) {
          vec2 nv = checkCollision(pos.x + k, pos.y + j);
          if (nv.getMagnitude() == EMPTY) {
              found = true;
              pos.x = int(pos.x + k);
              pos.y = int(pos.y + j);
          }
        }
      }
    }
    /*if (!found)
      Serial.println("Dead Ball");*/
  }

  void approachTarget() {
    vec2 vnorm = velocity;
    vnorm.convertNormal();
    float vmag = velocity.getMagnitude();
    vec2 surface_normal = checkCollision(pos.x, pos.y);
    /*if (surface_normal.getMagnitude() != EMPTY) {
      moveFromCollision();
    }*/
    vec2 safe = pos;
    vec2 newpos(pos);
    vec2 vcopy = velocity;
    boolean collide = false;

    for (int i = 0; i < vmag; i += 1) {
      if (!collide) {
        //newpos += vnorm * inc;
        if (abs(vcopy.x) < 1) {
          newpos.x += vcopy.x;
          vcopy.x = 0;
        }
        else{
          newpos.x += sign(vcopy.x);
          vcopy.x -= sign(vcopy.x);
        }
        if (abs(vcopy.y) < 1) {
          newpos.y += vcopy.y;
          vcopy.y = 0;
        }
        else{
          newpos.y += sign(vcopy.y);
          vcopy.y -= sign(vcopy.y);
        }
  
        surface_normal = checkCollision(round(newpos.x), round(newpos.y));
        
        if (surface_normal.getMagnitude() != EMPTY) {
          collide = true;
          safe = newpos + surface_normal;
          safe = vec2(round(safe.x), round(safe.y));
          
          reflect(surface_normal);
          break;
        }
        else {
            safe = newpos; // don't remove again!
        }
      }
    }
    pos = safe;
  }

  /*void approachTarget() {
    vec2 vnorm = velocity;
    vnorm.convertNormal();
    float vmag = velocity.getMagnitude();
    vec2 surface_normal = checkCollision(pos.x, pos.y);
    if (surface_normal.getMagnitude() != 0.0) {
      moveFromCollision();
    }
    float subtract = 0;
    vec2 safe = pos;
    
    for (float i = 1; i < abs(int(vmag)) + 1; i += ACCURACY) {
      if (surface_normal.getMagnitude() == 0.0) {
        vec2 newpos(pos);
        newpos += vnorm * i;
  
        surface_normal = checkCollision(int(newpos.x), int(newpos.y));
        
        if (surface_normal.getMagnitude() != 0.0) {
          vec2 final = safe;
          pos.x = int(final.x);
          pos.y = int(final.y);
          reflect(surface_normal);
          break;
        }
        else {
            safe = newpos;
        }
      }
    }
    if (surface_normal.getMagnitude() == 0.0) {
      vec2 next = pos + velocity;
      if (checkCollision(next.x, next.y).getMagnitude() == 0.0)
        pos = next;
    }
  }*/

  void reflect(vec2 normal_vector) {
    vec2 vadded = (velocity.getNormal() + normal_vector);
    float reduction = max((vadded.x + vadded.y) / 3, 0.5);
    velocity -= normal_vector * 2 * (velocity * normal_vector);
    //velocity *= 0.6;
    velocity *= reduction;
    if (velocity.x == 0.0)
      velocity.x += random(1)/2 - 0.25;
  }

  void impulse(vec2 impulse_vector) {
    velocity += impulse_vector;
  }

  vec2 getPos() {
    return pos;
  }

  void print() {
    Serial.print("Ball: Position: ");
    pos.print();
    Serial.print(" Velocity: ");
    velocity.print();
    Serial.println();
  }

private:
  vec2 pos;
  vec2 velocity;

  //debug
  unsigned int iteration;
};


#endif

