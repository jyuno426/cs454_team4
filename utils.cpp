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
