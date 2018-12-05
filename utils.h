#ifndef _UTILS_
#define _UTILS_

#include <vector>
using namespace std;

void util_init();
double random_real(double s, double e);
int random_int(int s, int e);
vector<int> random_distinct_int(int s, int e, int cnt);

#endif /* _UTILS_ */
