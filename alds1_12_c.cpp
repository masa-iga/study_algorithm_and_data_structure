#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <climits>
#include <cinttypes>

#include <memory>
#include <queue>
#include <map>
#include <unordered_map>
using namespace std;

#define NDEBUG
#include <cassert>

#define MYDEBUG (0)

#if MYDEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // MYDEBUG


static const int kMaxVertex = 10 * 1000;
static const int kMaxWeight = 100 * 1000;
static const int kMaxNumPath = 500 * 1000;
static const int kInvalidWeight = -1;

static int g_num_of_vertex;
static int g_min_weight[kMaxVertex];


/*
   2D data structure ([x][y] = value)
*/
struct DstAndWeight {
    int dst_;
    int weight_;
};


struct StartIdxAndLen {
    int start_;
    int len_;
};


class WeightGraph {
public:
    WeightGraph()
    {
        num_weight_ = 0;
        num_idx_len_ = 0;

        const int n = kMaxNumPath;
        weights_ = unique_ptr<DstAndWeight[]>(new DstAndWeight[n]);

        DPRINTF("allocated weights_ ptr (%lu * %d bytes)\n", sizeof(weights_), n);

        const int n2 = kMaxVertex;
        idx_len_ = unique_ptr<StartIdxAndLen[]>(new StartIdxAndLen[n2]);

        for (int i = 0; i < n2; i++)
        {
            idx_len_[i].start_ = -1;
            idx_len_[i].len_ = -1;
        }

        DPRINTF("allocated idx_len_ ptr (%lu * %d bytes)\n", sizeof(idx_len_), n2);
    }

    void setWeight(int from, vector<int> &tos, vector<int> &weights);
    int getWeight(int from, int to) const;
    vector<int> getConnectedVertices(int from) const;

private:
    inline int getLen(int v) const { return idx_len_[v].len_; }
    inline int getStartIdx(int v) const { return idx_len_[v].start_; }

    unique_ptr<DstAndWeight[]> weights_;
    unique_ptr<StartIdxAndLen[]> idx_len_;

    uint32_t num_weight_;
    uint32_t num_idx_len_;
};


// sort a and b in ascending order based on a's value
int partition(vector<int> &a, vector<int> &b, int start, int end)
{
    const int pivot = a[(start+end)/2];
    int i = start, j = end;

    while (i < j)
    {
        while (a[i] < pivot)   i++;
        while (a[j] > pivot)   j--;

        if (i < j)
        {
            const int t0 = a[i], t1 = b[i];
            a[i] = a[j]; b[i] = b[j];
            a[j] = t0; b[j] = t1;
            i++; j--;
        }
    }

    return i;
}


void quicksort(vector<int> &a, vector<int> &b, int start, int end)
{
    if (start >= end)
        return;

    const int p = partition(a, b, start, end);
    quicksort(a, b, start, p);
    quicksort(a, b, p+1, end);
}


//void WeightGraph::setWeight(int from, const vector<int> &tos, const vector<int> &weights)
// This function manipulates inputs vectors to be sorted in ascending order
void WeightGraph::setWeight(int from, vector<int> &tos, vector<int> &weights)
{
    assert(0 <= from && from < kMaxNumPath);
    assert(tos.size() == weights.size());
    assert(num_weight_ + tos.size() < kMaxNumPath);

    idx_len_[from].start_ = num_weight_;
    idx_len_[from].len_ = tos.size();

    quicksort(tos, weights, 0, tos.size() - 1);

    for (int i = 0; i < tos.size(); i++)
    {
        weights_[num_weight_].dst_ = tos[i];
        weights_[num_weight_].weight_ = weights[i];
        num_weight_++;
    }

    num_idx_len_++;

}


int WeightGraph::getWeight(int from, int to) const
{
    const int start = getStartIdx(from);
    const int len = getLen(from);

    assert(0 <= start);
    assert(start + len <= num_weight_);

    // binary search; assuming index sorted in ascending order
    int left = start;
    int right = start + (len - 1);

    while (left <= right)
    {
        const int center = (left + right)/2;

        if (weights_[center].dst_ == to)
            return weights_[center].weight_;
        else if (weights_[center].dst_ > to)
            right = center - 1;
        else
            left = center + 1;
    }

    return kInvalidWeight;
}


///////////////////////////////////////////////////
struct Edge {
    Edge(int num_vertex)
        : num_vertex_(num_vertex)
    {
        graph_ = new WeightGraph();
    }

    ~Edge()
    {
        delete graph_;
    }

    bool isConnected(int from, int to) const;
    vector<int> getConnectedVertices(int v) const;
    inline int getWeight(int from, int to) const;
    void setWeight(int from, vector<int> &tos, vector<int> &weights);
    void printWeight() const;

    int num_vertex_;

    WeightGraph *graph_;
};


bool Edge::isConnected(int from, int to) const
{
    if (from == to)
        return false;

    if (getWeight(from, to) == kInvalidWeight)
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


void Edge::setWeight(int from, vector<int> &tos, vector<int> &weights)
{
    assert(0 <= from && from < num_vertex_);
    for (const int t : tos)       { assert(0 <= t && t < kMaxVertex && t < num_vertex_); (void)t; }
    for (const int w : weights)   { assert(0 <= w && w <= kMaxWeight); (void)w; }

    graph_->setWeight(from, tos, weights);
}


inline int Edge::getWeight(int from, int to) const
{
    return graph_->getWeight(from, to);
}


void Edge::printWeight() const
{
    for (int i = 0; i < num_vertex_; i++)
    {
        DPRINTF("%d:", i);

        for (int j = 0; j < num_vertex_; j++)
        {
            const int w = graph_->getWeight(i, j);
            if (w != kInvalidWeight)        printf(" %d", w);
            else                            printf(" -");
        }
        cout << endl;

    }
}


//////////////////////////////////////////////////////////////
struct VtxAndWeight {
    VtxAndWeight(int vtx, int weight)
        : vtx_(vtx)
        , weight_(weight)
    {}

    int vtx_;
    int weight_;

    bool operator>(const VtxAndWeight &rhs)
    {
        return (*this).weight_ > rhs.weight_;
    };

    bool operator<(const VtxAndWeight &rhs)
    {
        return (*this).weight_ < rhs.weight_;
    };
};


struct LessThanByWeight
{
    bool operator()(const VtxAndWeight& lhs, const VtxAndWeight& rhs) const
    {
        return lhs.weight_ > rhs.weight_;
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

        if (weight > g_min_weight[vtx])
            continue;

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

    assert(kMaxVertex >= g_num_of_vertex);

    // init
    for (int i = 0; i < g_num_of_vertex; i++)
    {
        g_min_weight[i] = INT_MAX;
    }

    Edge edge(g_num_of_vertex);

    // input
    for (int i = 0; i < g_num_of_vertex; i++)
    {
        int from, degree;
        cin >> from >> degree;

        vector<int> tos, weights;

        for (int j = 0; j < degree; j++)
        {
            int to, weight;
            cin >> to >> weight;

            tos.push_back(to);
            weights.push_back(weight);

        }
        cin.ignore();

        edge.setWeight(from, tos, weights);
    }

#if MYDEBUG
    edge.printWeight();
#endif // MYDEBUG

    // process
    const int start = 0;
    calcShortestPath(start, edge);

    // output
    for (int i = 0; i < g_num_of_vertex; i++)
        printf("%d %d\n", i, g_min_weight[i]);

    return 0;
}

