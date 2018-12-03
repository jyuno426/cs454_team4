#include "utils.h"
#include "genetic.h"

#include "algo/dinic.h"
#include "algo/edmondsKarp.h"
#include "algo/fordFulkerson.h"

/* ---- Helper Functions ---- */
// construct random edge according to graph type.
Edge *randomEdge(int V, int C, GraphType G) {
	int s = 0, t = 0, c = random_int(1, C);
	if (G == ANY) {
		while (s == t) {
			s = random_int(0, V - 1);
			t = random_int(0, V - 1);
		}
	}
	if (G == AC) {
		auto tmp = random_multiple_int(0, V - 1, 2);
		s = tmp[0], t = tmp[1];
	}
	return new Edge(s, t, c);
}
Edge *randomEdge(int s, int t, int V, int C, GraphType G) {
	int c = random_int(1, C);
	if (G == ANY) {
		if (random_int(0, 1)) swap(s, t);
	}
	if (G == AC) {
		if (s > t) swap(s, t);
	}
	return new Edge(s, t, c);
}

/* -------- Edge --------*/
Edge::Edge() {}
Edge::Edge(Edge *edge) {
	s = edge->s, t = edge->t, c = edge->c;
}
Edge::Edge(int _s, int _t, int _c) {
	s = _s, t = _t, c = _c;
}

/* -------- Indiv --------*/
Indiv::Indiv() {}
Indiv::Indiv(int n) { gene.resize(n); }
Indiv::~Indiv() {
	for (auto &edge : gene) delete edge;
}

/* -------- Generation --------*/
Generation::Generation() {}
Generation::~Generation() {
	for (auto &indiv : population) delete indiv;
}
Generation::Generation(int V, int E, int C, SolverType _S, GraphType _G, CrossoverType _CO) {
	S = _S, G = _G, CO = _CO;

	int i, j;

	for (i = 0; i < populationSize; i++) {
		Indiv *indiv = new Indiv();
		for (j = 0; j < E; j++)
			indiv->gene.push_back(randomEdge(V, C, G));
		population.push_back(indiv);
	}
}
pair<Indiv *, Indiv *> Generation::crossover(Indiv *indiv1, Indiv *indiv2) {
	Indiv *res1 = new Indiv(E);
	Indiv *res2 = new Indiv(E);

	// single-point crossover
	if (CO == SPC) {
		int i, point = random_int(0, E - 1);
		for (i = 0; i <= point; i++) {
			res1->gene[i] = new Edge(indiv1->gene[i]);
			res2->gene[i] = new Edge(indiv2->gene[i]);
		}
		for (; i < E; i++) {
			res1->gene[i] = new Edge(indiv2->gene[i]);
			res2->gene[i] = new Edge(indiv1->gene[i]);
		}
	}

	// two-point crossover with shift
	if (CO == TPCS) {

	}

	return { res1, res2 };
}
Indiv *Generation::mutation(Indiv *indiv) {
	Indiv *res = new Indiv(E);

	for (int i = 0; i < E; i++) {
		if (random_real(0, E) < 1)
			res->gene[i] = randomEdge(V, C, G);
		else
			res->gene[i] = new Edge(indiv->gene[i]);
	}

	return res;
}
Indiv *Generation::sizeManipulation(Indiv *indiv, int v_change, int e_change) {
	Indiv *res = new Indiv(E + e_change);

	if (v_change > 0 && e_change >= v_change) {
		Indiv old_edges(E), new_edges(e_change);

		auto splitted = random_multiple_int(0, V - 1, v_change);

		int i, j;
		vector<bool> is_splitted(V);
		vector<int> index_change(V);
		for (i = j = 0; i < V; i++) {
			index_change[i] = j;
			if (j < splitted.size() && i == splitted[j]) {
				is_splitted[i] = true;
				j++;
			}
			else
				is_splitted[j] = false;
		}

		for (i = 0; i < E; i++) {
			Edge *edge = indiv->gene[i];
			int s = edge->s, t = edge->t;
			s = s + index_change[s] + random_int(0, is_splitted[s]);
			t = t + index_change[t] + random_int(0, is_splitted[t]);
			old_edges.gene[i] = new Edge(s, t, edge->c);
		}

		for (i = 0; i < e_change; i++) {
			if (i < v_change) {
				int s = splitted[i], t = s + 1;
				new_edges.gene[i] = randomEdge(s, t, V, C, G);
			}
			else
				new_edges.gene[i] = randomEdge(V, C, G);
		}

		auto shuffled_order = random_shuffle_int(0, e_change - 1);
		auto new_location = random_multiple_int(0, E + v_change - 1, e_change);

		for (i = j = 0; i < E + e_change; i++) {
			if (i == new_location[j])
				res->gene[i] = new_edges.gene[shuffled_order[j++]];
			else
				res->gene[i] = old_edges.gene[i - j];
		}
	}
	
	return res;
}
long long Generation::fitness(Indiv *a) {
	if (S == DINIC) {
		Dinic d(a, V);
		return d.match(0, V - 1);
	}

	if (S == EC) {
		EdmondsKarp d(a, V);
		return d.match(0, V - 1);
	}

	if (S == FF) {
		FordFulkerson d(a, V);
		return d.match(0, V - 1);
	}

	return 0;
}