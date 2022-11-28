#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Header.h"

void seq() {
	printf("SECUENCIAL");
	double start = omp_get_wtime();
	init_all();
	n_body_seq();
	double end = omp_get_wtime();
	printf("TIME: %.17g", end - start);
}

void parallel() {
	printf("PARALELO");
	double start = omp_get_wtime();
	init_all_parallel();
	n_body_parallel();
	double end = omp_get_wtime();
	printf("TIME: %.17g", end - start);
	
}

int main() {
	//seq();
	parallel();
	return 0;
}