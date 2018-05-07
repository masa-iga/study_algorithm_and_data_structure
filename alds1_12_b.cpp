#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>

#include <memory>
#include <queue>
using namespace std;

#define DEBUG (1)

#if DEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // DEBUG


const int kMaxVertex = 100;
int g_num_of_vertex;
int g_min_weight[kMaxVertex];

struct Edge {
    Edge(int num_vertex)
        : num_vertex_(num_vertex)
    {
        const int size_of_ptr_ = num_vertex_ * num_vertex_;
        weight_ = unique_ptr<int>(new int[size_of_ptr_]);

        for (int i = 0; i < num_vertex_; i++)
            for (int j = 0; j < num_vertex_; j++)
                setWeight(i, j, kInvalidWeight);

        for (int i = 0; i < num_vertex_; i++)
            setWeight(i, i, 0);
    }

    const int kInvalidWeight = INT_MAX;

    bool isConnected(int v0, int v1) const;
    vector<int> getConnectedVertices(int v) const;
    int getWeight(int v0, int v1) const;
    void setWeight(int v0, int v1, int w);

    void printWeight() const;

    int num_vertex_;
    int size_of_ptr_;
    unique_ptr<int> weight_;
};

bool Edge::isConnected(int v0, int v1) const
{
    if (v0 == v1)
        return false;

    if (getWeight(v0, v1) == kInvalidWeight)
        return false;

    return true;
}

vector<int> Edge::getConnectedVertices(int v) const
{
    vector<int> vertices;

    for (int i = 0; i < num_vertex_; i++)
        if (isConnected(v, i))
            vertices.push_back(i);

    return vertices;
}

void Edge::setWeight(int v0, int v1, int w)
{
    assert(0 <= v0 && v0 < num_vertex_);
    assert(0 <= v1 && v1 < num_vertex_);

    int *p = weight_.get() + (v0 * num_vertex_) + v1;
    *p = w;
}

int Edge::getWeight(int v0, int v1) const
{
    int *p = weight_.get() + (v0 * num_vertex_) + v1;
    return *p;
}

void Edge::printWeight() const
{
    DPRINTF("\n");
    for (int i = 0; i < num_vertex_; i++)
    {
        printf("    [%d]", i);
        for (int j = 0; j < num_vertex_; j++)
        {
            const int w = getWeight(i, j);
            if (w != kInvalidWeight)
                printf(" %d", getWeight(i, j));
            else
                printf(" -");
        }
        cout << endl;
    }
}


struct VtxAndWeight {
    VtxAndWeight(int vtx, int weight)
        : vtx_(vtx)
        , weight_(weight)
    {}

    int vtx_;
    int weight_;

    bool operator>(const VtxAndWeight& rhs)
    {
        return (*this).weight_ > rhs.weight_;
    };

    bool operator<(const VtxAndWeight& rhs)
    {
        return (*this).weight_ < rhs.weight_;
    };
};

struct LessThanByWeight
{
    bool operator()(const VtxAndWeight& lhs, const VtxAndWeight& rhs) const
    {
        return lhs.weight_ < rhs.weight_;
    }

};


/* Dijkstra's algorithm */
void calcShortestPath(int start, const Edge &edge)
{
    priority_queue<VtxAndWeight, vector<VtxAndWeight>, LessThanByWeight> priority_queue;

    const int init_weight = 0;
    g_min_weight[start] = 0;
    priority_queue.push(VtxAndWeight(start, init_weight));

    while (!priority_queue.empty())
    {
        // select the minimum weight from the priority queue
        const int vtx = priority_queue.top().vtx_;
        const int weight = priority_queue.top().weight_;
        priority_queue.pop();

        DPRINTF("vtx %d  weight %d\n", vtx, weight);

        // find connected vertices
        vector<int> connectedVertices = edge.getConnectedVertices(vtx);

        // update min weight for the vertices
        for (int neighbour : connectedVertices)
        {
            const int updated_weight= weight + edge.getWeight(vtx, neighbour);

            if (updated_weight < g_min_weight[neighbour])
            {
                g_min_weight[neighbour] = updated_weight;
                priority_queue.push(VtxAndWeight(neighbour, g_min_weight[neighbour]));
            }

            DPRINTF("g_min_weight[%d] %d\n", neighbour, g_min_weight[neighbour]);
        }
    }

}


int main()
{
    cin >> g_num_of_vertex;
    cin.ignore();

    // init
    for (int i = 0; i < g_num_of_vertex; i++)
    {
        g_min_weight[i] = INT_MAX;
    }

    Edge edge(g_num_of_vertex);

    // input
    for (int i = 0; i < g_num_of_vertex; i++)
    {
        int start, degree;
        cin >> start >> degree;

        for (int j = 0; j < degree; j++)
        {
            int dst, weight;
            cin >> dst >> weight;

            edge.setWeight(start, dst, weight);
        }

        cin.ignore();
    }

#if DEBUG
    edge.printWeight();
#endif

    // process
    const int start = 0;
    calcShortestPath(start, edge);

    // output
    for (int i = 0; i < g_num_of_vertex; i++)
        printf("%d %d\n", i, g_min_weight[i]);

    return 0;
}


