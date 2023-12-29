#include "../headers/utils.h"

Vector2 operator+(const Vector2& a, const Vector2& b) {
    Vector2 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

Vector2 operator-(const Vector2& a, const Vector2& b) {
    Vector2 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

Vector2 operator*(const Vector2& a, const double& scalar) {
    Vector2 r;
    r.x = a.x * scalar;
    r.y = a.y * scalar;
    return r;
}

Vector2 operator/(const Vector2& a, const double& scalar) {
    Vector2 r;
    r.x = a.x / scalar;
    r.y = a.y / scalar;
    return r;
}

double norm(Vector2 &a) {
    return sqrt(a.x*a.x + a.y*a.y);
}

double dist(Vector2 a, Vector2 b) {
    return sqrt((a.x - b.x)*(a.x- b.x) + (a.y - b.y)*(a.y - b.y));
}

void normalize(Vector2 &a) {
    double m = norm(a);
    a = a / m;
}

void setMag(Vector2 &a, double val) {
  normalize(a);
  a = a * val;
}

void clamp(Vector2 &a, double min, double max) {
    double m = norm(a);
    if(m > max) {
      normalize(a);
      a = a * max;
    }
    else if(m < min){
      normalize(a);
      a = a * min;
    }
}
