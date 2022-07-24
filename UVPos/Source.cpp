#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CSampleSceneNode : public ISceneNode {
public:
	CSampleSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id) {
		Material.Wireframe = false;
		Material.Lighting = false;
		Material.BackfaceCulling = false;

		Vertices[0] = S3DVertex(-10, 10, 0, -1, 1, 0, SColor(255, 255, 255, 255), 0, 0);
		Vertices[1] = S3DVertex(-10, -10, 0, -1, -1, 0, SColor(255, 255, 255, 255), 0, 1);
		Vertices[2] = S3DVertex(10, -10, 0, 1, -1, 0, SColor(255, 255, 255, 255), 1, 1);
		Vertices[3] = S3DVertex(10, 10, 0, 0, 1, 0, SColor(255, 255, 255, 255), 1, 0);

		Box.reset(Vertices[0].Pos);
		for (s32 i = 1; i < 4; ++i) {
			Box.addInternalPoint(Vertices[i].Pos);
		}
	}

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible) {
			SceneManager->registerNodeForRendering(this);
		}

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		//u16 indices[] = { 0,1,2, 0,2,3 };
		//u16 indices[] = { 0,2,1, 0,3,2 };
		//u16 indices[] = { 0,1,3, 1,2,3 };
		//u16 indices[] = { 0,3,1, 1,3,2 };
		//u16 indices[] = { 3,1,0, 2,3,1 };
		//...
		u16 indices[] = { 2,1,3, 3,0,1 };
		IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, EVT_STANDARD, EPT_TRIANGLES, EIT_16BIT);
	}

	virtual const aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual SMaterial& getMaterial(u32 i)
	{
		return Material;
	}

private:
	aabbox3d<f32> Box;
	S3DVertex Vertices[4];
	SMaterial Material;
};

int main() {
	IrrlichtDevice *device = createDevice(EDT_OPENGL);

	if (device == 0) {
		return 1;
	}

	device->setWindowCaption(L"UVPos test");

	IVideoDriver* videoDriver = device->getVideoDriver();
	ISceneManager* sceneManager = device->getSceneManager();

	sceneManager->addCameraSceneNode(0, vector3df(0, 0, -40), vector3df(0, 0, 0));

	CSampleSceneNode *node = new CSampleSceneNode(sceneManager->getRootSceneNode(), sceneManager, 666);

	ISceneNodeAnimator* animator = sceneManager->createRotationAnimator(vector3df(0, 0.1f, 0));
	if (animator) {
		node->addAnimator(animator);
		animator->drop();
	}

	//ITexture* texture = videoDriver->getTexture("texture.png");
	//ITexture* texture = videoDriver->getTexture("texture1.png");
	//ITexture* texture = videoDriver->getTexture("texture2.png");
	//ITexture* texture = videoDriver->getTexture("texture3.png");
	//ITexture* texture = videoDriver->getTexture("texture4.png");
	ITexture* texture = videoDriver->getTexture("texture5.png");
	if (texture) {
		node->setMaterialTexture(0, texture);
	}

	bool first = true;

	while (device->run())
	{
		videoDriver->beginScene(true, true, video::SColor(0, 100, 100, 100));

		sceneManager->drawAll();

		videoDriver->endScene();

		if (first) {
			first = false;

			IImage* scsh = videoDriver->createScreenShot();
			if (scsh) {
				videoDriver->writeImageToFile(scsh, "result.png");
			}
			else {
				device->getGUIEnvironment()->getBuiltInFont()->draw("screen shot failed", recti(10, 10, 200, 30), SColor(255, 255, 255, 255));
			}
		}
	}

	device->drop();

	return 0;
}

/*
		Material.MaterialType = EMT_DETAIL_MAP;
		Material.MaterialType = EMT_LIGHTMAP;
		Material.MaterialType = EMT_LIGHTMAP_ADD;
		Material.MaterialType = EMT_LIGHTMAP_LIGHTING_M2;
		Material.MaterialType = EMT_LIGHTMAP_LIGHTING_M4;
		Material.MaterialType = EMT_LIGHTMAP_M2;
		Material.MaterialType = EMT_LIGHTMAP_M4;
		Material.MaterialType = EMT_NORMAL_MAP_SOLID;
		Material.MaterialType = EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR;
		Material.MaterialType = EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA;
		Material.MaterialType = EMT_ONETEXTURE_BLEND;
		Material.MaterialType = EMT_PARALLAX_MAP_SOLID;
		Material.MaterialType = EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR;
		Material.MaterialType = EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA;
		Material.MaterialType = EMT_REFLECTION_2_LAYER;
		Material.MaterialType = EMT_SOLID;
		Material.MaterialType = EMT_SOLID_2_LAYER;
		Material.MaterialType = EMT_SPHERE_MAP;
		Material.MaterialType = EMT_TRANSPARENT_ADD_COLOR;
		Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
		Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		Material.MaterialType = EMT_TRANSPARENT_REFLECTION_2_LAYER;
		Material.MaterialType = EMT_TRANSPARENT_VERTEX_ALPHA;
		*/