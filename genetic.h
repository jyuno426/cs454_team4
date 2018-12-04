#ifndef _GENETIC_
#define _GENETIC_

#include "utils.h"

const int populationSize = 100;

enum SolverType { DINIC, EC, FF };
enum GraphType { ANY, AC };
enum CrossoverType { SPC, TPCS };

struct TestType {
	SolverType ST;
	GraphType GT;
	CrossoverType CT;
	int V, E, C;

	TestType(SolverType ST, GraphType GT, CrossoverType CT, int V, int E, int C);
};

class Edge {
public:
	Edge();
	Edge(Edge *e);
	Edge(int s, int t, int c);

	int s, t, c;
};

class Indiv {
public:
	Indiv();
	Indiv(int n);
	Indiv(Indiv *indiv);
	~Indiv();

	vector<Edge *> gene;
	long long fitness;
};


class Generation {
public:
	Generation(TestType TT);
	~Generation();

	void randomCreation();
	Indiv *reproduct();
	void crossover(Indiv *indiv1, Indiv *indiv2);
	void mutation(Indiv *indiv);
	void evaluate(Indiv *indiv);

	void sort();
	long long max_fitness();
	Edge *randomEdge();

	Indiv *sizeManipulation(Indiv *indiv, int V_change, int E_change);

	TestType TT;
	vector<Indiv *> population;
};

#endif
