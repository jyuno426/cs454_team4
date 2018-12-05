#ifndef _DINIC_
#define _DINIC_

#include "../utils.h"
#include "../genetic.h"

class Dinic : public Algo {
private:
	vector<vector<Edge>> graph;
	vector<int> dist;
	vector<int> pnt;
	int V;
	int edgeCount;
    bool bfs(int src, int sink);
    int dfs(int x, int sink, int f);
    bool initialized;
public:
    Dinic();
    void init(Indiv *indiv, int _V);
	long long match(int src, int sink);
};
#endif
