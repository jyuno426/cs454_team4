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
	Indiv(Indiv *);
	~Indiv();

	vector<Edge *> gene;
	long long fitness;
};

enum SolverType { DINIC, EC, FF };
enum GraphType { ANY, AC };
enum CrossoverType { SPC, TPCS };

const int populationSize = 100;

class Generation {
public:
	Generation();
	Generation(int, int, int, SolverType, GraphType, CrossoverType);
	Generation(char *filename);
	~Generation();

	void randomCreation();
	Indiv *reproduct();
	void crossover(Indiv *, Indiv *);
	void mutation(Indiv *);
	void evaluate(Indiv *);

	void sort();
	long long max_fitness();
	Edge *randomEdge();

	Indiv *sizeManipulation(Indiv *, int, int);

	SolverType S;
	GraphType G;
	CrossoverType CO;

	int V, E, C;
	vector<Indiv *> population;
};

#endif
