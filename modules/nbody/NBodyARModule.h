#pragma once

#include <osg/MatrixTransform>

#include "Physics.h"
#include "Integrator.h"
#include "ARModule.h"

class NBodyARModule : public ARModule {
    Universe universe;
    RKNIntegrator integrator;

    vector<osg::ref_ptr<osg::MatrixTransform>> osgBodiesTransforms; //Current spheres positions
    osg::ref_ptr<osg::Group> nbody;    
public:
    NBodyARModule();
	virtual ~NBodyARModule() {};
    virtual void Update(double dt);
    virtual osg::ref_ptr<osg::Group> getSubgraph();
private:
    void initialize_hardcoded_universe();
    void createSubgraph();
};