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
	GUI_ID_QUIT_BUTTON,
};

class Event : public IEventReceiver{
public :
	Event(IrrlichtDevice *device) {
		this->device = device;
	}

	virtual bool OnEvent(const SEvent& event) {
		switch (event.EventType) {
		case EET_KEY_INPUT_EVENT :
			key[event.KeyInput.Key] = event.KeyInput.PressedDown;
			break;
		case EET_GUI_EVENT :
			s32 id = event.GUIEvent.Caller->getID();

			switch (event.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED :
				switch (id) {
				case GUI_ID_QUIT_BUTTON :
					device->closeDevice();
					break;
				}
				break;
			}
			break;
		}

		return false;
	}

	bool isKeyDown(EKEY_CODE code) {
		return key[code];
	}
private :
	IrrlichtDevice * device;

	bool key[KEY_KEY_CODES_COUNT]{};
};

int main() {
	IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

	core::dimension2d<u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice->drop();

	IrrlichtDevice *device = createDevice(EDT_OPENGL, deskres, 32, false, true, true, 0);

	if (!device) {
		return 1;
	}

	device->setWindowCaption(L"fullscreen");
	Event *recv = new Event(device);
	device->setEventReceiver((IEventReceiver *)recv);

	IVideoDriver *videoDriver = device->getVideoDriver();
	ISceneManager *sceneManager = device->getSceneManager();
	IGUIEnvironment *GUIEnvironment = device->getGUIEnvironment();

	IGUIButton *button = GUIEnvironment->addButton(recti(0, 0, 100, 100), 0, GUI_ID_QUIT_BUTTON, L"I—¹", L"I—¹‚µ‚Ü‚·B");
	if (button) {
		IGUIFont *font = GUIEnvironment->getFont("../Yu Gothic UI Light0.png");
		if (font) {
			button->setOverrideFont(font);
		}
	}

	while (device->run()) {
		if (device->isWindowActive()) {
			if (recv->isKeyDown(KEY_ESCAPE)) {
				device->closeDevice();
			}

			videoDriver->beginScene(true, false, SColor(200, 200, 200, 200));
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