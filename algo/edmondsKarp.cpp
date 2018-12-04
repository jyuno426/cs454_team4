#include "edmondsKarp.h"

/* -------- EdmondsKarp -------- */
EdmondsKarp::EdmondsKarp(Indiv *indiv, int _V) {
	V = _V;
	graph.resize(V);
	pred.resize(V);

	if(indiv->gene.empty())
		return;

	vector<Edge> edges;
	for(auto &e : indiv->gene)
		edges.push_back({ e->s, e->t, e->c });

	sort(edges.begin(), edges.end(), [&](const Edge a, const Edge b) {
		return a.s != b.s ? a.s < b.s : a.t < b.t;
	});

	int p = 0;
	for(int i = 1; i < (int)edges.size(); i++) {
		if(edges[i].s == edges[p].s && edges[i].t == edges[p].t) {
			edges[p].c += edges[i].c;
		}
		else {
			p++;
			edges[p] = edges[i];
		}
	}

	edges.resize(p + 1);

	for(auto &e : edges) {
		graph[e.s].push_back({ (int)graph[e.t].size(), e.t, e.c });
		graph[e.t].push_back({ (int)graph[e.s].size() - 1, e.s, 0 });
	}
}

bool EdmondsKarp::bfs(int src, int sink) {
	for(int i = 0; i < V; i++) {
		pred[i] = -1;
	}
	queue<int> q;
	q.push(src);
	pred[src] = 0;
	while(!q.empty()) {
		int s = q.front(); q.pop();
		for(auto &e : graph[s]) {
			//edgeCount++;
			if(e.c > 0 && pred[e.t] == -1) {
				pred[e.t] = e.s;
				q.push(e.t);
			}
		}
	}
	pred[src] = -1;
	return ~pred[sink];
}

int EdmondsKarp::dfs(int src, int sink) {
	int f = (int)2e9;
	int x = sink;
	while(x != src) {
		//edgeCount++;
		Edge &e = graph[x][pred[x]];
		f = min(f, graph[e.t][e.s].c);
		x = e.t;
	}
	x = sink;
	while(x != src) {
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
	while(bfs(src, sink)) {
		totalFlow += dfs(src, sink);
	}
	return totalFlow;
}
