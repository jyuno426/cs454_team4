#include "dinic.h"

/* -------- Dinic --------*/
Dinic::Dinic(Indiv *indiv, int _V) {
	V = _V;
	graph.resize(V);
	dist.resize(V);
	pnt.resize(V);

	vector<Edge> edges;
	for(auto &e: indiv->gene)
		edges.push_back({ e->s, e->t, e->c });

	sort(edges.begin(), edges.end(), [&](const Edge a, const Edge b){
		return a.s != b.s ? a.s < b.s : a.t < b.t;
	});

	int p = 0;
	for(int i = 1; i < (int)edges.size(); i++){
		if(edges[i].s == edges[p].s && edges[i].t == edges[p].t){
			edges[p].c += edges[i].c;
		}
		else{
			p++;
			edges[p] = edges[i];
		}
	}

	edges.resize(p + 1);

	for(auto &e: edges){
		graph[e.s].push_back({(int)graph[e.t].size(), e.t, e.c});
		graph[e.t].push_back({(int)graph[e.s].size() - 1, e.s, 0});
	}
}

bool Dinic::bfs(int src, int sink) {
	for(int i=0;i<V;i++){
		dist[i] = pnt[i] = 0;
	}
	queue<int> q;
	q.push(src);
	dist[src] = 1;
	while(!q.empty()){
		int s = q.front(); q.pop();
		for(auto &e: graph[s]){
			//edgeCount++;
			if(e.c > 0 && !dist[e.t]){
				dist[e.t] = dist[s] + 1;
				q.push(e.t);
			}
		}
	}
	return dist[sink] > 0;
}

int Dinic::dfs(int x, int sink, int f) {
	if(x == sink) return f;
	for(; pnt[x] < (int)graph[x].size(); pnt[x]++) {
		//edgeCount++;
		Edge e = graph[x][pnt[x]];
		if(e.c > 0 && dist[e.t] == dist[x] + 1) {
			int w = dfs(e.t, sink, min(f, e.c));
			if(w){
				//printf("%d %d\n", e.t, sink);
				graph[x][pnt[x]].c -= w;
				graph[e.t][e.s].c += w;
				return w;
			}
		}
	}
	return 0;
}

long long Dinic::match(int src, int sink) {
	edgeCount = 0;
	long long totalFlow = 0;
	while(bfs(src, sink)){
		int ret;
		while((ret = dfs(src, sink, 2e9))) totalFlow += ret;
	}
	return totalFlow;
}
