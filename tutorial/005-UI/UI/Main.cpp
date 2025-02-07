#include <irrlicht.h>
#include <iostream>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

//イベントを保持する構造体
struct SAppContext {
	IrrlichtDevice *device;
	s32 counter;
	IGUIListBox *listbox;
};

//個々のコンポーネントの識別番号
enum {
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
};

//透明度を設定
void setSkinTransparency(s32 alpha, IGUISkin * skin) {
	for (s32 i = 0; i < EGDC_COUNT; ++i) {
		SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}

class eventReceiver : public IEventReceiver {
public:
	eventReceiver(SAppContext & context) : Context(context){}

	virtual bool OnEvent(const SEvent& event) {
		//GUIイベントかどうか
		if (event.EventType == EET_GUI_EVENT) {
			//イベントを呼んだコンポーネントのID
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			//どのGUIイベントか
			switch (event.GUIEvent.EventType) {
				//スクロールバーのイベント
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR) {
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					setSkinTransparency(pos, env->getSkin());
				}

				break;

				//ボタンのイベント
			case EGET_BUTTON_CLICKED:
				//IDで識別
				switch (id) {
					//終了ボタン
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

					//ウィンドウを新しく生成するボタン
				case GUI_ID_NEW_WINDOW_BUTTON:
				{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200) {
						Context.counter = 0;
					}

					IGUIWindow *window = env->addWindow(rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter), false, L"Test window");

					env->addStaticText(L"Please close me", rect<s32>(35, 35, 140, 50), true, false, window);
					return true;
				}

					//ファイルを開くボタン
				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;
				}

				break;

				//ファイルが選択されたら
			case EGET_FILE_SELECTED:
				IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
				Context.listbox->addItem(dialog->getFileName());

				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};

int main() {
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, true, true, 0);

	if (!device) {
		return 1;
	}

	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	device->getFileSystem()->changeWorkingDirectoryTo("irrlicht-1.8.4/media");

	//フォントを設定
	IGUISkin* skin = GUIEnvironment->getSkin();
	IGUIFont* font = GUIEnvironment->getFont("fonthaettenschweiler.bmp");
	if (font) {
		skin->setFont(font);
	}

	skin->setFont(GUIEnvironment->getBuiltInFont(), EGDF_TOOLTIP);

	//ボタンを追加
	GUIEnvironment->addButton(rect<s32>(10, 240, 110, 240 + 32), 0, GUI_ID_QUIT_BUTTON, L"Quit", L"Exits Program");
	GUIEnvironment->addButton(rect<s32>(10, 280, 110, 280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON, L"New Window", L"Launches a new Window");
	GUIEnvironment->addButton(rect<s32>(10, 320, 110, 320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON, L"File Open", L"Opens a file");

	//スクロールバーの設定
	GUIEnvironment->addStaticText(L"Transparent Control:", rect<s32>(150, 20, 350, 40), true);
	IGUIScrollBar* scrollbar = GUIEnvironment->addScrollBar(true, rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255);
	setSkinTransparency(scrollbar->getPos(), GUIEnvironment->getSkin());

	scrollbar->setPos(GUIEnvironment->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	GUIEnvironment->addStaticText(L"Logging ListBox:", rect<s32>(50, 110, 250, 130), true);
	IGUIListBox *listbox = GUIEnvironment->addListBox(rect<s32>(50, 140, 250, 210));
	GUIEnvironment->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	//イベントレシーバに渡す引数の設定
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	eventReceiver receiver(context);

	//イベントレシーバを追加
	device->setEventReceiver(&receiver);

	GUIEnvironment->addImage(videoDriver->getTexture("irrlichtlogo2.png"), position2d<int>(10, 10));

	while (device->run()) {
		if (device->isWindowActive()) {
			videoDriver->beginScene(true, true, SColor(0, 200, 200, 200));
			{
				GUIEnvironment->drawAll();
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