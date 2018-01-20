#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <math.h>
using namespace std;

#define PI 3.14159265

#define DEBUG (0)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

class Point {
public:
  Point(float x, float y) :
    x_ (x), y_(y) { }
  //Point operator- (const Point& p) const;
  float x_, y_;

private:
};

class Vector {
public:
  Vector(float x, float y) :
    x_(x), y_(y) { }
  float x_, y_;

private:
};

Point rotation60(Point p0, Point p1)
{
  /*
  R = (cos  -sin) (s)
      (sin   cos) (t)
  */
  float s = p1.x_ - p0.x_;
  float t = p1.y_ - p0.y_;

  float Rx = cos(60.f * PI / 180.f) * s - sin(60.f * PI / 180.f) * t + p0.x_;
  float Ry = sin(60.f * PI / 180.f) * s + cos(60.f * PI / 180.f) * t + p0.y_;

  return Point(Rx, Ry);
}


void kochCurve(int n, Point p0, Point p1)
{
  if (n < 1)
  {
    printf("%f %f\n", p0.x_, p0.y_);
    return ;
  }

  float vx = p1.x_ - p0.x_;
  float vy = p1.y_ - p0.y_;

  Point s(p0.x_ + vx * (1.f/3.f), p0.y_ + vy * (1.f/3.f));
  Point t(p0.x_ + vx * (2.f/3.f), p0.y_ + vy * (2.f/3.f));
  Point u = rotation60(s, t);

#if DEBUG
  DPRINTF("(%1.2f, %1.2f), ", p0.x_, p0.y_);
  printf("(%1.2f, %1.2f), ",  s.x_,  s.y_);
  printf("(%1.2f, %1.2f), ",  u.x_,  u.y_);
  printf("(%1.2f, %1.2f), ",  t.x_,  t.y_);
  printf("(%1.2f, %1.2f)\n", p1.x_, p1.y_);
#endif // DEBUG

  kochCurve(n-1, p0,  s);
  kochCurve(n-1,  s,  u);
  kochCurve(n-1,  u,  t);
  kochCurve(n-1,  t, p1);

}

int main()
{
  int n;
  cin >> n;

  DPRINTF("N = %d\n", n);

  Point first(0.0, 0.0), last(100.0, 0.0);
  kochCurve(n, first, last);
  printf("%f %f\n", last.x_, last.y_);

  return 0;
}
