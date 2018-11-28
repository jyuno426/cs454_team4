#ifndef _SOLVER_
#define _SOLVER_

#include <bits/stdc++.h>
using namespace std;

class Solver{
public:
	virtual long long fitness(Indiv) = 0;
};

#endif