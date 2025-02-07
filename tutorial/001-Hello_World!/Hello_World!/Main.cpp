#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main() {
	//デバイスのポインタを取得
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, true, true, 0);

	//デバイスの作成が成功していなかったら終了
	if (!device) {
		return 1;
	}

	//ウィンドウのタイトルを設定
	device->setWindowCaption(L"Hello_World!");

	//各種トップノードのポインタを取得
	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	//ノードとしてGUIラベルを追加
	GUIEnvironment->addStaticText(L"Hello_World! Multi byte OK ?", rect<s32>(10, 10, 210, 25), true);

	//カントリディレクトリをライブラリのメディアファイルが詰まったディレクトリに変更
	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");

	//モデルの取り込み
	IAnimatedMesh *Queke = sceneManager->getMesh("sydney.md2");

	if (!Queke) {
		device->drop();
		return 1;
	}

	//ノードとしてモデルを追加
	IAnimatedMeshSceneNode *node = sceneManager->addAnimatedMeshSceneNode(Queke);

	if (node) {
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(EMAT_STAND);
		node->setMaterialTexture(0, videoDriver->getTexture("sydney.bmp"));

	}

	//ノードとしてカメラを追加
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