#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

IMeshBuffer* createMeshBufferWithTangents(IMeshBuffer* meshBuffer, IMeshManipulator* mnp, bool recalculateNormals = false, bool smooth = false, bool angleWeighted = false, bool calculateTangents = true) {
	if (!meshBuffer) {
		return 0;
	}

	const IMeshBuffer* const original = meshBuffer;
	const u32 indexCount = original->getIndexCount();
	const u16* indices = original->getIndices();

	SMeshBufferTangents* buffer = new SMeshBufferTangents();

	buffer->Material = original->getMaterial();
	buffer->Vertices.reallocate(indexCount);
	buffer->Indices.reallocate(indexCount);

	core::map<video::S3DVertexTangents, int> verticesMap;
	int vertexLocation;

	const video::E_VERTEX_TYPE vertexType = original->getVertexType();
	video::S3DVertexTangents newVertex;
	for (u32 i = 0; i < indexCount; i++) {
		switch (vertexType) {
		case video::EVT_STANDARD:
		{
			const video::S3DVertex oldVertex = ((const video::S3DVertex*)original->getVertices())[indices[i]];
			newVertex = video::S3DVertexTangents(oldVertex.Pos, oldVertex.Normal, oldVertex.Color, oldVertex.TCoords);
		}
		break;

		case video::EVT_2TCOORDS:
		{
			const video::S3DVertex2TCoords oldVertex = ((const video::S3DVertex2TCoords*)original->getVertices())[indices[i]];
			newVertex = video::S3DVertexTangents(oldVertex.Pos, oldVertex.Normal, oldVertex.Color, oldVertex.TCoords);
		}
		break;

		case video::EVT_TANGENTS:
		{
			const video::S3DVertexTangents oldVertex = ((const video::S3DVertexTangents*)original->getVertices())[indices[i]];
			newVertex = oldVertex;
		}
		break;
		}

		core::map<video::S3DVertexTangents, int>::Node* node = verticesMap.find(newVertex);
		if (node) {
			vertexLocation = node->getValue();
		}
		else {
			vertexLocation = buffer->Vertices.size();
			buffer->Vertices.push_back(newVertex);
			verticesMap.insert(newVertex, vertexLocation);
		}

		buffer->Indices.push_back(vertexLocation);
	}

	buffer->recalculateBoundingBox();

	if (calculateTangents) {
		mnp->recalculateTangents(buffer, recalculateNormals, smooth, angleWeighted);
	}

	return buffer;
}

class CSceneNode : public ISceneNode {
public:
	CSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id) : ISceneNode(parent, mgr, id), meshBuffer(new SMeshBuffer()) {
		meshBuffer->Material.Wireframe = false;
		//meshBuffer->Material.Lighting = false;
		meshBuffer->Material.MaterialType = EMT_PARALLAX_MAP_SOLID;

		meshBuffer->Vertices.set_used(6);
		meshBuffer->Vertices[0] = S3DVertex(0, 0, 10, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1);
		meshBuffer->Vertices[1] = S3DVertex(10, 0, -10, 1, 0, 0, SColor(255, 255, 0, 255), 0.5f, 0);
		meshBuffer->Vertices[2] = S3DVertex(0, 20, 0, 0, 1, 1, SColor(255, 255, 255, 0), 0, 0);
		meshBuffer->Vertices[3] = S3DVertex(-10, 0, -10, 0, 0, 1, SColor(255, 0, 255, 0), 0.5f, 1);

		meshBuffer->Vertices[4] = S3DVertex(0, 0, 10, 1, 1, 0, SColor(255, 0, 255, 255), 1, 1);
		meshBuffer->Vertices[5] = S3DVertex(0, 20, 0, 0, 1, 1, SColor(255, 255, 255, 0), 1, 0);

		meshBuffer->Indices.set_used(12);
		u16 indices[] = { 2,3,0, 1,3,2, 1,4,3, 5,4,1 };
		for (s32 i = 0; i < 12; i++) {
			meshBuffer->Indices[i] = indices[i];
		}

		IMeshBuffer* newMeshBuffer = createMeshBufferWithTangents(meshBuffer, SceneManager->getMeshManipulator());
		meshBuffer->drop();
		meshBuffer = (SMeshBuffer*)newMeshBuffer;

		meshBuffer->BoundingBox.reset(meshBuffer->Vertices[0].Pos);
		for (s32 i = 1; i < 4; ++i) {
			meshBuffer->BoundingBox.addInternalPoint(meshBuffer->Vertices[i].Pos);
		}
	}

	virtual void OnRegisterSceneNode() {
		if (IsVisible) {
			SceneManager->registerNodeForRendering(this);
		}

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render() {
		IVideoDriver* drv = SceneManager->getVideoDriver();

		drv->setMaterial(meshBuffer->Material);
		drv->setTransform(ETS_WORLD, AbsoluteTransformation);
		drv->drawMeshBuffer(meshBuffer);
	}

	virtual const aabbox3d<f32>& getBoundingBox() const
	{
		return meshBuffer->BoundingBox;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual SMaterial& getMaterial(u32 i)
	{
		return meshBuffer->Material;
	}

private:
	SMeshBuffer* meshBuffer;
};

int main() {
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16, false);
	if (device == 0) {
		return 1;
	}

	IVideoDriver* drv = device->getVideoDriver();
	ISceneManager* mgr = device->getSceneManager();

	drv->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

	//mgr->addCameraSceneNode(0, vector3df(0, 0, -80), vector3df(0, 0, 0));
	mgr->addCameraSceneNodeFPS()->setPosition(vector3df(0, 0, -80));
	mgr->addLightSceneNode(0, vector3df(0, 0, -60));
	mgr->addSphereSceneNode(5.0f, 16, 0, 777, vector3df(0, 0, -60))->setMaterialFlag(EMF_LIGHTING, false);

	ITexture* tex = drv->getTexture("a.png");
	ITexture* HTM = drv->getTexture("ah.png");
	drv->makeNormalMapTexture(HTM);

	s32 nodeCount = 8;
	f32 rad = 2 * PI / nodeCount;
	for (s32 i = 0; i < nodeCount; i++) {
		CSceneNode *node = new CSceneNode(mgr->getRootSceneNode(), mgr, 666 + i);
		node->setPosition(vector3df(30 * cosf(rad*i), 30 * sinf(rad*i), 0));
		node->setMaterialTexture(0, tex);
		node->setMaterialTexture(1, HTM);

		ISceneNodeAnimator* animator = mgr->createRotationAnimator(vector3df(0.8f, 0, 0.3f));
		if (animator) {
			node->addAnimator(animator);
			animator->drop();
		}

		node->drop();
	}

	while (device->run()) {
		//drv->beginScene();
		drv->beginScene(true, true, SColor(0, 100, 100, 100));

		mgr->drawAll();

		SMaterial someMaterial;
		someMaterial.Lighting = false;
		drv->setMaterial(someMaterial);
		drv->setTransform(ETS_WORLD, IdentityMatrix);
		for (s32 i = 0; i < nodeCount; i++) {
			drv->draw3DLine(vector3df(0, 0, -60), mgr->getSceneNodeFromId(666 + i)->getPosition(), SColor(255, 255, 255, 0));
		}

		drv->endScene();
	}

	device->drop();

	return 0;
}

		/*SMesh* mesh = new SMesh();
		mesh->addMeshBuffer(meshBuffer);
		IMesh* newMesh = SceneManager->getMeshManipulator()->createMeshWithTangents(mesh);
		mesh->drop();
		meshBuffer = (SMeshBuffer*)newMesh->getMeshBuffer(0);*/

/*#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

int main() {
	IrrlichtDevice* device = createDevice(EDT_OPENGL);

	IVideoDriver* drv = device->getVideoDriver();
	ISceneManager* mgr = device->getSceneManager();

	drv->setTextureCreationFlag(ETCF_ALWAYS_32_BIT, true);

	ICameraSceneNode* camera = mgr->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(-10, 10, -10));
	device->getCursorControl()->setVisible(false);

	mgr->addLightSceneNode()->addAnimator(mgr->createFlyCircleAnimator());

#define e

#ifdef a
	path model = "room.3ds";
	path texture1 = "rockwall.jpg";
	path texture2 = "rockwall_height.bmp";
#endif
#ifdef b
	path model = "model.obj";
	path texture1 = "model.png";
	path texture2 = "model_h.png";
#endif
#ifdef c
	path model = "model.3ds";
	path texture1 = "model.png";
	path texture2 = "model_h.png";
#endif
#ifdef d
	path model = "model2.3ds";
	path texture1 = "model2.png";
	path texture2 = "model2_h.png";
#endif
#ifdef e
	path model = "room.3ds";
	path texture1 = "a.png";
	path texture2 = "ah.png";
#endif

	IAnimatedMesh* mesh = mgr->getMesh(model);
	mgr->getMeshManipulator()->makePlanarTextureMapping(mesh->getMesh(0), 0.003f);
	ISceneNode* node = mgr->addMeshSceneNode(mgr->getMeshManipulator()->createMeshWithTangents(mesh->getMesh(0)));
	node->setMaterialTexture(0,drv->getTexture(texture1));
	ITexture* map = drv->getTexture(texture2);
	drv->makeNormalMapTexture(map, 9.0f);
	node->setMaterialTexture(1, map);
	node->setMaterialType(EMT_PARALLAX_MAP_SOLID);
	//node->setMaterialFlag(EMF_LIGHTING, false);
	drv->setAmbientLight(SColorf(1.0f, 1.0f, 1.0f, 1.0f));
	//node->setScale(vector3df(10.0f, 10.0f, 10.0f));
	for (s32 i = 0; i < node->getMaterialCount(); i++) {
		node->getMaterial(i).ColorMaterial = ECM_NONE;
		SMaterial material = node->getMaterial(i);
		
	}

	while (device->run()) {
		drv->beginScene(true, true, SColor(255, 128, 128, 128));
		mgr->drawAll();
		drv->endScene();
	}

	device->drop();

	return 0;
}*/