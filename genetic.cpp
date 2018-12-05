#include "genetic.h"
#include "utils.h"
#include "graph.h"
#include "algo/dinic.h"
#include "algo/edmondsKarp.h"
#include "algo/fordFulkerson.h"
#include <random>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdio>
using namespace std;

extern mt19937 gen;

int fitnessCount;
bool IndivCompare(Indiv *aa, Indiv *bb) {
	return aa->fitness < bb->fitness;
}

/* -------- TestType --------*/
TestType::TestType() {}
TestType::TestType(SolverType ST, GraphType GT, CrossoverType CT, int V, int E, int C) : ST(ST), GT(GT), CT(CT), V(V), E(E), C(C) {}

/* -------- Indiv --------*/
Indiv::Indiv() {}
Indiv::Indiv(int n) { if(n > 0) gene.resize(n); }
Indiv::Indiv(Indiv *indiv) {
	for(auto &e : indiv->gene)
		gene.push_back(new Edge(e));
	fitness = indiv->fitness;
}
Indiv::~Indiv() { for(auto &e : gene) delete e; }

/* -------- Generation --------*/
Generation::Generation() {}
Generation::Generation(const TestType &TT) : TT(TT) {}
Generation::~Generation() {
	for(auto &i : population) delete i;
}

void Generation::randomCreation() {
	for(int i = 0; i < populationSize; i++) {
		Indiv *indiv = new Indiv(TT.E);
		for(int j = 0; j < TT.E; j++)
			indiv->gene[j] = randomEdge();
		evaluate(indiv);
		population.push_back(indiv);
	}
}

Indiv *Generation::reproduct() {
	auto reprod = [&](Indiv *aa, Indiv *bb) {
		if(aa->fitness > bb->fitness)
			return random_real(0, 10) < 9 ? aa : bb;
		else
			return random_real(0, 10) < 9 ? bb : aa;
	};

	vector<Indiv *> candidates;
	while(candidates.size() < 8)
		candidates.push_back(population[random_int(0, populationSize - 1)]);
	while(candidates.size() > 1) {
		assert(candidates.size() % 2 == 0);
		vector<Indiv *> tmp = candidates;
		candidates.clear();
		for(size_t j = 0; j < tmp.size(); j += 2)
			candidates.push_back(reprod(tmp[j], tmp[j + 1]));
	}

	return new Indiv(candidates[0]);
}

void Generation::crossover(Indiv *indiv1, Indiv *indiv2) {
	if(TT.CT == SPC) {
		for(int i = random_int(0, TT.E - 1); i < TT.E; i++)
			swap(indiv1->gene[i], indiv2->gene[i]);
	}
	if(TT.CT == TPCS) {

	}
}

void Generation::mutation(Indiv *indiv) {
	for(int i = 0; i < TT.E; i++) {
		if(random_real(0, TT.E) < 1) {
			delete indiv->gene[i];
			indiv->gene[i] = randomEdge();
		}
	}
	evaluate(indiv);
}

void Generation::evaluate(Indiv *a) {
	fitnessCount++;

	long long res = 0;

	if(TT.ST == DINIC) {
		Dinic d;
		d.init(a->gene, TT.V);
		res = d.match(0, TT.V - 1);
	}

	if(TT.ST == EC) {
		EdmondsKarp d;
		d.init(a->gene, TT.V);
		res = d.match(0, TT.V - 1);
	}

	if(TT.ST == FF) {
		FordFulkerson d;
		d.init(a->gene, TT.V);
		res = d.match(0, TT.V - 1);
	}

	a->fitness = res;
}

void Generation::sort() {
	std::sort(population.begin(), population.end(), IndivCompare);
}

long long Generation::max_fitness() {
	return (**max_element(population.begin(), population.end(), IndivCompare)).fitness;
}

// construct a random edge according to graph type.
Edge *Generation::randomEdge() {
	int s = random_int(0, TT.V - 1);
	int t = random_int(0, TT.V - 1);
	if(TT.GT == AC && s > t) swap(s, t);
	return new Edge(s, t, random_int(1, TT.C));
}

Indiv *Generation::sizeManipulation(Indiv *indiv, int V_change, int E_change) {
	/* Logic
		Original graph size is (V, E).
		V_change and E_change can be negative.

		1. Fit vertex size into V + V_change.
			1-1. When increasing vertex size,
					randomly select |V_change| distinct vertices from [0, V-1] and split it into two vertices.
			1-2. When splitting each vertex i,
					- newly added vertex becomes i+1 and the following vertices become i+2, i+3, ... , V.
					- all incoming edges (for original i) attach to i / all outgoing edges attach to i+1.
					- add one random edge from i to i+1.
			1-3. When decreasing vertex size,
					- randomly select |V_change| vertices from [0, V-2].
					- If we select vertex i, merge it with i+1 into one vertex.
		2. Fit edge size into E + E_change.
			2-1. When increasing edge size,
					add random edges into a random position in the gene.
			2-2. When decreasing edge size,
					delete randomly selected edges.
	*/

	assert(-(TT.V - 1) < V_change && V_change < TT.V);
	assert(TT.V + V_change >= 1);
	assert(TT.E + E_change >= 0);

	int i;

	vector<Edge *> edges_kept, edges_added;
	Indiv *res = new Indiv();

	/* ------------ 1. Fit vertex size ----------- */
	auto splitted = random_distinct_int(0, TT.V - 1, V_change);
	auto merged = random_distinct_int(0, TT.V - 2, -V_change);
	vector<int> state(TT.V, 0);		// -1: merged, 0: none, 1: splitted
	vector<int> index_change(TT.V, 0);	// index change table of "old" vertices

	for(i = 0; i < V_change; i++)
		state[splitted[i]] = 1;
	for(i = 0; i < -V_change; i++)
		state[merged[i]] = -1;

	int dist;
	for(i = dist = 0; i < TT.V; dist += state[i++])
		index_change[i] = i + dist;

	for(i = 0; i < TT.E; i++) {
		Edge *edge = indiv->gene[i];
		int s = index_change[edge->s];
		int t = index_change[edge->t];
		// edge->s is splitted and the edge is outgoing.
		if(state[edge->s] == 1) s++;
		edges_kept.push_back(new Edge(s, t, edge->c));
	}

	for(auto &p : splitted) {
		int v = index_change[p];
		edges_added.push_back(new Edge(v, v + 1, random_int(1, TT.C)));
	}
	/* ------------------------------------------- */

	/* ------------- 2. Fit Edge size ------------ */
	// edges_added.size() can be larger than E_change
	while((int)edges_added.size() < E_change)
		edges_added.push_back(randomEdge());
	shuffle(edges_added.begin(), edges_added.end(), gen);

	int A = (int)edges_added.size();		// # of added edges
	int D = (TT.E + A) - (TT.E + E_change);	// # of deleted edges

	state.resize(TT.E + A);
	fill(state.begin(), state.end(), 0);
	// position of added edges
	for(auto &p : random_distinct_int(0, TT.E + A - 1, A)) state[p] += 1;
	// position of deleted edges
	for(auto &p : random_distinct_int(0, TT.E + A - 1, D)) state[p] += 2;
	// 00: kept / 01: added / 10: kept and deleted / 11: added and deleted

	auto it_added = edges_added.begin();
	auto it_kept = edges_kept.begin();
	for(i = 0; i < TT.E + A; i++) {
		if(state[i] == 0)
			res->gene.push_back(*(it_kept++));
		else if(state[i] == 1)
			res->gene.push_back(*(it_added++));
		else if(state[i] == 2)
			delete *(it_kept++);
		else
			delete *(it_added++);
	}
	/* ------------------------------------------- */

	assert((int)res->gene.size() == TT.E + E_change);

	return res;
}

void Generation::load(const char *path) {
	FILE *file = fopen(path, "r");

	int t[6], fscanfRes;
	for (int i = 0; i < 6; i++) {
		fscanfRes = fscanf(file, "%d", t + i);
		assert(fscanfRes == 1);
	}
	TT = { (SolverType)t[0], (GraphType)t[1], (CrossoverType)t[2], t[3], t[4], t[5] };

	for(int i = 0; i < populationSize; i++) {
		Indiv *indiv = new Indiv();
		fscanfRes = fscanf(file, "%lld\n", &indiv->fitness);
		assert(fscanfRes == 1);
		for(int j = 0; j < TT.E; j++) {
			Edge *edge = new Edge();
			fscanfRes = fscanf(file, "%d %d %d\n", &edge->s, &edge->t, &edge->c);
			assert(fscanfRes == 3);
			indiv->gene.push_back(edge);
		}
		population.push_back(indiv);
	}

	fclose(file);
}

void Generation::dump(const char *path) {
	FILE *file = fopen(path, "w");

	fprintf(file, "%d %d %d %d %d %d\n", TT.ST, TT.GT, TT.CT, TT.V, TT.E, TT.C);

	for(auto *indiv : population) {
		fprintf(file, "%lld\n", indiv->fitness);
		for(auto *edge : indiv->gene)
			fprintf(file, "%d %d %d\n", edge->s, edge->t, edge->c);
	}

	fclose(file);
}
