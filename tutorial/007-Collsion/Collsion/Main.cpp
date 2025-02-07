#include <irrlicht.h>
#include <iostream>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

enum {
	ID_IsNotPickable = 0,
	IDFlag_IsPickable = 1 << 0,
	IDFlag_IsHighlightable = 1 << 1
};

int main() {
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false);

	if (!device) {
		return 1;
	}

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");
	device->getFileSystem()->addFileArchive("map-20kdm2.pk3");

	IAnimatedMesh *map = sceneManager->getMesh("20kdm2.bsp");
	IMeshSceneNode *mapNode = 0;

	if (map) {
		mapNode = sceneManager->addOctreeSceneNode(map->getMesh(0), 0, IDFlag_IsPickable);
	}
	else {
		device->drop();
		return 1;
	}

	ITriangleSelector* selector = 0;

	if (mapNode) {
		mapNode->setPosition(vector3df(-1350, -130, -1400));

		selector = sceneManager->createOctreeTriangleSelector(mapNode->getMesh(), mapNode, 128);
		mapNode->setTriangleSelector(selector);
	}

	ICameraSceneNode* camera = sceneManager->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
	camera->setPosition(vector3df(50, 50, -60));
	camera->setTarget(vector3df(-70, 30, -60));

	device->getCursorControl()->setVisible(false);

	if (selector) {
		ISceneNodeAnimator *animator = sceneManager->createCollisionResponseAnimator(selector, camera, vector3df(30, 50, 30), vector3df(0, -10, 0), vector3df(0, 30, 0));
		selector->drop();
		camera->addAnimator(animator);
		animator->drop();
	}

	IBillboardSceneNode *bill = sceneManager->addBillboardSceneNode();
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, videoDriver->getTexture("particle.bmp"));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialFlag(EMF_ZBUFFER, false);
	bill->setSize(dimension2d<f32>(20.0f, 20.0f));
	bill->setID(ID_IsNotPickable);

	IAnimatedMeshSceneNode* node = 0;
	SMaterial material;

	node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("faerie.md2"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(vector3df(-90, -15, -140));
	node->setScale(vector3df(1.6f));
	node->setMD2Animation(EMAT_POINT);
	node->setAnimationSpeed(20.f);
	material.setTexture(0, videoDriver->getTexture("faerie2.bmp"));
	material.Lighting = true;
	material.NormalizeNormals = true;
	node->getMaterial(0) = material;
	selector = sceneManager->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("ninja.b3d"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setScale(vector3df(10));
	node->setPosition(vector3df(-75, -66, -80));
	node->setRotation(vector3df(0, 90, 0));
	node->setAnimationSpeed(8.f);
	node->getMaterial(0).NormalizeNormals = true;
	node->getMaterial(0).Lighting = true;
	selector = sceneManager->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("dwarf.x"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(vector3df(-70, -66, -30));
	node->setRotation(vector3df(0, -90, 0));
	node->setAnimationSpeed(20.f);
	node->getMaterial(0).Lighting = true;
	selector = sceneManager->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	node = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("yodan.mdl"), 0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	node->setPosition(vector3df(-90, -25, 20));
	node->setScale(vector3df(0.8f));
	node->getMaterial(0).Lighting = true;
	node->setAnimationSpeed(20.f);
	selector = sceneManager->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();

	material.setTexture(0, 0);
	material.Lighting = false;

	ILightSceneNode *light = sceneManager->addLightSceneNode(0, vector3df(-60, 100, 400), SColorf(1.0f, 1.0f, 1.0f, 1.0f), 600.0f);
	light->setID(ID_IsNotPickable);

	ISceneNode *highlightedSceneNode = 0;
	ISceneCollisionManager *collMan = sceneManager->getSceneCollisionManager();

	material.Wireframe = true;

	int lastFPS = -1;

	while (device->run()) {
		if (device->isWindowActive()) {
			videoDriver->beginScene(true, true, SColor(255, 200, 200, 200));
			{
				sceneManager->drawAll();

				if (highlightedSceneNode)
				{
					highlightedSceneNode->setMaterialFlag(EMF_LIGHTING, true);
					highlightedSceneNode = 0;
				}

				line3d<f32> ray;
				ray.start = camera->getPosition();
				ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

				vector3df intersection;
				triangle3df hitTriangle;

				ISceneNode * selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, IDFlag_IsPickable, 0);

				if (selectedSceneNode) {
					bill->setPosition(intersection);

					videoDriver->setTransform(ETS_WORLD, matrix4());
					videoDriver->setMaterial(material);
					videoDriver->draw3DTriangle(hitTriangle, SColor(0, 255, 0, 0));

					if ((selectedSceneNode->getID() & IDFlag_IsHighlightable) == IDFlag_IsHighlightable) {
						highlightedSceneNode = selectedSceneNode;
						highlightedSceneNode->setMaterialFlag(EMF_LIGHTING, false);
					}
				}
			}
			videoDriver->endScene();

			int fps = videoDriver->getFPS();

			if (lastFPS != fps)
			{
				stringw str = L"Collision detection example - Irrlicht Engine [";
				str += videoDriver->getName();
				str += "] FPS:";
				str += fps;

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}

		}
		else {
			device->yield();
		}
	}

	device->drop();

	return 0;
}