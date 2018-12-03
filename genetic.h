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
	Indiv(int n);
	~Indiv();
	
	vector<Edge *> gene;
};

enum SolverType { DINIC, EC, FF };
enum GraphType { ANY, AC };
enum CrossoverType { SPC, TPCS };
const int populationSize = 100;

class Generation {
public:
	Generation();
	~Generation();
	Generation(int, int, int, SolverType, GraphType, CrossoverType);

	vector<Indiv *> population;
	pair<Indiv *, Indiv *> crossover(Indiv *, Indiv *);
	Indiv *mutation(Indiv *);
	Indiv *sizeManipulation(Indiv *, int, int);
	long long fitness(Indiv *);

	SolverType S;
	GraphType G;
	CrossoverType CO;

	int V, E, C;
};

#endif