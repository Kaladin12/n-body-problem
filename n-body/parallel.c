#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "Header.h"

//double bodies_[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, masses_[BODY_NUM] = { 54,12,3 };
//double forces_[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, positions_[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, velocities_[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, forces_old_[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} };


void parallel_do_positions() {
	int i, axis;
#pragma omp parallel for schedule(dynamic, 50) num_threads(8)
	for (i = 0; i < BODY_NUM; i++)
	{
		for (axis = 0; axis < 3; axis++)
		{
			positions[i][axis] = velocities[i][axis] * dt;
		}
	}
}

void parallel_do_velocities() {
	int i, axis;
#pragma omp parallel for schedule(dynamic, 50) num_threads(8)
	for (i = 0; i < BODY_NUM; i++)
	{
		for (axis = 0; axis < 3; axis++)
		{
			velocities[i][axis] = velocities[i][axis] + (((forces[i][axis] + forces_old[i][axis]) * dt) / masses[i]);
			forces_old[i][axis] = forces[i][axis];
		}
	}
}

void parallel_restore_forces() {
	int i, j;
#pragma omp parallel for schedule(dynamic, 50) num_threads(8)
	for ( i = 0; i < BODY_NUM; i++)
	{
		for ( j = 0; j < 3; j++)
		{
			forces[i][j] = 0;
		}
	}
}

void n_body_parallel() {

	parallel_restore_forces();
	for (double t = 0; t < 1; t += dt)
	{
		parallel_do_positions();
		parallel_restore_forces();
		int i, j;
		double* res;
#pragma omp parallel for schedule(dynamic, 50) num_threads(8) private(res)
		for (i = 0; i < BODY_NUM; i++)
		{
			for (j = 0; j < BODY_NUM; j++)
			{
				if (i != j) {
					 res = sequential_gravitational_force(&bodies[i], &bodies[j], masses[i], masses[j]);
					//printf("vals %.17g, %.17g, %.17g\n", res[0], res[1], res[2]);
					forces[i][0] += res[0];
					forces[i][1] += res[1];
					forces[i][2] += res[2];
				}

			}
		}
		parallel_do_velocities();
		
		//printf("%lf", t);
	}
	printf("vals %.17g, %.17g, %.17g\n", forces[1][0], forces[1][1], forces[1][2]);

}

void init_all_parallel() {
	int i, j;
#pragma omp parallel  num_threads(8)
	{
#pragma omp  for schedule(dynamic, 50)
		for (i = 0; i < BODY_NUM; i++)
		{
			for (j = 0; j < 3; j++)
			{
				bodies[i][j] = RandomReal(1, 100000);
				positions[i][j] = RandomReal(1, 1000);
				velocities[i][i] = RandomReal(1, 1000);
				forces_old[i][j] = 1;
			}
			masses[i] = RandomReal(1, 100000);
		}
	}
	printf("\nPOSITIONS %.17g, %.17g, %.17g\n", positions[0][0], positions[0][1], positions[0][2]);
}
