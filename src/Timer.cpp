#include "Timer.h"

bool TimerHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
	switch (ea.getEventType()) {
	case osgGA::GUIEventAdapter::FRAME: {
		osgViewer::Viewer *viewer = dynamic_cast<osgViewer::Viewer *>(&aa);
		if (!viewer)
			break;
		double time = viewer->getFrameStamp()->getReferenceTime();

		if (!first_time) 
		{
			double delta_time = (time - previous_time);
		    ar_module.Update(delta_time);
		}
		else {
			first_time = false;
		}

		//Update background
		background.Update();

		previous_time = time;

		//Extract tag location and modify tag_transfor for virtual scene
		osg::Matrix tmp;
		bool see_tag = background.getExternalWorldViewMatrix(tmp);
		if (see_tag) {
			tag_transform->setMatrix(tmp);
		}

		return true;
	}
	case osgGA::GUIEventAdapter::USER:
		break;
	default:
		break;
	}
	return false;
}