#ifndef _EDMONDSKARP_
#define _EDMONDSKARP_

#include "../utils.h"
#include "../genetic.h"

class EdmondsKarp : public Algo {
private:
	vector<vector<Edge>> graph;
	vector<int> pred;
	int V;
	int edgeCount;
    bool bfs(int src, int sink);
    int dfs(int src, int sink);
    bool initialized;
public:
    EdmondsKarp();
    void init(Indiv *indiv, int _V);
	long long match(int src, int sink);
};
#endif
