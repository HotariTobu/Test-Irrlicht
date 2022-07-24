#pragma once

#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class IrrGameBase {
public :
	virtual void init() = 0;
	virtual void drop() = 0;

	virtual void update() = 0;
	virtual void draw() = 0;

	virtual void gui(SEvent::SGUIEvent event) = 0;
	virtual void joystick(SEvent::SJoystickEvent event) = 0;
	virtual void keyPressed(EKEY_CODE code, bool control, bool shift) = 0;
	virtual void keyReleased(EKEY_CODE code, bool control, bool shift) = 0;
	virtual void log(SEvent::SLogEvent event) = 0;
	virtual void mousePressed(s32 x, s32 y, EKEY_CODE code, bool control, bool shift) = 0;
	virtual void mouseReleased(s32 x, s32 y, EKEY_CODE code, bool control, bool shift) = 0;
	virtual void mouseDoubleClicked(s32 x, s32 y, EKEY_CODE code, bool control, bool shift) = 0;
	virtual void mouseTripleClicked(s32 x, s32 y, EKEY_CODE code, bool control, bool shift) = 0;
	virtual void mouseMoved(s32 x, s32 y) = 0;
	virtual void mouseDraged(s32 width, s32 height, EKEY_CODE code, bool control, bool shift) = 0;
	virtual void mouseWheel(f32 value) = 0;
	virtual void user(SEvent::SUserEvent event) = 0;
};