
#include "headers/utils.h"
#include "headers/boids.h"
#include "headers/colors.h"

typedef struct Nest{
  Vector2 pos;
  double radius;
  bool active;
} Nest;

std::vector<boid> boids_group;
std::vector<boid> preds;
std::vector<Nest> nests;

int number_of_boids = 1;
int number_of_nests = 1000;
int number_of_pred   = 10;

Vector2 center_of_mass;

void new_boid(int W, int H, Camera2D camera){
  Vector2 pos = {(float)GetRandomValue(0, W), (float)GetRandomValue(0, H)};
  Vector2 vel = {(float)GetRandomValue(0, 5), (float)GetRandomValue(0, 5)}; 
  pos = GetScreenToWorld2D(pos, camera);
  setMag(vel, MAX_SPEED);
  boids_group.push_back(boid(pos, vel, {0,0}, 8));
}

void new_pred(int W, int H, Camera2D camera){
  Vector2 pos = {(float)GetRandomValue(-10*W, 10*W), (float)GetRandomValue(-10*H, 10*H)};
  Vector2 vel = {(float)GetRandomValue(0, 2), (float)GetRandomValue(0, 2)}; 
  pos = GetScreenToWorld2D(pos, camera);
  setMag(vel, MAX_SPEED);
  preds.push_back(boid(pos, vel, {0,0}, 8));
}

void new_boid2(Vector2 pos, Camera2D camera){
  Vector2 vel = {(float)GetRandomValue(0, 5), (float)GetRandomValue(0, 5)}; 
  setMag(vel, MAX_SPEED);
  boids_group.push_back(boid(pos, vel, {0,0}, 8));
  number_of_boids++;
}

void game_init(int W, int H, Camera2D camera){

  // Init boids
  for(int i = 0; i < number_of_boids; i++){
    new_boid(W, H, camera);
  }

  for(int i = 0; i < number_of_pred; i++){
    new_pred(W, H, camera);
  }

  int g = 5;
  for(int i = 0; i < number_of_nests; i++){
     Nest n;
     n.pos = {(float)GetRandomValue(-W*g, W*g), (float)GetRandomValue(-H*g, H*g)};
     n.pos = GetScreenToWorld2D(n.pos, camera);
     n.radius = GetRandomValue(6, 30);
     n.active = true; 
     nests.push_back(n);
  }
}

double call_radius = 0;
bool calling = false;
double init_calling_time = 0;
double time_of_calling = 1;
bool loopa = false;

int update_loop(Rectangle box, Vector2 target, int W, int H, Camera2D camera){
 
  bool state = (IsKeyPressed(KEY_SPACE) || IsMouseButtonDown(0) || IsMouseButtonDown(1));
  
  for(auto &p : preds){
    p.update(preds, target, state);
  }

  for(auto &boid : boids_group){ 
    for(auto &p : preds){
      if(dist(p.pos, boid.pos) < 15 && boid.visible == true){
        boid.visible = false;
        number_of_boids--;
      }
    }

    boid.update(boids_group, target); 
  }

  if(IsKeyPressed(KEY_SPACE) && calling == false){
    calling = true;
    loopa = true;
    init_calling_time = GetTime();
  }
  if(calling == true){
    if(GetTime() - init_calling_time > time_of_calling){
      calling = false;
      call_radius = 0;
    }
    else{
      call_radius += 5;
    }
  }

  if(loopa == true){
    for(auto &nest : nests){
      if(nest.active == true){ 
        float nest_dist = dist(nest.pos, target);
        int is_there = GetRandomValue(0, 100); 
        if( (nest_dist < call_radius) && (is_there > 20) ){
          new_boid2(nest.pos, camera);
          nest.active = false;
        }  
      }
    }
  }
  
  return number_of_boids;
}

void draw_loop(Vector2 target){
  
  Vector2 dis = {6, 6};
  
  for(auto &boid : boids_group){ 
    Vector2 head = boid.pos - target;
    if(IsMouseButtonDown(1)) head = head*(-1);
    setMag(head, boid.radius);

    float theta = 60;
    Vector2 tail1 = Vector2Rotate(head, theta);
    Vector2 tail2 = Vector2Rotate(head, 127);

    if(boid.visible){
    DrawTriangle(head + boid.pos + dis, tail1 + boid.pos + dis, tail2 + boid.pos + dis, d4);
    DrawTriangle(head + boid.pos, tail1 + boid.pos, tail2 + boid.pos, l1);                                    
    }
  }
  
  for(auto &p : preds){ 
    Vector2 head = p.pos - target;
    if(IsMouseButtonDown(1)) head = head*(-1);
    setMag(head, 2*p.radius);

    float theta = 60;
    Vector2 tail1 = Vector2Rotate(head, theta);
    Vector2 tail2 = Vector2Rotate(head, 127);

    DrawTriangle(head + p.pos + dis, tail1 + p.pos + dis, tail2 + p.pos + dis, d4);
    DrawTriangle(head + p.pos, tail1 + p.pos, tail2 + p.pos, l3);                                    
  }

  for(auto &nest : nests){
    DrawCircleV(nest.pos + dis, nest.radius, d4);

    if(nest.active == true) DrawCircleV(nest.pos, nest.radius, l2);
    else{
      DrawCircleV(nest.pos, nest.radius, d2);
    }
  }

  if(calling){
    DrawCircleLines(target.x, target.y, call_radius, l1); 
  }


}
