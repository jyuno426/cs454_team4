#include "utils.h"
#include "genetic.h"

extern int fitnessCount;

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

		printf("fitness: %lld\n", cur->max_fitness());
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
	// Steps          : sqrt(FL/PS)
	// SizeManip tries: sqrt(FL/PS)/2
	// GA tries       : sqrt(FL/PS)/2

	const TestType TT_from = from->TT;
	TestType TT_cur(TT_from);
	Generation *cur = from;

	int i, j, k;
	int steps, SM_Tries, GA_Tries;
	for(steps = 1; steps * steps * populationSize < fitnessLimit; steps++);
	SM_Tries = steps / 2;
	GA_Tries = steps - SM_Tries;

	for(i = 1; i <= steps; i++) {
		printf("Step: %d\n", i);
		int V_change = (TT_to.V - TT_from.V) * i / steps - (TT_to.V - TT_from.V) * (i - 1) / steps;
		int E_change = (TT_to.E - TT_from.E) * i / steps - (TT_to.E - TT_from.E) * (i - 1) / steps;

		TT_cur.V += V_change;
		TT_cur.E += E_change;

		Generation *next = new Generation(TT_cur);

		puts("sizeManipulating");
		// sizeManipulation
		for(j = 0; j < populationSize; j++) {
			// option 1
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

			// option 2?
		}

		puts("GA");
		// GA over same size
		next = GA(next, GA_Tries);

		delete cur;
		cur = next;
	}

	return cur;
}

void exp1() {
	const int fitnessLimit = 50000;

	const TestType TT1 = { DINIC, AC, SPC, 100, 5000, 10000 };
	fitnessCount = 0;
	// Generation *res1 = originalGA(TT1, fitnessLimit);
	Generation *res1 = new Generation(TT1);
	res1->randomCreation();

	const TestType TT2 = { DINIC, AC, SPC, 110, 5500, 10000 };

	// fitnessCount = 0;
	// Generation *res2 = originalGA(TT2, fitnessLimit);

	fitnessCount = 0;
	Generation *res3 = sizeFlexibleGA(res1, TT2, fitnessLimit);

	// Now compare res2 and res3
}

int main() {
	util_init();

	exp1();

	return 0;
}
