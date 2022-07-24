#pragma once

#include "IrrGameHost.h"

class IrrGame : public IrrGameBase {
public :
	void init();
	void drop();

	void update();
	void draw();

	void gui(SEvent::SGUIEvent event);
	void joystick(SEvent::SJoystickEvent event);
	void keyPressed(EKEY_CODE code, bool control, bool shift);
	void keyReleased(EKEY_CODE code, bool control, bool shift);
	void log(SEvent::SLogEvent event);
	void mousePressed(s32 x, s32 y, EKEY_CODE code, bool control, bool shift);
	void mouseReleased(s32 x, s32 y, EKEY_CODE code, bool control, bool shift);
	void mouseDoubleClicked(s32 x, s32 y, EKEY_CODE code, bool control, bool shift);
	void mouseTripleClicked(s32 x, s32 y, EKEY_CODE code, bool control, bool shift);
	void mouseMoved(s32 x, s32 y);
	void mouseDraged(s32 width, s32 height, EKEY_CODE code, bool control, bool shift);
	void mouseWheel(f32 value);
	void user(SEvent::SUserEvent event);
};