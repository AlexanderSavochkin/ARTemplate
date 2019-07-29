#include "SphereModule.h"
#include <osg/ShapeDrawable>

SphereModule::SphereModule() {
    sphere_node = new osg::Group;
    osg::ref_ptr<osg::ShapeDrawable> sphere0 = new osg::ShapeDrawable;
    sphere0->setShape(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 0.5));
    sphere0->setColor(osg::Vec4(4.0, 4.0, 4.0, 1.0));
    sphere_node->addChild(sphere0.get());

	osg::ref_ptr<osg::ShapeDrawable> sphere1 = new osg::ShapeDrawable;
	sphere1->setShape(new osg::Sphere(osg::Vec3(1.0, 0.0, 0.0), 0.5));
	sphere1->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
	sphere_node->addChild(sphere1.get());

	osg::ref_ptr<osg::ShapeDrawable> sphere2 = new osg::ShapeDrawable;
	sphere2->setShape(new osg::Sphere(osg::Vec3(0.0, 1.0, 0.0), 0.5));
	sphere2->setColor(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	sphere_node->addChild(sphere2.get());

	osg::ref_ptr<osg::ShapeDrawable> sphere3 = new osg::ShapeDrawable;
	sphere3->setShape(new osg::Sphere(osg::Vec3(0.0, 0.0, 1.0), 0.5));
	sphere3->setColor(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	sphere_node->addChild(sphere3.get());

}

osg::ref_ptr<osg::Group> SphereModule::getSubgraph() {
    return sphere_node;
}

