#include <osg/Geometry> 
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>

//For printing osg pbjects for debug purposes
#include <osg/io_utils>

#include "modules/nbody/NBodyARModule.h"
#include "modules/SphereModule.h"
#include "Timer.h"

osg::Node *createLightSource(int num,
	const osg::Vec3 &dir,
	const osg::Vec4 &color)
{
	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(num);
	light->setDiffuse(color);
	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
	lightSource->setLight(light);

	osg::ref_ptr<osg::MatrixTransform> sourceTrans = new osg::MatrixTransform;
	sourceTrans->setMatrix(osg::Matrix::translate(dir));
	sourceTrans->addChild(lightSource.get());

	return sourceTrans.release();
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	osg::ref_ptr<osg::Group> root = new osg::Group;

	//Initialize background
	Background background;
    //NBodyARModule ar_module;       
	SphereModule ar_module;

	osg::ref_ptr<osg::Camera> backGroundCamera = background.getBackgroundCamera();
	backGroundCamera->getOrCreateStateSet()->setRenderBinDetails(1, "RenderBin");

	//Initialize module
	osg::ref_ptr<osg::Group> ar_scene = ar_module.getSubgraph();
	ar_scene->getOrCreateStateSet()->setRenderBinDetails(2, "RenderBin");

	osg::ref_ptr<osg::MatrixTransform> tag_transform = new osg::MatrixTransform;
	Vector3d position(0.0, 0.0, 13.0);
	tag_transform->setMatrix(osg::Matrix::translate(position(0), position(1), position(2)));
	tag_transform->addChild(ar_scene);

	//root->addChild(ar_scene.get());
	root->addChild(tag_transform.get());
	root->addChild(backGroundCamera.get());

	osg::Node *light0 = createLightSource(0, osg::Vec3(0.0f, 0.0f, 1.0f),
		osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
	root->addChild(light0);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(0, 0, 1200, 800);

    auto physical_camera_matrix = background.getPhysicalCameraMatrix();
	auto background_camera_projection = backGroundCamera->getProjectionMatrix();

	osg::Matrixd projection_matrix = physical_camera_matrix;

	viewer.getCamera()->setProjectionMatrix(projection_matrix);

	viewer.getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	viewer.getCamera()->setCullingMode(osg::CullSettings::NO_CULLING);

	auto cam_rotation_matr = viewer.getCamera()->getViewMatrix();

	viewer.getCamera()->setAllowEventFocus(false);

	auto cam_rotation_matr2 = viewer.getCamera()->getViewMatrix();

	viewer.setSceneData(root.get());
	viewer.addEventHandler(new TimerHandler(ar_module, background, tag_transform));

	return viewer.run();
}