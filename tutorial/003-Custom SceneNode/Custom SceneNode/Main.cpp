#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//シーンノードを継承したクラス
class CSampleSceneNode : public ISceneNode {
	//シーンノードがすっぽりと入る大きさの箱
	aabbox3d<f32> Box;
	S3DVertex Vertices[4];
	SMaterial Material;

public:
	//コンストラクタ
	CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id) {
		Material.Wireframe = false;
		Material.Lighting = false;

		Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0, (255, 0, 255, 255), 0, 1);
		Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0, SColor(255, 255, 0, 255), 1, 1);
		Vertices[2] = S3DVertex(0, 20, 0, 0, 1, 1, SColor(255, 255, 255, 0), 1, 0);
		Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1, SColor(255, 0, 255, 0), 0, 0);
		
		Box.reset(Vertices[0].Pos);
		for (s32 i = 1; i < 4; ++i) {
			Box.addInternalPoint(Vertices[i].Pos);
		}
	}

	//みんな呼ばれる。
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	//シーンノード自身を描画する。
	virtual void render()
	{
		u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
		IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 4, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}

	virtual const aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	//SMaterial型のメンバーの数かな
	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual SMaterial& getMaterial(u32 i)
	{
		return Material;
	}


};

int main() {
	E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == EDT_COUNT) {
		return 1;
	}

	IrrlichtDevice *device = createDevice(driverType, dimension2d<u32>(640, 480), 16, false);

	if (device == 0) {
		return 1;
	}

	device->setWindowCaption(L"Custom Scene Node");

	IVideoDriver* videoDriver = device->getVideoDriver();
	ISceneManager* sceneManager = device->getSceneManager();

	sceneManager->addCameraSceneNode(0, vector3df(0, -40, 0), vector3df(0, 0, 0));

	//どっちが直感的なんだろうね
	CSampleSceneNode *node = new CSampleSceneNode(sceneManager->getRootSceneNode(), sceneManager, 666);

	ISceneNodeAnimator* animator = sceneManager->createRotationAnimator(vector3df(0.8f, 0, 0.8f));

	if (animator) {
		node->addAnimator(animator);

		animator->drop();
		animator = 0;
	}

	//再度、参照してはいけない。
	node->drop();
	node = 0;

	while (device->run())
	{
		videoDriver->beginScene(true, true, video::SColor(0, 100, 100, 100));

		sceneManager->drawAll();

		videoDriver->endScene();
	}

	device->drop();


	return 0;
}