#include "utils.h"
#include <random>
#include <chrono>
using namespace std;

static mt19937 gen;

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

/* pick cnt distinct random integers in [s, e] in increasing order */
vector<int> random_multiple_int(int s, int e, int cnt) {
	vector<int> res;
	for (int flag = s - 1; cnt--;) {
		flag = random_int(flag + 1, e - cnt);
		res.push_back(flag);
	}
	return res;
}

/* shuffle [s, e] and return it */
vector<int> random_shuffle_int(int s, int e) {
	vector<int> res;
	for (int i = s; i <= e; i++)
		res.push_back(i);
	shuffle(res.begin(), res.end(), gen);
	return res;
}