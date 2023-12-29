#ifndef CORE_H
#define CORE_H

#include "raylib.h"
#include "raymath.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <limits>

#define oo std::numeric_limits<double>::max()

const double MAX_SPEED = 4.0;
const double MAX_FORCE = 0.2;

static const int CELLSIZE = 20;
static const int WIDTH    = 25;
static const int HEIGHT   = 25;
static const int OFFSET   = 65;
 
Vector2 operator+(const Vector2& a, const Vector2& b);
Vector2 operator-(const Vector2& a, const Vector2& b);
Vector2 operator*(const Vector2& a, const double& escalar);
Vector2 operator/(const Vector2& a, const double& escalar);
double mag(Vector2 &a);
double dist(Vector2 a, Vector2 b);
void normalize(Vector2 &a);
void clamp(Vector2 &a, double max, double min);
void setMag(Vector2 &a, double val);

#endif
