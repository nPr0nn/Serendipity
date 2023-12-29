#ifndef BOID_H
#define BOID_H

#include "utils.h"

class boid { 
    public:
        Vector2 pos;
        Vector2 vel;
        Vector2 acc;
        double  radius;
        bool visible;
        
        boid(Vector2 pos, Vector2 vel, Vector2 acc, double radius);

        void render(Vector2 p, float r);
        void update(std::vector<boid> boids, Vector2 t, bool state=false);
        Vector2 separation(std::vector<boid> &boids);
        Vector2 alignment(std::vector<boid> &boids);
        Vector2 cohesion(std::vector<boid> &boids);
        Vector2 target(std::vector<boid> &boids, Vector2 t);
};

#endif
