#pragma once

#define G	0.1
//0.000000000066740
#define BODY_NUM	300
#define dt			0.001
double bodies[BODY_NUM][3], masses[BODY_NUM];
double forces[BODY_NUM][3], positions[BODY_NUM][3], velocities[BODY_NUM][3], forces_old[BODY_NUM][3];


double RandomReal(double low, double high);
double* sequential_gravitational_force(double q1[], double q2[], double m2, double m1);
void sequential_update_position();
void sequential_update_velocity();
void restore_forces();
void init_all_parallel();
void n_body_parallel();