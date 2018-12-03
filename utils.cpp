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

/* pick random integer in [0, m) uniformly */
int random_int(int m) {
    uniform_int_distribution<int> dis(0, m - 1);
	return dis(gen);
}
