#pragma once

#include "Physics.h"

using std::get;

class RKNIntegrator 
{
	Universe& universe;
	Matrix<double, 3, Dynamic> ZeroMatrix;
	Matrix<double, 3, Dynamic> K1, K2_and_3, K4; //Runge-Kutta-Nystrom coefficients
	Matrix<double, 3, Dynamic> positions_2_and_3, positions_4;
	Matrix<double, 3, Dynamic> new_positions, new_velocities;

public:
	RKNIntegrator(Universe& universe) : universe(universe) {
		initialize();
	};

	void update(double dt);

	void initialize();
};
