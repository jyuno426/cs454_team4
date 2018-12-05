#ifndef _GRAPH_
#define _GRAPH_

#include <vector>
using namespace std;

class Edge {
public:
    Edge();
    Edge(Edge *e);
    Edge(int s, int t, int c);

    int s, t, c;
};

class Algo {
public:
    virtual void init(vector<Edge *> &gene, int _V) = 0;
    virtual long long match(int src, int sink) = 0;
};

#endif /* _GRAPH_ */
