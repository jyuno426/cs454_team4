#include "fordFulkerson.h"

/* -------- FordFulkerson -------- */
FordFulkerson::FordFulkerson(Indiv *indiv, int _V) {
	V = _V;
	graph.resize(V);
	pred.resize(V);
	for(auto &e: indiv->gene){
		graph[e->s].push_back({(int)graph[e->t].size(), e->t, e->c});
		graph[e->t].push_back({(int)graph[e->s].size() - 1, e->s, 0});
	}
}

int FordFulkerson::dfs(int x, int sink, int f) {
	if(x == sink){
		return f;
	}
	for(auto &e: graph[x]){
		//edgeCount++;
		if(e.c > 0 && pred[e.t] == -1){
			pred[e.t] = e.s;
			int w = dfs(e.t, sink, min(f, e.c));
			if(w){
				e.c -= w;
				graph[e.t][e.s].c += w;	
				return w; 
			}
			pred[e.t] = -1;
		}
	}
	return 0;
}

long long FordFulkerson::match(int src, int sink) {
	edgeCount = 0;
	long long totalFlow = 0;
	while(1){
		for(int i=0;i<V;i++){
			pred[i] = -1;
		}
		pred[src] = 0;
		int ret = dfs(src, sink, 2e9);
		totalFlow += ret;
		if(!ret) break;
	}
	return totalFlow;
}