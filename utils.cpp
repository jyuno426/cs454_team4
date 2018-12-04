#include "utils.h"
#include <chrono>
using namespace std;

extern mt19937 gen;

/* initialization */
void util_init() {
	gen.seed(chrono::high_resolution_clock::now().time_since_epoch().count());
}

/* pick random real number in (s, e) uniformly */
double random_real(double s, double e) {
	uniform_real_distribution<double> dis(s, e);
	return dis(gen);
}

/* pick random integer in [s, e] uniformly */
int random_int(int s, int e) {
	uniform_int_distribution<int> dis(s, e);
	return dis(gen);
}

/* pick cnt distinct random integers in [s, e]*/
vector<int> random_distinct_int(int s, int e, int cnt) {
	int i, p;
	vector<int> fisher, res;
	for(i = s; i <= e; i++)
		fisher.push_back(i);
	for(i = s; cnt--; i++) {
		p = random_int(i, e);
		swap(fisher[i], fisher[p]);
		res.push_back(fisher[i]);
	}
	return res;
}
