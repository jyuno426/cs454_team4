#ifndef _FORDFULKERSON_
#define _FORDFULKERSON_

#include "../utils.h"
#include "../genetic.h"

class FordFulkerson : public Algo {
private:
	vector<vector<Edge>> graph;
	vector<int> pred;
	int V;
	int edgeCount;
    int dfs(int x, int sink, int f);
    bool initialized;
public:
    FordFulkerson();
    void init(Indiv *indiv, int _V);
	long long match(int src, int sink);
};
#endif
