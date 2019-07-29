#pragma once

#include <vector>
#include <string>
#include <Eigen/Dense>
#include <tuple>

using std::vector;
using std::string;
using std::tuple;
using std::tie;

using Eigen::Matrix;
using Eigen::Dynamic;
using Eigen::Vector3d;

class Universe
{
public:
	class CeleistailBody
	{
		friend Universe;
		double mass;
		double radius;
		string name;
		int index;
		Universe& universe;
		CeleistailBody(Universe& universe) : universe(universe) {}
	public:
		Vector3d getPosition() const { return universe.state_positions.col(index); }
		Vector3d getVelocity() const { return universe.state_velocities.col(index); }
		double getMass() const { return mass; }
		double getRadius() const { return radius; }
	};

private:
    vector<CeleistailBody> objects;
    double gravitation_constant;
	Matrix<double, 3, Dynamic> state_positions;
	Matrix<double, 3, Dynamic> state_velocities;

public:
	void setGravity(double gravitation_constant) {
		this->gravitation_constant = gravitation_constant;
	}

	double getGravity() const { return gravitation_constant; }

    const vector<CeleistailBody>& getObjects() const { return objects; }
    void addObject(
        const Vector3d& position,
        const Vector3d& velocity,
        double mass,
        double radius);

    tuple<Matrix<double, 3, Dynamic>&, Matrix<double, 3, Dynamic>&> state() { return tie(state_positions, state_velocities); }
    tuple<const Matrix<double, 3, Dynamic>&, const Matrix<double, 3, Dynamic>&> state() const { return tie(state_positions, state_velocities); }

    void setState(Matrix<double, 3, Dynamic>& positions_, Matrix<double, 3, Dynamic>& velocities_) {
        state_positions = positions_;
        state_velocities = velocities_;
    }
};