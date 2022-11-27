#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Header.h"

//double bodies[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, masses[BODY_NUM] = { 54,12,3 };
//double forces[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, positions[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, velocities[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} }, forces_old[BODY_NUM][3] = { {1,25,65},{75,412,3},{45,158,3} };


double *sequential_gravitational_force(double q1[], double q2[], double m2) {
	
	double signed_difference[3] = { q2[0] - q1[0],q2[1] - q1[1], q2[2] - q1[2] };
	double distance = sqrt(pow(signed_difference[0], 2) + pow(signed_difference[1], 2) + pow(signed_difference[2], 2));
	double res_force[3];
	for (int i = 0; i < 3; i++)
	{
		res_force[i] = (G * signed_difference[i] * m2 * m2) / pow(distance, 3); 
	}
	//printf("POINT %.17g %.17g %.17g\n", sign[0], sign[1], sign[2]);
	return res_force;
}

void sequential_update_position() {

	for (int i = 0; i < BODY_NUM; i++)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			positions[i][axis] = velocities[i][axis] * dt;
		}
	}

}

void sequential_update_velocity( ) {

	for (int i = 0; i < BODY_NUM; i++)
	{
		for (int axis = 0; axis < 3; axis++)
		{
			velocities[i][axis] = velocities[i][axis] + (((forces[i][axis]+forces_old[i][axis]) * dt) / masses[i]);
			forces_old[i][axis] = forces[i][axis];
		}
	}

}

void do_positions() {
	for (int i = 0; i < BODY_NUM; i++)
	{
		sequential_update_position();
	}
}

void do_velocities() {
	for (int i = 0; i < BODY_NUM; i++)
	{
		sequential_update_velocity();
	}
}

void restore_forces() {
	for (int i = 0; i < BODY_NUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			forces[i][j] = 0;
		}
	}
}

void n_body_seq() {

	restore_forces();
	for (double t = 0; t < 1; t+=dt )
	{
		do_positions();
		restore_forces();
		for (int i = 0; i < BODY_NUM; i++)
		{
			for (int j = 0; j < BODY_NUM; j++)
			{
				if (i != j) {
					double* res = sequential_gravitational_force(&bodies[i], &bodies[j], masses[i]);
					//printf("vals %.17g, %.17g, %.17g\n", res[0], res[1], res[2]);
					forces[i][0] += res[0];
					forces[i][1] += res[1];
					forces[i][2] += res[2];
				}

			}
		}
		do_velocities();
		//printf("%lf", t);
		
	}
	printf("vals %.17g, %.17g, %.17g\n", forces[0][0], forces[0][1], forces[0][2]);

}

void init_all() {
	for (int i = 0; i < BODY_NUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			bodies[i][j] = RandomReal(0, 100000);
			positions[i][j] = RandomReal(0, 1000);
			velocities[i][i] = RandomReal(0, 1000);
			forces_old[i][j] = 0;
		}
		masses[i] = RandomReal(0, 100000);
	}
	printf("\nPOSITIONS %.17g, %.17g, %.17g\n", positions[0][0], positions[0][1], positions[0][2]);
}
