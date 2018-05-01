#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>

#include <vector>
#include <string>
using namespace std;

#define DEBUG (0)

#if DEBUG
#define DPRINTF(...) do { \
    printf("D(L%d) %s: ", __LINE__, __func__); \
    printf(__VA_ARGS__); \
  } while (false)
#else
#define DPRINTF(...)
#endif // DEBUG

enum e_state {
    UNVISITED = 0,
    CANDITATE,
    VISITED,
};

const int kInvalid = -1;
const uint8_t kMaxN = 100;
int g_numOfVertices;
int g_weightOfEdge[kMaxN][kMaxN];
int g_minWeight[kMaxN];
e_state g_visited[kMaxN];


void printAllWeights()
{
    DPRINTF("Number of vertices %d\n", g_numOfVertices);

    for (int i = 0; i < g_numOfVertices; i++)
    {
        for (int j = 0; j < g_numOfVertices; j++)
        {
            if (j == 0)  DPRINTF("");
            printf(" %d", g_weightOfEdge[i][j]);
        }
        cout << endl;
    }
}


void printAllPath()
{
    DPRINTF("Number of vertices %d\n", g_numOfVertices);

    for (int i = 0; i < g_numOfVertices; i++)
    {
        for (int j = 0; j < g_numOfVertices; j++)
        {
            if (g_weightOfEdge[i][j] > 0)
                printf("%d -> %d (%d)\n", i, j, g_weightOfEdge[i][j]);
        }
    }
}


bool isConnected(int a, int b)
{
    if (g_weightOfEdge[a][b] == kInvalid)   return false;
    else                                    return true;
}


void updateStatus(int v)
{
    g_visited[v] = VISITED;

    // mark neighthoods to CANDITATE
    for (int i = 0; i < g_numOfVertices; i++)
    {
        if (isConnected(v, i) && g_visited[i] == UNVISITED)
            g_visited[i] = CANDITATE;
    }

#if (1) & DEBUG
    for (int i = 0; i < g_numOfVertices; i++)
    {
        DPRINTF("[%d] ", i);
        switch (g_visited[i]) {
            case UNVISITED: cout << "UNVISITED" << endl; break;
            case CANDITATE: cout << "CANDITATE" << endl; break;
            case VISITED: cout << "VISITED" << endl; break;
            default: assert(false); break;
        }
    }
#endif
}


void updateMinWeight()
{
    for (int i = 0; i < g_numOfVertices; i++)
    {
        if (g_visited[i] == VISITED)
        {
            for (int j = 0; j < g_numOfVertices; j++)
            {
                if (isConnected(i, j) && g_visited[j] == CANDITATE)
                {
                    if (g_minWeight[j] > g_weightOfEdge[i][j])
                    {
                        g_minWeight[j] = g_weightOfEdge[i][j];
                    }
                }
            }
        }
    }

#if (1) & DEBUG
    for (int i = 0; i < g_numOfVertices; i++)
    {
        if (g_minWeight[i] < INT_MAX)
            DPRINTF("[%d] %d\n", i, g_minWeight[i]);
    }
#endif
}


int takeUnvisitedVertex()
{
    for (int i = 0; i < g_numOfVertices; i++)
        if (g_visited[i] == CANDITATE)
            return i;

    // make sure g_visited[i] has one CANDITATE element at least
    assert(false);
    return -1;
}


bool haveUnvisitedVertex()
{
    for (int i = 0; i < g_numOfVertices; i++)
        if (g_visited[i] != VISITED)
            return true;

    return false;
}


int minSpanningTree()
{
    int sumOfWeight = 0;
    const int entryPoint = 0;

    updateStatus(entryPoint);

    while (haveUnvisitedVertex())
    {
        // update weight on edges that is neighborhood
        updateMinWeight();

        int minVertex, minWeight = INT_MAX;
    
        // take minimum edge, mark it as visited
        for (int i = 0; i < g_numOfVertices; i++)
        {
            if (g_visited[i] == CANDITATE && g_minWeight[i] < minWeight)
            {
                minVertex = i;
                minWeight = g_minWeight[i];
            }
        }

        assert(minWeight != INT_MAX);

        sumOfWeight += minWeight;
        updateStatus(minVertex);
    }

    return sumOfWeight;
}


int main()
{
    cin >> g_numOfVertices; cin.ignore();

    DPRINTF("Number of vertices %d\n", g_numOfVertices);

    for (int i = 0; i < g_numOfVertices; i++)
    {
        for (int j = 0; j < g_numOfVertices; j++)
            cin >> g_weightOfEdge[i][j];

        cin.ignore();
    }

    // init
    for (int i = 0; i < g_numOfVertices; i++)
    {
        g_minWeight[i] = INT_MAX;
        g_visited[i] = UNVISITED;
    }

#if DEBUG
    DPRINTF("Number of vertices %d\n", g_numOfVertices);
    printAllWeights();
    //printAllPath();
#endif // DEBUG


    cout << minSpanningTree() << endl;

    return 0;
}

// todo: check the reference code
