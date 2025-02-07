#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#define MOVEMENT_SPEED 10.0f

//イベントレシーバー
class eventReceiver : IEventReceiver {
public :
	//trueを返すとイベントが来なくなる。
	virtual bool OnEvent(const SEvent& event) {
		if (event.EventType == EET_KEY_INPUT_EVENT) {
			keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		return false;
	}

	bool isKeyDown(EKEY_CODE keyCode) {
		return keyIsDown[keyCode];
	}

	eventReceiver() {
		for (u16 i = 0; i < KEY_KEY_CODES_COUNT; i++) {
			keyIsDown[i] = false;
		}
	}

private :
	bool keyIsDown[KEY_KEY_CODES_COUNT];

};

int main() {
	eventReceiver receiver;
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, true, true, (IEventReceiver*)&receiver);

	if (!device) {
		return 1;
	}

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");

	ISceneNode *sphere = sceneManager->addSphereSceneNode();
	ISceneNode *cube = sceneManager->addCubeSceneNode();
	IAnimatedMeshSceneNode *ninja = sceneManager->addAnimatedMeshSceneNode(sceneManager->getMesh("ninja.b3d"));

	if (sphere) {
		sphere->setPosition(vector3df(0, 0, 30));
		sphere->setMaterialTexture(0, videoDriver->getTexture("wall.bmp"));
		sphere->setMaterialFlag(EMF_LIGHTING, false);
	}

	if (cube) {
		cube->setMaterialTexture(0, videoDriver->getTexture("t351sml.jpg"));
		cube->setMaterialFlag(EMF_LIGHTING, false);

		ISceneNodeAnimator *animator = sceneManager->createFlyCircleAnimator(vector3df(0, 0, 30), 20.0f);

		if (animator) {
			cube->addAnimator(animator);
			animator->drop();
		}
	}

	if (ninja) {
		ISceneNodeAnimator *animator = sceneManager->createFlyStraightAnimator(vector3df(100, 0, 60), vector3df(-100, 0, 60), 8000, true);

		if (animator) {
			ninja->addAnimator(animator);
			animator->drop();
		}

		//ninja->setMaterialTexture(0, videoDriver->getTexture("sydney.bmp"));
		ninja->setMaterialFlag(EMF_LIGHTING, false);

		ninja->setFrameLoop(0, 13);
		ninja->setAnimationSpeed(15);
		//ninja->setMD2Animation(EMAT_RUN);

		ninja->setScale(vector3df(2.0f, 2.0f, 2.0f));
		ninja->setRotation(vector3df(0, -90, 0));

	}

	sceneManager->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	GUIEnvironment->addImage(videoDriver->getTexture("irrlichtlogoalpha2.tga"), position2d<s32>(10, 10));

	int lastFPS = -1;
	u32 then = device->getTimer()->getTime();

	while (device->run()) {
		u32 now = device->getTimer()->getTime();
		f32 frameDeltaTime = (f32)(now - then) / 1000.0f;
		then = now;

		vector3df spherePosition = sphere->getPosition();

		if (receiver.isKeyDown(KEY_KEY_W)) {
			spherePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		}
		else if (receiver.isKeyDown(KEY_KEY_S)) {
			spherePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;
		}

		if (receiver.isKeyDown(KEY_KEY_D)) {
			spherePosition.X += MOVEMENT_SPEED * frameDeltaTime;
		}
		else if (receiver.isKeyDown(KEY_KEY_A)) {
			spherePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		}

		if (receiver.isKeyDown(KEY_ESCAPE)) {
			break;
		}

		sphere->setPosition(spherePosition);

		videoDriver->beginScene(true, true, SColor(255, 113, 113, 133));
		{
			sceneManager->drawAll();
			GUIEnvironment->drawAll();
		}
		videoDriver->endScene();

		int fps = videoDriver->getFPS();

		if (lastFPS != fps) {
			stringw caption = L"Movement [";
			caption += videoDriver->getName();
			caption += L"] fps : ";
			caption += fps;

			device->setWindowCaption(caption.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}