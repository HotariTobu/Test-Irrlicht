#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{
	//Useing opengl, Screen size 640, 480, 32 bit, fullscreen = false, stencilbuffer = true, vsync = false
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, true, false);
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	/////////////////////////////////////////////////////////////////////////
	//        START   ----    CAMERA        
	/////////////////////////////////////////////////////////////////////////
	//parent = 0, rotateSpeed AKA mouse = 100, moveSpeed = 0.1f
	scene::ICameraSceneNode * camera = smgr->addCameraSceneNodeFPS(0, 100.f, 0.1f);
	camera->setPosition(core::vector3df(40.f, 20.f, 5.f));
	device->getCursorControl()->setVisible(false);

	/////////////////////////////////////////////////////////////////////////
	//        END   ----    CAMERA       
	/////////////////////////////////////////////////////////////////////////

	//Add light so we can see the nice water
	smgr->setAmbientLight(video::SColorf(0.3, 0.3, 0.3, 1));

	//Create mesh to add water effect to
	IAnimatedMesh* plane = smgr->addHillPlaneMesh("plane" // Name of mesh
		,core::dimension2d<f32>(20, 20) //	Size of a tile of the mesh. (10.0f, 10.0f) would be a good value to start, for example. 
		,core::dimension2d<u32>(40, 40)
		, 0
		, 0 // 	Specifies how much tiles there will be. If you specifiy for example that a tile has the size (10.0f, 10.0f) and the tileCount is (10,10), than you get a field of 100 tiles which has the dimension 100.0fx100.0f. 
		,core::dimension2d<f32>(0, 0) //material 
		,core::dimension2d<f32>(10, 10)
	); //countHills 

	plane = smgr->addHillPlaneMesh("myHill", dimension2d<f32>(20, 20), dimension2d<u32>(200, 200), 0, 0, dimension2d<f32>(0, 0), dimension2d<f32>(10, 10));


										 //mesh, waveheight = 5.0f, wave speed = 300.0f, wave length = 40.0f
	ISceneNode* sea = smgr->addWaterSurfaceSceneNode(plane->getMesh(0)
		, 5.0f
		, 300.0f
		, 40.0f
	);
	sea->setMaterialTexture(0, driver->getTexture("stones.jpg"));
	sea->setMaterialTexture(1, driver->getTexture("water.jpg"));
	sea->setMaterialFlag(EMF_LIGHTING, true);
	sea->setMaterialType(video::EMT_REFLECTION_2_LAYER);

	/////////////////////////////////////////////////////////////////////////
	//                             Main loop       
	/////////////////////////////////////////////////////////////////////////
	int lastFPS = -1;
	while (device->run())
		if (device->isWindowActive())
		{
			driver->beginScene(true, true, video::SColor(0, 200, 200, 200));
			smgr->drawAll();
			driver->endScene();

			//Shows FPS in the title screen
			int fps = driver->getFPS();
			if (lastFPS != fps)
			{
				core::stringw str = L"Water Example[";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
		else {
			device->yield(); //Cause the device to temporarily pause execution and let other processes run. 
		}//End else
		device->drop();
		return 0;
}