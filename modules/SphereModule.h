#pragma once

#include "ARModule.h"

class SphereModule : public ARModule {
    osg::ref_ptr<osg::Group> sphere_node;
public:
	SphereModule();
    virtual ~SphereModule() {};
    virtual void Update(double dt) {};
    virtual osg::ref_ptr<osg::Group> getSubgraph();
};