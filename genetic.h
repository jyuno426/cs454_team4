#ifndef _GENETIC
#define _GENETIC

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
private:
	vector<Edge *> gene;
public:
	Indiv();
	Indiv(int);
	~Indiv();
	int size();
	void resize(int);
	Edge *&at(int);
};

class Genetic {
private:
	vector<Indiv *> population;
	void crossover(Indiv *, Indiv *, Indiv *, Indiv *);
	void mutation(Indiv *, Indiv *);
	int fitness(Indiv *);
public:
	Genetic();
	Genetic(int, int, int);

	int V, E, C;
};

#endif