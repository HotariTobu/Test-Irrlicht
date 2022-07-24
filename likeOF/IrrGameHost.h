#pragma once

#include "IrrGameBase.h"

class IrrGameHost : public IEventReceiver {
public :
	void start(IrrGameBase *game);

	virtual bool OnEvent(const SEvent& event);

	u32 bits = 32;
	dimension2du size = dimension2du(1200, 600);
	bool fullscreen = true;
	bool stencilbuffer = true;
	bool vsync = true;

	const wchar_t *caption;

private :
	s32 lastX, lastY;
	bool mouse[3];
};

extern void change(IrrGameBase *game);

extern IrrGameBase *game;

extern IrrlichtDevice *device;

extern ECOLOR_FORMAT colorFormat;
extern ICursorControl *cursorControl;
extern IFileSystem *fileSystem;
extern IGUIEnvironment *GUIEnvironment;
extern ILogger *logger;
extern IOSOperator *osOperator;
extern IRandomizer *randmizer;
extern ISceneManager *sceneManager;
extern ITimer *timer;
extern IVideoDriver *videoDriver;
extern IVideoModeList *videoModeList;