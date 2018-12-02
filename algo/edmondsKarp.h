#ifndef _EDMONDSKARP_
#define _EDMONDSKARP_

#include "../genetic.h"

class EdmondsKarp {
private:
	vector< vector<Edge> > graph;
	vector<int> pred;
	int V;
	int edgeCount;
public:
	EdmondsKarp(Indiv *, int);
	bool bfs(int, int);
	int dfs(int, int);
	long long match(int, int);
};
#endif