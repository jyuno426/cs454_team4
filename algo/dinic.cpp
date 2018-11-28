#include "dinic.h"

/*
#include <bits/stdc++.h>
using namespace std;

struct Edge{
	int s, t, c;
	Edge(int x, int y, int z){
		s = x; t = y; c = z;
	}
};

class Indiv{
public:
	vector<Edge *> gene;
	Indiv() { gene.clear(); }
};

class Dinic {
private:
	vector< vector<Edge> > graph;
	vector< int > dist;
	vector< int > pnt;
	int V;
	int edgeCount;
public:
	Dinic(Indiv *, int);
	bool bfs(int, int);
	int dfs(int, int, int);
	long long match(int, int);
};
*/

/* -------- Dinic --------*/


Dinic::Dinic(Indiv *indiv, int _V) {
	V = _V;
	graph.resize(V);
	dist.resize(V);
	pnt.resize(V);
	for(auto &e: indiv->gene){
		graph[e->s].push_back({(int)graph[e->t].size(), e->t, e->c});
		graph[e->t].push_back({(int)graph[e->s].size() - 1, e->s, 0});
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
	for(; pnt[x] < graph[x].size(); pnt[x]++) {
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

/*
int main(){
	int tt;
	scanf("%d", &tt);
	while(tt--){
		int n, m;
		scanf("%d%d", &n, &m);
		Indiv indiv = Indiv();
		for(int i=0;i<m;i++){
			int x, y;
			scanf("%d%d", &x, &y);
			x++; y++;
			indiv.gene.push_back(new Edge(x, y+n, 1));
		}
		for(int i=1;i<=n;i++){
			indiv.gene.push_back(new Edge(0, i, 1));
			indiv.gene.push_back(new Edge(i+n, n+n+1, 1));
		}
		Dinic dn(&indiv, 2*n+2);
		long long ret = dn.match(0, 2*n+1);
		printf("%lld\n", ret);
	}
}
*/