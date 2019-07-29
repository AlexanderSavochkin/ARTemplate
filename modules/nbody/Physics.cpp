#include "Physics.h" 

void Universe::addObject(
    const Vector3d& position,
    const Vector3d& velocity,
    double mass,
    double radius) 
{
    CeleistailBody new_object(*this);
    new_object.mass = mass;
    new_object.radius = radius;

	new_object.index = objects.size();

    state_positions.conservativeResize(state_positions.rows(), state_positions.cols() + 1);
    state_positions.col(state_positions.cols() - 1) = position;

    state_velocities.conservativeResize(state_velocities.rows(), state_velocities.cols() + 1);
    state_velocities.col(state_velocities.cols() - 1) = velocity;
    objects.push_back(new_object);
}
