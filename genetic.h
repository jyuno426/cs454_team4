#ifndef _GENETIC_
#define _GENETIC_

#include "graph.h"
#include <vector>
using namespace std;

const int populationSize = 100;

enum SolverType { DINIC, EC, FF };
enum GraphType { ANY, AC };
enum CrossoverType { SPC, TPCS };

class TestType {
public:
	TestType();
	TestType(SolverType ST, GraphType GT, CrossoverType CT, int V, int E, int C);

	SolverType ST;
	GraphType GT;
	CrossoverType CT;
	int V, E, C;
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
	Generation();
	Generation(const TestType &_TT);
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

	void load(const char *path);
	void dump(const char *path);
};

#endif /* _GENETIC_ */
