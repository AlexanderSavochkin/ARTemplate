#include "Integrator.h"

double pown(double x, unsigned n)
{
	double y = 1;
	// n = 2*d + r. x^n = (x^2)^d * x^r.
	unsigned d = n >> 1;
	unsigned r = n & 1;
	double x_2_d = d == 0 ? 1 : pown(x*x, d);
	double x_r = r == 0 ? 1 : x;
	return x_2_d * x_r;
}

void RKNIntegrator::update(double dt) {
	tuple<const Matrix<double, 3, Dynamic>&, const Matrix<double, 3, Dynamic>&> state =
		universe.state();
	const Matrix<double, 3, Dynamic>& positions = get<0>(state);
	const Matrix<double, 3, Dynamic>& velocities = get<1>(state);

	//K1
	Matrix<double, 3, Dynamic> K1 = ZeroMatrix;
	for (int i = 0; i < universe.getObjects().size(); ++i) {
		for (int j = i + 1; j < universe.getObjects().size(); ++j) {
			Vector3d v_ij = positions.col(j) - positions.col(i);
			double distance_ij = v_ij.norm();
			Vector3d f_ij = v_ij / pown(distance_ij, 3);
			K1.col(i) += universe.getGravity() * universe.getObjects()[j].getMass() * f_ij;
			K1.col(j) -= universe.getGravity() * universe.getObjects()[i].getMass() * f_ij;
		}
	}

	positions_2_and_3 = positions + 0.5 * dt * (velocities + 0.25 * dt * K1);
		
	//K2_and_3
	Matrix<double, 3, Dynamic> K2_and_3 = ZeroMatrix;
	for (int i = 0; i < universe.getObjects().size(); ++i) {
		for (int j = i + 1; j < universe.getObjects().size(); ++j) {
			Vector3d v_ij = positions_2_and_3.col(j) - positions_2_and_3.col(i);
			double distance_ij = v_ij.norm();
			Vector3d f_ij = v_ij / pown(distance_ij, 3);
			K2_and_3.col(i) += universe.getGravity() * universe.getObjects()[j].getMass() * f_ij;
			K2_and_3.col(j) -= universe.getGravity() * universe.getObjects()[i].getMass() * f_ij;
		}
	}

	positions_4 = positions + dt * (velocities + 0.5 * dt * K2_and_3);

	//K4
	Matrix<double, 3, Dynamic> K4 = ZeroMatrix;
	for (int i = 0; i < universe.getObjects().size(); ++i) {
		for (int j = i + 1; j < universe.getObjects().size(); ++j) {
			Vector3d v_ij = positions_4.col(j) - positions_4.col(i);
			double distance_ij = v_ij.norm();
			Vector3d f_ij = v_ij / pown(distance_ij, 3);
			K2_and_3.col(i) += universe.getGravity() * universe.getObjects()[j].getMass() * f_ij;
			K2_and_3.col(j) -= universe.getGravity() * universe.getObjects()[i].getMass() * f_ij;
		}
	}

	new_positions = positions + dt * velocities + dt * dt / 6.0 * (K1 + 2.0 * K2_and_3);
	new_velocities = velocities + dt / 6.0 * (K1 + 2.0 * K2_and_3 + K4);

	universe.setState(new_positions, new_velocities);
}

void RKNIntegrator::initialize() {
	tuple<const Matrix<double, 3, Dynamic>&, const Matrix<double, 3, Dynamic>&> state =
		universe.state();
	const Matrix<double, 3, Dynamic>& positions = get<0>(state);
	const Matrix<double, 3, Dynamic>& velocities = get<1>(state);
		
	ZeroMatrix = Matrix<double, 3, Dynamic>::Zero(3, positions.cols());
	K1.resize(3,positions.cols());
	K2_and_3.resize(3, positions.cols());
	K4.resize(3, positions.cols());

	positions_2_and_3.resize(3, positions.cols());
	positions_4.resize(3, positions.cols());
}