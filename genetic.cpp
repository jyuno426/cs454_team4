#include "utils.h"
#include "genetic.h"

#include "algo/dinic.h"
#include "algo/edmondsKarp.h"
#include "algo/fordFulkerson.h"

mt19937 gen;
int fitnessCount;
bool IndivCompare(Indiv *aa, Indiv *bb) {
	return aa->fitness < bb->fitness;
};

/* -------- Edge --------*/
Edge::Edge() {}
Edge::Edge(Edge *e) : s(e->s), t(e->t), c(e->c) {}
Edge::Edge(int _s, int _t, int _c) : s(_s), t(_t), c(_c) {}

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
Generation::Generation(int _V, int _E, int _C, SolverType _S, GraphType _G, CrossoverType _CO) :
	V(_V), E(_E), C(_C), S(_S), G(_G), CO(_CO) {}
Generation::Generation(char *filename) {
	// read generation from file
}
Generation::~Generation() {
	for(auto &i : population) delete i;
}

void Generation::randomCreation() {
	for(int i = 0; i < populationSize; i++) {
		Indiv *indiv = new Indiv(E);
		for(int j = 0; j < E; j++)
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
		vector<Indiv *> tmp = candidates;
		candidates.clear();
		for(size_t j = 0; j < tmp.size(); j += 2)
			candidates.push_back(reprod(tmp[j], tmp[j + 1]));
	}

	return new Indiv(candidates[0]);
}

void Generation::crossover(Indiv *indiv1, Indiv *indiv2) {
	if(CO == SPC) {
		for(int i = random_int(0, E - 1); i < E; i++)
			swap(indiv1->gene[i], indiv2->gene[i]);
	}
	if(CO == TPCS) {

	}
}

void Generation::mutation(Indiv *indiv) {
	for(int i = 0; i < E; i++) {
		if(random_real(0, E) < 1) {
			delete indiv->gene[i];
			indiv->gene[i] = randomEdge();
		}
	}
	evaluate(indiv);
}

void Generation::evaluate(Indiv *a) {
	fitnessCount++;

	long long res = 0;

	if(S == DINIC) {
		Dinic d(a, V);
		res = d.match(0, V - 1);
	}

	if(S == EC) {
		EdmondsKarp d(a, V);
		res = d.match(0, V - 1);
	}

	if(S == FF) {
		FordFulkerson d(a, V);
		res = d.match(0, V - 1);
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
	int s = random_int(0, V - 1);
	int t = random_int(0, V - 1);
	if(G == AC && s > t) swap(s, t);
	return new Edge(s, t, random_int(1, C));
}

Indiv *Generation::sizeManipulation(Indiv *indiv, int V_change, int E_change) {
	/* Logic
		Original graph size is (V, E).
		V_change and E_change can be negative.

		1. Fit vertex size into V + V_change.
			1-1. When increasing vertex size,
					randomly select |V_change| vertices from [0, V-1] and split it into two vertices.
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

	int i;

	vector<Edge *> edges_kept, edges_added;
	Indiv *res = new Indiv();

	/* ------------ 1. Fit vertex size ----------- */
	auto splitted = random_distinct_int(0, V - 1, V_change);
	auto merged = random_distinct_int(0, V - 2, -V_change);
	vector<int> state(V, 0);		// -1: merged, 0: none, 1: splitted
	vector<int> index_change(V, 0);	// index change table of "old" vertices

	for(i = 0; i < V_change; i++)
		state[splitted[i]] = 1;
	for(i = 0; i < -V_change; i++)
		state[merged[i]] = -1;

	int dist;
	for(i = dist = 0; i < V; dist += state[i++])
		index_change[i] = i + dist;

	for(i = 0; i < E; i++) {
		Edge *edge = indiv->gene[i];
		int s = index_change[edge->s];
		int t = index_change[edge->t];
		// edge->s is splitted and the edge is outgoing.
		if(state[edge->s] == 1) s++;
		edges_kept[i] = new Edge(s, t, edge->c);
	}

	for(auto &p : splitted) {
		int v = index_change[p];
		edges_added.push_back(new Edge(v, v + 1, random_int(1, C)));
	}
	/* ------------------------------------------- */

	/* ------------- 2. Fit Edge size ------------ */
	// edges_added.size() can be larger than E_change
	while((int)edges_added.size() < E_change)
		edges_added.push_back(randomEdge());
	shuffle(edges_added.begin(), edges_added.end(), gen);

	int A = edges_added.size();			// # of added edges
	int D = (E + A) - (E + E_change);	// # of deleted edges

	state.resize(E + A);
	fill(state.begin(), state.end(), 0);
	// position of added edges
	for(auto &p : random_distinct_int(0, E + A - 1, A)) state[p] |= 1;
	// position of deleted edges
	for(auto &p : random_distinct_int(0, E + A - 1, D)) state[p] |= 2;
	// 00: kept / 01: added / 10: kept and deleted / 11: added and deleted

	auto it_added = edges_added.begin();
	auto it_kept = edges_kept.begin();
	for(i = 0; i < E + A; i++) {
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

	return res;
}
