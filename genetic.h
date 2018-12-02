#ifndef _GENETIC_
#define _GENETIC_

#include <bits/stdc++.h>
using namespace std;

class Edge {
public:
	Edge();
	Edge(Edge *);
	Edge(int, int, int);

	int s, t, c;
};

class Indiv {
public:
	Indiv();
	Indiv(int);
	~Indiv();
	int size();
	void resize(int);
	Edge *&at(int);
	
	vector<Edge *> gene;
	int V, E, C;
};

enum SolverType { DINIC, EC, FF };

class Genetic {
private:
	vector<Indiv *> population;
	void crossover(Indiv *, Indiv *, Indiv *, Indiv *);
	void mutation(Indiv *, Indiv *);
	int fitness(Indiv *);
public:
	Genetic();
	Genetic(int, int, int);

	SolverType S;
	int V, E, C;
};

#endif