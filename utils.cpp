#include "utils.h"
#include <time.h>
using namespace std;

/* pick random real number in (s, e) uniformly */
double random_real(double s, double e) {
	static mt19937 gen((unsigned int)time(NULL));
	static uniform_real_distribution<double> dis(0, 1);
	return s + dis(gen) * (e - s);
}

/* pick random integer in [0, m) uniformly */
int random_int(int m) {
	return (int)(random_real(0, (double)m));
}