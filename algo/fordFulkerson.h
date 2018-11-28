#ifndef _FORDFULKERSON_
#define _FORDFULKERSON_

#include "../genetic.h"

class FordFulkerson {
private:
	vector< vector<Edge> > graph;
	vector<int> pred;
	int V;
	int edgeCount;
public:
	FordFulkerson(Indiv *, int);
	int dfs(int, int, int);
	long long match(int, int);
};
#endif