#include <irrlicht.h>
#include <iostream>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

int main() {
	E_DRIVER_TYPE driverType;

	cout << "���̗�̂��߂ɂ��Ȃ��̎v���h���C�o�[��I��ł��������B\n" << endl;
	cout << "(a) OpenGL" << endl;
	cout << "(b) Direct3D" << endl;
	cout << "(c) Burning's Software Renderer" << endl;
	cout << "(d) Software Renderer" << endl;
	cout << "(e) NullDevice(���̃L�[) �I�����܂��B" << endl;

	char selection;
	cin >> selection;

	switch (selection) {
	case 'a':
		driverType = EDT_OPENGL;
		break;
	case 'b':
		driverType = EDT_DIRECT3D9;
		break;
	case 'c':
		driverType = EDT_BURNINGSVIDEO;
		break;
	case 'd':
		driverType = EDT_SOFTWARE;
		break;
	case 'e':
		driverType = EDT_NULL;
		break;
	default:
		return 1;
	}

	IrrlichtDevice *device = createDevice(driverType, dimension2d<u32>(640, 480), 32, false, true, true, 0);

	if (!device) {
		return 1;
	}

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	//�A�[�J�C�u���t�@�C���V�X�e���ɒǉ�
	device->getFileSystem()->addFileArchive("irrlicht-1.8.4/media/map-20kdm2.pk3");

	IAnimatedMesh *map = sceneManager->getMesh("20kdm2.bsp");

	ISceneNode *node = 0;

	if (map) {
		node = sceneManager->addOctreeSceneNode(map->getMesh(0), 0, -1, 1024);
	}
	else {
		device->drop();
		return 1;
	}

	if (node) {
		node->setPosition(vector3df(-1300, -144, -1249));
	}

	//�P�l�̂̃J������ǉ�
	sceneManager->addCameraSceneNodeFPS();

	//�J�[�\�����\��
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	while (device->run()) {
		if (device->isWindowActive()) {
			videoDriver->beginScene(true, true, SColor(255, 200, 200, 200));
			{
				sceneManager->drawAll();
			}
			videoDriver->endScene();

			int fps = videoDriver->getFPS();

			if (lastFPS != fps) {
				stringw caption = L"Quake 3 Map [";
				caption += videoDriver->getName();
				caption += L"] fps : ";
				caption += fps;

				device->setWindowCaption(caption.c_str());

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