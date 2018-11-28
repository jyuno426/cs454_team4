#include "edmondsKarp.h"

/* -------- EdmondsKarp -------- */
EdmondsKarp::EdmondsKarp(Indiv *indiv, int _V) {
	V = _V;
	graph.resize(V);
	pred.resize(V);
	for(auto &e: indiv->gene){
		graph[e->s].push_back({(int)graph[e->t].size(), e->t, e->c});
		graph[e->t].push_back({(int)graph[e->s].size() - 1, e->s, 0});
	}
}

bool EdmondsKarp::bfs(int src, int sink) {
	for(int i=0;i<V;i++){
		pred[i] = -1;
	}
	queue<int> q;
	q.push(src);
	pred[src] = 0;
	while(!q.empty()){
		int s = q.front(); q.pop();
		for(auto &e: graph[s]){
			//edgeCount++;
			if(e.c > 0 && pred[e.t] == -1){
				pred[e.t] = e.s;
				q.push(e.t);
			}
		}
	}
	pred[src] = -1;
	return ~pred[sink];
}

int EdmondsKarp::dfs(int src, int sink) {
	int f = 2e9;
	int x = sink;
	while(x != src){
		//edgeCount++;
		Edge &e = graph[x][pred[x]];
		f = min(f, graph[e.t][e.s].c);
		x = e.t;
	}
	x = sink;
	while(x != src){
		//edgeCount++;
		Edge &e = graph[x][pred[x]];
		graph[e.t][e.s].c -= f;
		graph[x][pred[x]].c += f;
		x = e.t;
	}
	return f;
}

long long EdmondsKarp::match(int src, int sink) {
	edgeCount = 0;
	long long totalFlow = 0;
	while(bfs(src, sink)){
		totalFlow += dfs(src, sink);		
	}
	return totalFlow;
}