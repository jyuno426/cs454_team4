#include "graph.h"
using namespace std;

Edge::Edge() {}
Edge::Edge(Edge *e) : s(e->s), t(e->t), c(e->c) {}
Edge::Edge(int s, int t, int c) : s(s), t(t), c(c) {}
