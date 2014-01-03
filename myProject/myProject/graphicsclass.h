#ifndef _GRAPHICSCLASS_H_
#define _GRAPHCISCLASS_H_

#include <Windows.h>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class graphicsClass {
public:
	graphicsClass();
	graphicsClass(const graphicsClass&);
	~graphicsClass();

	bool initialize(int, int, HWND);
	void shutdown();
	bool frame();

private:
	bool render();
};

#endif