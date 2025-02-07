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
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false);

	if (!device) {
		return 1;
	}

	device->setWindowCaption(L"Irrlicht Engine - 2D Graphics Demo");
	device->setResizable(true);

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");

	//テクスチャの読み込み
	ITexture *images = videoDriver->getTexture("2ddemo.png");
	//アルファチャンネルの位置を教える
	videoDriver->makeColorKeyTexture(images, position2d<s32>(0, 0));

	//組み込みのフォント
	IGUIFont* font = GUIEnvironment->getBuiltInFont();
	//外部のフォント
	IGUIFont* font2 = GUIEnvironment->getFont("fonthaettenschweiler.bmp");

	rect<s32> imp1(349, 15, 385, 78);
	rect<s32> imp2(387, 15, 423, 78);

	/*バイリニアフィルタリング（ぼかす？）
	追記：2Dグラフィックのための設定*/
	videoDriver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	//アンチエイリアシング
	videoDriver->getMaterial2D().AntiAliasing = EAAM_FULL_BASIC;

	while (device->run()) {
		if (device->isWindowActive()) {
			u32 time = device->getTimer()->getTime();

			videoDriver->beginScene(true, true, SColor(0, 200, 200, 200));
			{
				// draw fire & dragons background world
				videoDriver->draw2DImage(images, position2d<s32>(50, 50), rect<s32>(0, 0, 342, 224), 0, SColor(255, 255, 255, 255), true);

				// draw flying imp
				videoDriver->draw2DImage(images, position2d<s32>(164, 125), (time / 500 % 2) ? imp1 : imp2, 0, SColor(255, 255, 255, 255), true);

				// draw second flying imp with colorcylce
				videoDriver->draw2DImage(images, position2d<s32>(270, 105), (time / 500 % 2) ? imp1 : imp2, 0, SColor(255, (time) % 255, 255, 255), true);

				//１つ目のフォントを使用
				if (font) {
					font->draw(L"This demo shows that Irrlicht is also capable of drawing 2D graphics.", rect<s32>(130, 10, 300, 50), SColor(255, 255, 255, 255));
				}

				//２つ目のフォントを使用
				if (font2) {
					font2->draw(L"Also mixing with 3d graphics is possible.", rect<s32>(130, 20, 300, 60), SColor(255, time % 255, time % 255, 255));
				}

				//ロゴ
				videoDriver->enableMaterial2D();
				videoDriver->draw2DImage(images, rect<s32>(10, 10, 108, 48), rect<s32>(354, 87, 442, 118));
				videoDriver->enableMaterial2D(false);

				//カーソルについてくる正方形
				position2d<s32> m = device->getCursorControl()->getPosition();
				videoDriver->draw2DRectangle(SColor(100, 255, 255, 255), rect<s32>(m.X - 20, m.Y - 20, m.X + 20, m.Y + 20));
			}
			videoDriver->endScene();
		}
		else {
			device->yield();
		}
	}

	device->drop();

	return 0;
}