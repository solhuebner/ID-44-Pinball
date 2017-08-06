#ifndef _BALL_
#define _BALL_

#include "board.h"
#include "vec2.h"

#define ACCURACY    0.3

class Ball {
public:
  Ball(int x, int y, vec2 initial_velocity) : velocity(initial_velocity) {
      pos = vec2(x, y);
  }

  Ball(int x, int y) : Ball(x, y, vec2(0, 0)) {}

  void draw() {
    sprites.drawPlusMask(int(pos.x) - 2, int(pos.y) - 2 - cam_y, sprBall, 0);
  }

  void update() {
    velocity += gravity;

    approachTarget();

    if (pos.y > 130 ) {
      pos.y = -10;
      pos.x = (30 + random(80));
      velocity = vec2(0, 0);
    }
  }

  vec2 checkCollision(int x, int y) {
    boolean collision = false;
    vec2 reflection_vector(0, 0);
    for (int k = -2; k <= 2; k++) {
      for (int j = -2; j <= 2; j++) {
        if (!mapGetCollisionPoint(x + k, y + j, BallCollisionMap)) {
          collision = true;
          reflection_vector += mapGetNormalVector(x + k, y + j, BallCollisionMap);
        }
      }
    }
    reflection_vector.convertNormal();
    return reflection_vector;
  }

  void moveFromCollision() {
    boolean found = false;
    for (int k = -2; k <= 2; k++) {
      for (int j = -2; j <= 2; j++) {
        if (!found) {
          vec2 nv = checkCollision(pos.x + k, pos.y + j);
          if (nv.getMagnitude() == 0.0) {
              found = true;
              pos.x = int(pos.x + k);
              pos.y = int(pos.y + j);
          }
        }
      }
    }
    if (!found)
      Serial.println("Dead Ball");
  }

  /*void approachTarget() {
    float vmag = velocity.getMagnitude();
    for (int i = 0; i < abs(int(vmag)); i += 1) {
      vec2 vnorm = velocity;
      vnorm.convertNormal();
      vmag = velocity.getMagnitude();
      vec2 surface_normal = checkCollision(pos.x, pos.y);
      if (surface_normal.getMagnitude() != 0.0) {
        moveFromCollision();
      }
      else {
        vec2 newpos(pos);
        newpos += vnorm;
  
        surface_normal = checkCollision(newpos.x, newpos.y);
        
        if (surface_normal.getMagnitude() != 0.0) {
          reflect(surface_normal);
        }
        else {
          pos = newpos;
        }
      }
    }
    /*if (surface_normal.getMagnitude() == 0.0) {
      vec2 next = pos + velocity;
      if (checkCollision(next.x, next.y).getMagnitude() == 0.0)
        pos = next;
    }
  }*/

  void approachTarget() {
    vec2 vnorm = velocity;
    vnorm.convertNormal();
    float vmag = velocity.getMagnitude();
    vec2 surface_normal = checkCollision(pos.x, pos.y);
    if (surface_normal.getMagnitude() != 0.0) {
      moveFromCollision();
    }
    float subtract = 0;
    
    for (float i = 1; i < abs(int(vmag)) + 1; i += ACCURACY) {
      if (surface_normal.getMagnitude() == 0.0) {
        vec2 newpos(pos);
        newpos += vnorm * i;
  
        surface_normal = checkCollision(int(newpos.x), int(newpos.y));
        
        if (surface_normal.getMagnitude() != 0.0) {
          vec2 final = pos + vnorm * (i - ACCURACY);
          pos.x = int(final.x);
          pos.y = int(final.y);
          reflect(surface_normal);
          break;
        }
      }
    }
    if (surface_normal.getMagnitude() == 0.0) {
      vec2 next = pos + velocity;
      if (checkCollision(next.x, next.y).getMagnitude() == 0.0)
        pos = next;
        vec2 testnext = pos + velocity.getNormal();
        if (checkCollision(testnext.x, testnext.y).getMagnitude() == 0.00) {
          pos = testnext;
        }
    }
  }

  void reflect(vec2 normal_vector) {
    vec2 vadded = (velocity.getNormal() + normal_vector);
    float reduction = max((vadded.x + vadded.y) / 3, 0.5);
    velocity -= normal_vector * 2 * (velocity * normal_vector);
    //velocity *= 0.6;
    velocity *= reduction;
    if (velocity.x == 0.0)
      velocity.x += random(1)/2 - 0.25;
  }

private:
  vec2 pos;
  vec2 velocity;
};

class Pixel {
public:
  Pixel(int x, int y, vec2 initial_velocity) : velocity(initial_velocity) {
      pos = vec2(x, y);
  }
  Pixel(int x, int y) : Pixel(x, y, vec2(0, 0)) {}
  Pixel() : Pixel(0, 0, vec2(0, 0)) {}

  void draw() {
    sprites.drawPlusMask(int(pos.x) - 2, int(pos.y) - 2 - cam_y, sprBall, 0);
  }

  void update() {
    velocity += gravity;

    approachTarget();

    if (pos.y > 130 ) {
      pos.y = -10;
      pos.x = (30 + random(80));
      velocity = vec2(0, 0);
    }
  }

  vec2 checkCollision(int x, int y) {
    boolean collision = false;
    vec2 reflection_vector(0, 0);
    if (!mapGetCollisionPoint(x, y, BallCollisionMap)) {
      collision = true;
      reflection_vector += mapGetNormalVector(x, y, BallCollisionMap);
    }
    //reflection_vector.convertNormal();
    return reflection_vector;
  }

  void moveFromCollision() {
    boolean found = false;
    for (int k = -1; k <= 1; k++) {
      for (int j = -1; j <= 1; j++) {
        if (!found) {
          vec2 nv = checkCollision(pos.x + k, pos.y + j);
          if (nv.getMagnitude() == 0.0) {
              found = true;
              pos.x = int(pos.x + k);
              pos.y = int(pos.y + j);
          }
        }
      }
    }
    if (!found)
      Serial.println("Dead Ball");
  }

  /*void approachTarget() {
    float vmag = velocity.getMagnitude();
    for (int i = 0; i < abs(int(vmag)); i += 1) {
      vec2 vnorm = velocity;
      vnorm.convertNormal();
      vmag = velocity.getMagnitude();
      vec2 surface_normal = checkCollision(pos.x, pos.y);
      if (surface_normal.getMagnitude() != 0.0) {
        moveFromCollision();
      }
      else {
        vec2 newpos(pos);
        newpos += vnorm;
  
        surface_normal = checkCollision(newpos.x, newpos.y);
        
        if (surface_normal.getMagnitude() != 0.0) {
          reflect(surface_normal);
        }
        else {
          pos = newpos;
        }
      }
    }
    /*if (surface_normal.getMagnitude() == 0.0) {
      vec2 next = pos + velocity;
      if (checkCollision(next.x, next.y).getMagnitude() == 0.0)
        pos = next;
    }
  }*/

  void approachTarget() {
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
        /*vec2 testnext = pos + velocity.getNormal();
        if (checkCollision(testnext.x, testnext.y).getMagnitude() == 0.00) {
          pos = testnext;
        }*/
    }
  }

  void reflect(vec2 normal_vector) {
    vec2 vadded = (velocity.getNormal() + normal_vector);
    float reduction = max((vadded.x + vadded.y) / 3, 0.5);
    velocity -= normal_vector * 2 * (velocity * normal_vector);
    //velocity *= 0.6;
    velocity *= reduction;
    if (velocity.x == 0.0)
      velocity.x += random(1)/2 - 0.25;
  }

private:
  vec2 pos;
  vec2 velocity;
};


#endif

