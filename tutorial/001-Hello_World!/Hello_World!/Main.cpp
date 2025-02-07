#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main() {
	//�f�o�C�X�̃|�C���^���擾
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, true, true, 0);

	//�f�o�C�X�̍쐬���������Ă��Ȃ�������I��
	if (!device) {
		return 1;
	}

	//�E�B���h�E�̃^�C�g����ݒ�
	device->setWindowCaption(L"Hello_World!");

	//�e��g�b�v�m�[�h�̃|�C���^���擾
	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	//�m�[�h�Ƃ���GUI���x����ǉ�
	GUIEnvironment->addStaticText(L"Hello_World! Multi byte OK ?", rect<s32>(10, 10, 210, 25), true);

	//�J���g���f�B���N�g�������C�u�����̃��f�B�A�t�@�C�����l�܂����f�B���N�g���ɕύX
	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");

	//���f���̎�荞��
	IAnimatedMesh *Queke = sceneManager->getMesh("sydney.md2");

	if (!Queke) {
		device->drop();
		return 1;
	}

	//�m�[�h�Ƃ��ă��f����ǉ�
	IAnimatedMeshSceneNode *node = sceneManager->addAnimatedMeshSceneNode(Queke);

	if (node) {
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(EMAT_STAND);
		node->setMaterialTexture(0, videoDriver->getTexture("sydney.bmp"));

	}

	//�m�[�h�Ƃ��ăJ������ǉ�
	sceneManager->addCameraSceneNode(node, vector3df(0, 30, -40), vector3df(0, 5, 0));

	while (device->run()) {
		videoDriver->beginScene(true,true,SColor(255,100,100,140));
		{
			sceneManager->drawAll();
			GUIEnvironment->drawAll();
		}
		videoDriver->endScene();
	}

	device->drop();

	return 0;
}