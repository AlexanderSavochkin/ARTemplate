#include <osg/ShapeDrawable>
#include <osg/Geode>

#include "NBodyARModule.h"

NBodyARModule::NBodyARModule() :
    integrator(universe)
{
    initialize_hardcoded_universe();
    integrator.initialize();
	createSubgraph();
}

osg::ref_ptr<osg::Group> NBodyARModule::getSubgraph() 
{
    return nbody;
}

void NBodyARModule::Update(double dt) 
{
    integrator.update(dt);
    for (int i = 0; i < universe.getObjects().size(); ++i) {
        //Translation
        Vector3d position = universe.getObjects()[i].getPosition();
        osgBodiesTransforms[i]->setMatrix(osg::Matrix::translate(position(0), position(1), position(2)));
    }
}

void NBodyARModule::createSubgraph()
{
	nbody = new osg::Group;
	osg::ref_ptr<osg::ShapeDrawable> sphere = new osg::ShapeDrawable;
	sphere->setShape(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 1.0));
	sphere->setColor(osg::Vec4(0.0, 0.0, 1.0, 1.0));

	tuple<const Matrix<double, 3, Dynamic>&, const Matrix<double, 3, Dynamic>&> state = universe.state();
	const Matrix<double, 3, Dynamic>& positions = get<0>(state);
	for (int i = 0; i < universe.getObjects().size(); ++i) {
		////Translation
		osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
		Vector3d position = positions.col(i);
		transform->setMatrix(osg::Matrix::translate(position(0), position(1), position(2)));
		//
		osgBodiesTransforms.push_back(transform);
		nbody->addChild(transform.get());

		////Scaling sphere
		osg::ref_ptr<osg::MatrixTransform> scale = new osg::MatrixTransform;
		double radius = universe.getObjects()[i].getRadius();
		scale->setMatrix(osg::Matrix::scale(osg::Vec3(radius, radius, radius)));
		transform->addChild(scale.get());

		//Adding geometry
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(sphere.get());
		scale->addChild(geode.get());
	}
}

void NBodyARModule::initialize_hardcoded_universe() {
	universe.setGravity(2.0);

	universe.addObject(
		Vector3d(0.0, 0.0, 0.0),
		Vector3d(0.0, 0.0, 0.0),
		1.0,
		1.0);

	universe.addObject(
		Vector3d(2.0, 0.0, 0.0),
		Vector3d(0.0, 1.2, 0.0),
		0.1,
		0.464);

	universe.addObject(
		Vector3d(0.0, 0.0, 3.0),
		Vector3d(0.9, 0.0, 0.0),
		0.1,
		0.464);
}
