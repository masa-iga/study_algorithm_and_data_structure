#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <climits>
using namespace std;

#define DEBUG (1)

#if DEBUG
#define DPRINTF(...)  printf("D(L%d) %s: ", __LINE__, __func__); printf(__VA_ARGS__)
#else
#define DPRINTF(...)
#endif // DEBUG

const char UP   = '/';
const char DOWN = '\\';
const char FLAT = '_';

class Graph {
public:
  Graph() :
    size_(0) {}
  void add(int y);
  int getYat(int x);
  int size() { return size_; }
  int slop(int x0, int x1);
  void print();
private:
  static const int MAX_SIZE = 20000 + 1;
  int maxY();
  int minY();
  int y_[MAX_SIZE];
  int size_;
};

void Graph::add(int y)
{
  y_[size_] = y;
  size_++;
}

int Graph::getYat(int x)
{
  assert(x < size_);
  return y_[x];
}

int Graph::slop(int x0, int x1)
{
  assert(x0 != x1);
  int yd = getYat(x1) - getYat(x0);
  int xd = x1 - x0;
  return yd/xd;
}

void Graph::print()
{
  int max = maxY();
  int min = minY();
  const int maxX = size();
  const int maxY = max - min + 1;
  char c[maxY][maxX];

  for (int y = 0; y < maxY; y++) {
    for (int x = 0; x < maxX; x++) {
      c[y][x] = ' ';
    }
  }

  for (int x = 0; x < size(); x++) {
    int y = getYat(x) - min;
    c[y][x] = '+';
  }

  for (int y = maxY-1; y > -1; y--) {
    for (int x = 0; x < maxX; x++) {
      printf("%c", c[y][x]);
    }
    cout << endl;
  }

  for( int x = 0; x < maxX; x++) {
    if (x % 10 == 0)  cout << '|';
    else              cout << ' ';
  }
  cout << endl;
}

int Graph::maxY()
{
  int y = INT_MIN;
  for (int i = 0; i < size(); i++) {
    if (getYat(i) > y)
      y = getYat(i);
  }
  return y;
}

int Graph::minY()
{
  int y = INT_MAX;
  for (int i = 0; i < size(); i++) {
    if (getYat(i) < y)
      y = getYat(i);
  }
  return y;
}

class Interval {
public:
  Interval() :
    x0_(-1), x1_(-1) { }
  int x0_, x1_;
};

int main() {
  Graph graph;
  int y = 0;
  graph.add(y);
  char c;
  while (cin >> c) {
    if (c == UP)  y++;
    else if (c == DOWN) y--;
    graph.add(y);
  }

#if DEBUG
  graph.print();
#endif

  // head, tailのポインタを用意して両端からflood areaを確定していく
  int head = 0, tail = graph.size() - 1;

  // Set head and tail
  //while (head+1 < tail && !(graph.slop(head, head+1) < 0))
  while (head+1 < graph.size() && !(graph.slop(head, head+1) < 0))
    head++;
  while (tail-1 > -1 && !(graph.slop(tail-1, tail) > 0))
    tail--;
  DPRINTF("HEAD %d   TAIL %d\n", head, tail);


  vector<Interval> intervals;
  while (head < tail) {

    Interval interval;
    interval.x0_ = head;

    int next_head = head + 1;
    int y = graph.getYat(head);

    while (next_head < tail && y != graph.getYat(next_head)) {
      next_head++;
    }

    if (y != graph.getYat(next_head))
      break;

    interval.x1_ = next_head;
    intervals.push_back(interval);
    DPRINTF("ADD INTERVAL S %d   E %d\n", interval.x0_, interval.x1_);

    // Move to the next peak or the edge
    while (next_head + 1 < tail && !(graph.slop(next_head, next_head+1) < 0)) {
      next_head++;
    }
    head = next_head;

  }

  DPRINTF("HEAD %d   TAIL %d\n", head, tail);

  auto size = intervals.size();
  // Back tail to head if possible
  while (head < tail)
  {
    Interval interval;
    interval.x1_ = tail;

    int next_tail = tail - 1;
    int y = graph.getYat(tail);

    while (head < next_tail && y != graph.getYat(next_tail)) {
      next_tail--;
    }

    if (y != graph.getYat(next_tail))
      break;

    interval.x0_ = next_tail;
    // don't push to the end of the container; align in ascending order to X
    intervals.insert(intervals.begin() + size, interval);
    DPRINTF("ADD INTERVAL S %d   E %d\n", interval.x0_, interval.x1_);

    // Move to the next peak
    while (head < next_tail && !(graph.slop(next_tail - 1, next_tail) > 0)) {
      next_tail--;
    }
    tail = next_tail;

  }

  // calculate each flood area
  int totalArea = 0;
  vector<int> areas;

  for (auto itr = intervals.begin(); itr != intervals.end(); itr++) {
    int depth = 0;
    float area = 0.0f;
    for (int x = itr->x0_; x < itr->x1_; x++) {
      if (graph.slop(x, x+1) < 0) // '\'
      {
        area += depth + 0.5;
        depth++;
      }
      else if (graph.slop(x, x+1) > 0) // '/'
      {
        area += depth - 0.5;
        depth--;
      }
      else // slop(x, x+1) // '_'
      {
        area += depth;
      }
    }
    DPRINTF("AREA %.1f (S %d   E %d)\n", area, itr->x0_, itr->x1_);
    areas.push_back((int)area);
    totalArea += (int)area;
  }

  // print messages
  cout << totalArea << endl;
  cout << intervals.size();
  for (int i = 0; i < areas.size(); i++) {
    cout << " " << areas[i];
  }
  cout << endl;
  return 0;
}

