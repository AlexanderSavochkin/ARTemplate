#pragma once

#include <osg/MatrixTransform>
#include <osgGA/EventHandler>
#include <osgViewer/Viewer>

#include "ARModule.h"
#include "BackgroundVideo.h"

class TimerHandler : public osgGA::GUIEventHandler {
	ARModule& ar_module;
	Background& background;
	osg::ref_ptr<osg::MatrixTransform> tag_transform;
public:
	TimerHandler(
		ARModule& ar_module,
		Background& background,
		osg::ref_ptr<osg::MatrixTransform> tag_transform) :
			ar_module(ar_module),
			background(background),
			tag_transform(tag_transform),
			first_time(true)
	{ }

	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
protected:
	bool first_time;
	double previous_time;
};