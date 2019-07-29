#pragma once

#include <osg/Geometry> 

class ARModule {
public:
	ARModule() {};
	virtual ~ARModule() {};
    virtual void Update(double dt) = 0;
    virtual osg::ref_ptr<osg::Group> getSubgraph() = 0;
};