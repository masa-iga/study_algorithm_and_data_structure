#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>

#include <vector>
#include <memory>
using namespace std;

#define DEBUG (0)
#define USE_UNIQUE_PTR (1)

#if DEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // DEBUG

int g_num = -1;
int g_time = 1;

class Vertex {
public:
  Vertex() :
    id_(-1),
    discover_time_(-1),
    finish_time_(-1),
    adjacentNum_(0),
#if USE_UNIQUE_PTR
    adjacentIds_(nullptr),
#endif
    isVisited_(false) { }

  ~Vertex() {
  }

  void setAdjacency(const vector<int> &adjacentIds);

  inline void setId(int id) { id_ = id; }
  inline void setDiscoverTime(int time) { discover_time_ = time; }
  inline void setFinishTime(int time) { finish_time_ = time; }
  inline void setVisited(bool visited) { isVisited_ = visited; }

  inline int getId() { return id_; }
  inline int getDiscoverTime() { return discover_time_; }
  inline int getFinishTime() { return finish_time_; }
  inline bool getVisited() { return isVisited_; }
  inline int getAdjacentNum() { return adjacentNum_; }
  inline int getAdjacentId(int idx) {
    if (adjacentIds_ == nullptr || idx > adjacentNum_)
      return -1;
    return adjacentIds_[idx];
  }

  void printTime();
  void printAdj();

private:
  int id_;
  int discover_time_;
  int finish_time_;

  int adjacentNum_;
#if USE_UNIQUE_PTR
  unique_ptr<int[]> adjacentIds_;
#else
  static const int kMaxId = 100;
  int adjacentIds_[kMaxId];
#endif // USE_UNIQUE_PTR

  bool isVisited_;
};


void Vertex::setAdjacency(const vector<int> &adjacentIds)
{
#if USE_UNIQUE_PTR
  if (adjacentIds_) assert(1);
#endif // USE_UNIQUE_PTR

  adjacentNum_ = adjacentIds.size();
#if USE_UNIQUE_PTR
  adjacentIds_ = unique_ptr<int[]>(new int[adjacentNum_]);
#endif // USE_UNIQUE_PTR

  for (int i = 0; i < adjacentNum_; i++)
  {
    adjacentIds_[i] = adjacentIds[i];
  }

}

void Vertex::printTime()
{
  printf("%d %d %d\n", id_, discover_time_, finish_time_);
}

void Vertex::printAdj()
{
  DPRINTF("%d %d", id_, adjacentNum_);
  for (int i = 0; i < adjacentNum_; i++) {
    printf(" %d", adjacentIds_[i]);
  }
  cout << endl;
}


void dfs(Vertex vertices[], int idx)
{

  DPRINTF("time %d  idx %d\n", g_time, idx);

  assert(idx <= g_num);

  Vertex &vtx = vertices[idx];

  if (vtx.getVisited())
    return ;

  vtx.setVisited(true);
  vtx.setDiscoverTime(g_time);
  g_time++;

  for (int i = 0; i < vtx.getAdjacentNum(); i++)
  {
    int id = vtx.getAdjacentId(i);
    dfs(vertices, id);
  }

  vtx.setFinishTime(g_time);
  g_time++;
}


int main()
{
  cin >> g_num; cin.ignore();
  DPRINTF("Node num %d\n", g_num);

  Vertex vertices[g_num+1];

  for (int i = 1; i <= g_num; i++)
  {
    int id;
    int num;

    cin >> id >> num;
    vertices[i].setId(id);
    DPRINTF("node %d (num %d)\n", id, num);

    vector<int> adjs;

    for (int j = 0; j < num; j++)
    {
      int adj;
      cin >> adj;

      adjs.push_back(adj);
    }

    if (adjs.size())
      vertices[i].setAdjacency(adjs);

    cin.ignore();
  }

#if DEBUG
  for (int i = 1; i <= g_num; i++) vertices[i].printAdj();
#endif

  for (int i = 1; i <= g_num; i++)
    if (!vertices[i].getVisited())
      dfs(vertices, i);

  for (int i = 1; i <= g_num; i++) vertices[i].printTime();

  return 0;
}

