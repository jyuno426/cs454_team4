#include "utils.h"
using namespace std;

mt19937 gen;

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
	assert(cnt <= e - s + 1);
	vector<int> fisher, res;
	for(int i = s; i <= e; i++)
		fisher.push_back(i);
	for(int i = 0; cnt--; i++) {
		int p = random_int(i, e - s);
		swap(fisher[i], fisher[p]);
		res.push_back(fisher[i]);
	}
	return res;
}
