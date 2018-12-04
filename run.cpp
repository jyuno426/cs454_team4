#include "genetic.h"
#include "utils.h"

extern int fitnessCount;

void GA(SolverType S, GraphType G, CrossoverType CO,
	int V, int E, int C, const int fitnessLimit) {

	Generation *cur = new Generation(V, E, C, S, G, CO);
	cur->randomCreation();

	int generation = 0;
	printf("%d generation: %lld\n", generation, cur->max_fitness());

	while(fitnessCount < fitnessLimit) {
		Generation *next = new Generation(V, E, C, S, G, CO);

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

		int elitist = populationSize * 0.1 + 1e-7; // 10% from cur population
		int top = populationSize - elitist;
		for(i = 0; i < elitist; i++) {
			delete next->population[i];
			next->population[i] = new Indiv(cur->population[top + i]);
		}

		delete cur;
		cur = next;

		printf("%d generation: %lld\n", ++generation, cur->max_fitness());
	}
}

void sizeFlexibleGA(SolverType S, GraphType G, CrossoverType CO,
	/*, char *filename,*/ // initial population
	int V_from, int E_from, int C_from, int V_to, int E_to, int C_to) {
	Generation *cur, *next;
}

void exp1() {
	fitnessCount = 0;
	const int fitnessLimit = 500000;
	GA(DINIC, AC, SPC, 100, 5000, 10000, fitnessLimit);
}

int main() {
	util_init();
	exp1();
	// exp2();

	system("pause");
	return 0;
}
