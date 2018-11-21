#include "utils.h"
#include "genetic.h"


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
Indiv::Indiv(int n) {
	resize(n);
}
Indiv::~Indiv() {
	for (auto *edge : gene) delete edge;
}
int Indiv::size() {
	return gene.size();
}
void Indiv::resize(int n) {
	if (size() != n) gene.resize(n);
}
Edge *&Indiv::at(int x) {
	return gene.at(x);
}


/* -------- Genetic --------*/
Genetic::Genetic() {}
Genetic::Genetic(int _V, int _E, int _C) {
	V = _V, E = _E, C = _C;
}
void Genetic::crossover(Indiv *indiv1, Indiv *indiv2, Indiv *res1, Indiv *res2) {
	assert(indiv1->size() > 0);
	assert(indiv1->size() == indiv2->size());

	int i, n = indiv1->size();
	res1->resize(n), res2->resize(n);

	for (i = 0; i <= random_int(n - 1); i++) {
		res1->at(i) = new Edge(indiv1->at(i));
		res2->at(i) = new Edge(indiv2->at(i));
	}

	for (; i < n; i++) {
		res1->at(i) = new Edge(indiv2->at(i));
		res2->at(i) = new Edge(indiv1->at(i));
	}
}
void Genetic::mutation(Indiv *indiv, Indiv *res) {

}
int Genetic::fitness(Indiv *a) {
	return 0;
}