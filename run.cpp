#include "genetic.h"
#include "utils.h"

extern int fitnessCount;

Generation *GA(const TestType TT, const int fitnessLimit) {
	Generation *cur = new Generation(TT);
	cur->randomCreation();

	int generation = 0;
	fprintf(stderr, "%d generation: %lld\n", generation, cur->max_fitness());

	while(fitnessCount < fitnessLimit) {
		Generation *next = new Generation(TT);

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

		fprintf(stderr, "%d generation: %lld\n", ++generation, cur->max_fitness());
	}

	cur->sort();
	return cur;
}

Generation *sizeFlexibleGA(Generation *From, const TestType TT, const int fitnessLimit) {



	return From;
}

void exp1() {
	fitnessCount = 0;
	const int fitnessLimit = 500000;

	const TestType TT1 = { DINIC, AC, SPC, 100, 5000, 10000 };
	Generation *res1 = GA(TT1, fitnessLimit);

	const TestType TT2 = { DINIC, AC, SPC, 200, 10000, 10000 };

	Generation *res2 = GA(TT2, fitnessLimit);
	Generation *res3 = sizeFlexibleGA(res1, TT2, fitnessLimit);

	// Now compare res2 and res3
}

int main() {
	util_init();

	exp1();

	return 0;
}
