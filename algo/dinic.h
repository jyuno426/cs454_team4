#ifndef _DINIC_
#define _DINIC_

#include "../utils.h"
#include "../genetic.h"

class Dinic {
private:
	vector<vector<Edge>> graph;
	vector<int> dist;
	vector<int> pnt;
	int V;
	int edgeCount;
public:
	Dinic(Indiv *, int);
	bool bfs(int, int);
	int dfs(int, int, int);
	long long match(int, int);
};
#endif
