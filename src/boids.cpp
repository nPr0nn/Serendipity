#include "../headers/boids.h"
#include "../headers/colors.h"

boid::boid(Vector2 pos, Vector2 vel, Vector2 acc, double radius) {
    this->pos = pos;
    this->vel = vel;
    this->acc = acc;
    this->radius = radius;
    this->visible = true;
}

Vector2 boid::alignment(std::vector<boid> &boids) {
    float radius = 50.0;
    Vector2 avg = {0,0};
    int cont = 0;

    for(boid &b : boids) {
        double d = dist(this->pos, b.pos);
        if(d > 0 && d < radius) {
            avg = avg + b.vel;
            cont++;
        }
    }

    Vector2 steer = {0.0, 0.0};
    if(cont > 0) {
        steer = avg / cont;
        setMag(steer, MAX_SPEED);
        steer = steer - this->vel;
        clamp(steer, -oo, MAX_FORCE);
    }
    return steer;
}

Vector2 boid::cohesion(std::vector<boid> &boids) {
    float radius = 50.0;
    Vector2 avg = {0,0};
    int cont = 0;

    for(boid &b : boids) {
        double d = dist(this->pos, b.pos);
        if(d > 0 && d < radius) {
            avg = avg + b.pos;
            cont++;
        }
    }

    Vector2 steer = {0.0, 0.0};
    if(cont > 0) { 
        steer = avg / cont;
        steer = steer - this->pos;
        setMag(steer, MAX_SPEED);
        steer = steer - this->vel;
        clamp(steer, -oo, MAX_FORCE);  
    }

    return steer;
}

Vector2 boid::separation(std::vector<boid> &boids) {
    float radius = 50.0;
    Vector2 avg = {0,0};
    int cont = 0;

    for(boid &b : boids) {
        double d = dist(this->pos, b.pos);
        if(d > 0 && d < radius ) {
            Vector2 diff = this->pos - b.pos; 
            diff = diff / d;
            avg = avg + diff;
            cont++;
        }
    }

    Vector2 steer = {0.0, 0.0};
    if(cont > 0) {
        steer = avg / cont;
        setMag(steer, MAX_SPEED);
        steer = steer - this->vel;
        clamp(steer, -oo, MAX_FORCE); 
    }

    return steer;    
}

Vector2 boid::target(std::vector<boid> &boids, Vector2 t){

  Vector2 steer = {0.0, 0.0};
  float theta = GetRandomValue(0, 360);
  Vector2 circ = {cos(DEG2RAD*theta), sin(DEG2RAD*theta)};

  steer = (t + circ*4) - this->pos;
  if(IsMouseButtonDown(1) && dist(this->pos, t) < 500){
    steer = steer*(-1); 
  }
  clamp(steer, -oo, MAX_FORCE);
  
  return steer;
}

void boid::update(std::vector<boid> boids, Vector2 t, bool state) {
    Vector2 align_rule = alignment(boids);
    Vector2 cohesion_rule = cohesion(boids);
    Vector2 separation_rule = separation(boids);
    Vector2 target_rule = target(boids, t);
 
    acc = target_rule + separation_rule*1.5 + align_rule*2 + cohesion_rule;   
    vel = vel + acc;
    pos = pos + vel;

    if(state) clamp(vel, -oo, 0.8*MAX_SPEED);
    else clamp(vel, -oo, MAX_SPEED);
    acc = acc * 0;
}
