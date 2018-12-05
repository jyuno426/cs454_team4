#include "utils.h"
#include "genetic.h"
#include <vector>
#include <algorithm>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <cassert>
#include <cstring>
using namespace std;

extern int fitnessCount;

bool log_mode = true;
char log_path[100] = "res/default_log.csv";

void startLog(const char *path) {
	fitnessCount = 0;
	for (int i = 0; i < 100; i++)
		log_path[i] = '\0';
	strcpy(log_path, path);
	fclose(fopen(log_path, "w"));
}

void writeLog(long long value) {
	if(log_mode) {
		FILE *file = fopen(log_path, "a+");
		int fprintfRes = fprintf(file, "%lld\n", value);
		assert(fprintfRes > 0);
		fclose(file);
	}
}

Generation *GA(Generation *cur, int steps) {
	while(steps--) {
		Generation *next = new Generation(cur->TT);

		int i;
		// reproduction
		for(i = 0; i < populationSize; i++)
			next->population.push_back(cur->reproduct());

		// crossover
		for(i = 0; i < populationSize; i += 2)
			cur->crossover(next->population[i], next->population[i + 1]);

		// mutation
		for(i = 0; i < populationSize; i++)
			cur->mutation(next->population[i]);

		// survival
		cur->sort();
		next->sort();

		int elitist = max(1, populationSize / 10); // 10% from cur population
		int top = populationSize - elitist;
		for(i = 0; i < elitist; i++) {
			delete next->population[i];
			next->population[i] = new Indiv(cur->population[top + i]);
		}

		delete cur;
		cur = next;
		
		writeLog(cur->max_fitness());
		printf("fitness: %lld, %d step(s) left\n", cur->max_fitness(), steps);
	}

	cur->sort();
	return cur;
}

Generation *originalGA(const TestType TT, const int fitnessLimit) {
	Generation *cur = new Generation(TT);
	cur->randomCreation();

	return GA(cur, fitnessLimit / populationSize);
}

Generation *sizeFlexibleGA(Generation *from, const TestType TT_to, const int fitnessLimit) {
	// FL             : fitnessLimit
	// PS             : pupulationSize
	// PG             : possible generations = FL/PS
	// Steps          : V_to - V_from
	// SizeManip tries: (PG/Steps)/2
	// GA tries       : (PG/Steps)/2
	// formula: Steps * (SM_Tries + GA_Tries) <= PG = FL/PS

	const TestType TT_from = from->TT;
	TestType TT_cur(TT_from);
	Generation *cur = from;

	int i, j, k;
	const int steps = TT_to.V - TT_from.V;
	const int SM_Tries = fitnessLimit / (populationSize * steps * 2);
	const int GA_Tries = fitnessLimit / (populationSize * steps) - SM_Tries;
	
	assert(steps * (SM_Tries + GA_Tries) * populationSize <= fitnessLimit);
	
	for(i = 1; i <= steps; i++) {
		//printf("Step: %d/%d\n", i, steps);
		
		int V_change = 1;
		TT_cur.V += V_change;
		
		// Equalize proportion
		// i.e., (TT_cur.V * TT_cur.V / TT_cur.E) == (TT_to.V * TT_to.V / TT_to.E)
		int E_change = TT_cur.V * TT_cur.V * TT_to.E / (TT_to.V * TT_to.V) - TT_cur.E;
		
		TT_cur.E += E_change;
		
		//printf("V_cur: %d\n", TT_cur.V);
		//printf("E_cur: %d\n", TT_cur.E);

		Generation *next = new Generation(TT_cur);

		// sizeManipulation
		for(j = 0; j < populationSize; j++) {
			Indiv *winner = NULL, *challenger;
			for(k = 0; k < SM_Tries; k++) {
				if(!winner) {
					winner = cur->sizeManipulation(cur->population[j], V_change, E_change);
					next->evaluate(winner);
				}
				else{
					challenger = cur->sizeManipulation(cur->population[j], V_change, E_change);
					next->evaluate(challenger);

					if(winner->fitness > challenger->fitness)
						delete challenger;
					else {
						delete winner;
						winner = challenger;
					}
				}
			}
			next->population.push_back(winner);
		}
		
		// Since SM_Tries generation has passed, write log SM_Tries times.
		for(j = 0; j < SM_Tries; j++)
			writeLog(next->max_fitness());
		printf("after sizeManipulation: %lld\n", next->max_fitness());

		// GA over same size
		if(i == steps)
			next = GA(next, (fitnessLimit - fitnessCount) / populationSize);
		else
			next = GA(next, GA_Tries);

		delete cur;
		cur = next;
	}

	return cur;
}

void exp1() {
	puts("start exp1");
	
	// char path[100];
	const int maxCapacity = 10000;
	const int smallSize = 100;
	const int bigSize = 200;
	const int sizeUp = bigSize - smallSize;
	
	Generation *res, *smallMax = NULL;
	
	/* --------------------------------------------- */
	
	// const TestType TT_small = { DINIC, AC, SPC, smallSize, square(smallSize) / 2, maxCapacity };
	
	/*
	int maxFlag = 1;
	for(int i = 1; i <= 10; i++) {
		sprintf(path, "res/small-%d.csv", i);
		startLog(path);
		
		res = new Generation();
		res->randomCreation();
		res = originalGA(TT_small, 150000);
		
		printf("small-%d complete: %lld\n", i, res->max_fitness());
		
		if(i == 1)
			smallMax = res;
		else if (smallMax->max_fitness() < res->max_fitness()) {
			delete smallMax;
			smallMax = res;
			maxFlag = 1;
		}
		else
			delete res;
	}
	
	printf("smallMax flag: %d\n", maxFlag);
	
	*/
	
	/* ---------------------------------------------- */
	
	smallMax = new Generation();
	smallMax->load("res/smallMax.dump");
	
	const TestType TT_big = { DINIC, AC, SPC, bigSize, square(bigSize) / 2, maxCapacity };
	
	startLog("res/smallToBig.csv");
	res = sizeFlexibleGA(smallMax, TT_big, sizeUp * 10000);
	res->dump("res/smallToBig.dump");
	
	printf("smallToBig complete: %lld\n", res->max_fitness());
}


void exp2() {
	puts("start exp2");
	const int fitnessLimit = 200000;
	const TestType DN1 = { DINIC, AC, SPC, 100, 5000, 10000 };
	const TestType DN2 = { DINIC, AC, SPC, 200, 20000, 10000 };
	const TestType EC1 = { EC, AC, SPC, 100, 5000, 10000 };
	const TestType EC2 = { EC, AC, SPC, 200, 20000, 10000 };

	clock_t s, e;
	double duration;

	startLog("res/dinic_small.csv");
	s = clock();
	Generation *res_dinic1 = originalGA(DN1, fitnessLimit);
	e = clock();
	res_dinic1->dump("res/dinic_small.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Dinic (small): time: %f sec, max fitness: %lld\n", duration, res_dinic1->max_fitness());

	startLog("res/dinic_sf.csv");
	s = clock();
	Generation *res_dinic2 = sizeFlexibleGA(res_dinic1, DN2, fitnessLimit);
	Generation *res_dinic3 = GA(res_dinic2, fitnessLimit / 100);
	e = clock();
	res_dinic2->dump("res/dinic_sf1.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Dinic (size flexible): time: %f sec, max fitness: %lld\n", duration, res_dinic3->max_fitness());

	startLog("res/dinic_original.csv");
	s = clock();
	Generation *res_dinic0 = originalGA(DN2, fitnessLimit + fitnessLimit);
	e = clock();
	res_dinic0->dump("res/dinic_original.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Dinic (original): time: %f sec, max fitness: %lld\n", duration, res_dinic0->max_fitness());
	
	startLog("res/ec_small.csv");
	s = clock();
	Generation *res_ec1 = originalGA(EC1, fitnessLimit);
	e = clock();
	res_ec1->dump("res/ec_small.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Edmond-Karp (small): time: %f sec, max fitness: %lld\n", duration, res_ec1->max_fitness());

	startLog("res/ec_sf.csv");
	s = clock();
	Generation *res_ec2 = sizeFlexibleGA(res_ec1, EC2, fitnessLimit);
	Generation *res_ec3 = GA(res_ec2, fitnessLimit / 100);
	e = clock();
	res_ec2->dump("res/ec_sf.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Edmond-Karp (size flexible): time: %f sec, max fitness: %lld\n", duration, res_ec3->max_fitness());

	startLog("res/ec_original.csv");
	s = clock();
	Generation *res_ec0 = originalGA(EC2, fitnessLimit + fitnessLimit);
	e = clock();
	res_ec0->dump("res/ec_original.dump");
	duration = (double)(e - s) / CLOCKS_PER_SEC;
	printf("Edmond-Karp (original): time: %f sec, max fitness: %lld\n", duration, res_ec0->max_fitness());
}


int main() {
	util_init();

	//exp1();
	exp2();

	return 0;
}
