#include "IrrGameHost.h"

IrrGameBase *game;

IrrlichtDevice *device;

ECOLOR_FORMAT colorFormat;
ICursorControl *cursorControl;
IFileSystem *fileSystem;
IGUIEnvironment *GUIEnvironment;
ILogger *logger;
IOSOperator *osOperator;
IRandomizer *randmizer;
ISceneManager *sceneManager;
ITimer *timer;
IVideoDriver *videoDriver;
IVideoModeList *videoModeList;

void IrrGameHost::start(IrrGameBase *game) {
	if (!game) {
		return;
	}

	::game = game;

	if (fullscreen) {
		IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

		if (!nulldevice) {
			return;
		}

		size = nulldevice->getVideoModeList()->getDesktopResolution();

		nulldevice->drop();
	}

	device = createDevice(EDT_OPENGL, size, bits, fullscreen, stencilbuffer, vsync, (IEventReceiver *)this);

	if (!device) {
		return;
	}

	device->setWindowCaption(caption);

	colorFormat = device->getColorFormat();
	cursorControl = device->getCursorControl();
	fileSystem = device->getFileSystem();
	GUIEnvironment = device->getGUIEnvironment();
	logger = device->getLogger();
	osOperator = device->getOSOperator();
	randmizer = device->getRandomizer();
	sceneManager = device->getSceneManager();
	timer = device->getTimer();
	videoDriver = device->getVideoDriver();
	videoModeList = device->getVideoModeList();

	::game->init();

	while (device->run()) {
		if (device->isWindowActive()) {
			::game->update();

			videoDriver->beginScene();
			::game->draw();
			videoDriver->endScene();
		}
		else {
			device->yield();
		}
	}

	::game->drop();

	device->drop();
}

void change(IrrGameBase *game) {
	::game->drop();

	if (!game) {
		return;
	}

	::game = game;

	game->init();
}

bool IrrGameHost::OnEvent(const SEvent &event) {
	EKEY_CODE code;
	bool control, shift;
	s32 x, y, width, height;

	switch (event.EventType) {
	case EET_GUI_EVENT :
		game->gui(event.GUIEvent);		
		break;
	case EET_JOYSTICK_INPUT_EVENT :
		game->joystick(event.JoystickEvent);
		break;
	case EET_KEY_INPUT_EVENT :
		code = event.KeyInput.Key;
		control = event.KeyInput.Control;
		shift = event.KeyInput.Shift;
		if (event.KeyInput.PressedDown) {
			game->keyPressed(code, control, shift);
		}
		else {
			game->keyReleased(code, control, shift);
		}
		break;
	case EET_LOG_TEXT_EVENT :
		game->log(event.LogEvent);
		break;
	case EET_MOUSE_INPUT_EVENT :
		x = event.MouseInput.X;
		y = event.MouseInput.Y;
		control = event.MouseInput.Control;
		shift = event.MouseInput.Shift;

		switch (event.MouseInput.Event) {
		case EMIE_LMOUSE_DOUBLE_CLICK :
			game->mouseDoubleClicked(x, y, KEY_LBUTTON, control, shift);
			break;
		case EMIE_LMOUSE_LEFT_UP :
			game->mouseReleased(x, y, KEY_LBUTTON, control, shift);
			mouse[0] = false;
			break;
		case EMIE_LMOUSE_PRESSED_DOWN :
			game->mousePressed(x, y, KEY_LBUTTON, control, shift);
			mouse[0] = true;
			break;
		case EMIE_LMOUSE_TRIPLE_CLICK :
			game->mouseTripleClicked(x, y, KEY_LBUTTON, control, shift);
			break;
		case EMIE_MMOUSE_DOUBLE_CLICK:
			game->mouseDoubleClicked(x, y, KEY_MBUTTON, control, shift);
			break;
		case EMIE_MMOUSE_LEFT_UP:
			game->mouseReleased(x, y, KEY_MBUTTON, control, shift);
			mouse[2] = false;
			break;
		case EMIE_MMOUSE_PRESSED_DOWN:
			game->mousePressed(x, y, KEY_MBUTTON, control, shift);
			mouse[2] = true;
			break;
		case EMIE_MMOUSE_TRIPLE_CLICK:
			game->mouseTripleClicked(x, y, KEY_MBUTTON, control, shift);
			break;
		case EMIE_RMOUSE_DOUBLE_CLICK:
			game->mouseDoubleClicked(x, y, KEY_RBUTTON, control, shift);
			break;
		case EMIE_RMOUSE_LEFT_UP:
			game->mouseReleased(x, y, KEY_RBUTTON, control, shift);
			mouse[1] = false;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			game->mousePressed(x, y, KEY_RBUTTON, control, shift);
			mouse[1] = true;
			break;
		case EMIE_RMOUSE_TRIPLE_CLICK:
			game->mouseTripleClicked(x, y, KEY_RBUTTON, control, shift);
			break;
		case EMIE_MOUSE_MOVED :
			game->mouseMoved(x, y);

			width = x - lastX;
			height = y - lastY;

			if (mouse[0]) {
				game->mouseDraged(width, height, KEY_LBUTTON, control, shift);
			}

			if (mouse[1]) {
				game->mouseDraged(width, height, KEY_RBUTTON, control, shift);
			}

			if (mouse[2]) {
				game->mouseDraged(width, height, KEY_MBUTTON, control, shift);
			}

			lastX = x;
			lastY = y;
			break;
		case EMIE_MOUSE_WHEEL :
			game->mouseWheel(event.MouseInput.Wheel);
			break;
		}
		break;
	case EET_USER_EVENT :
		game->user(event.UserEvent);
		break;
	}

	return false;
}