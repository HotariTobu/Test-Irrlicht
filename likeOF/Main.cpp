#include "IrrGame.h"

int main() {
	IrrGameHost host;
	host.fullscreen = false;
	host.caption = L"likeOF";
	host.start((IrrGameBase *)new IrrGame());

	return 0;
}