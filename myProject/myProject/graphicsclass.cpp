#include "graphicsclass.h"

graphicsClass::graphicsClass() {
	m_D3D = 0;
}

graphicsClass::graphicsClass(const graphicsClass& other) {

}

graphicsClass::~graphicsClass() {

}

bool graphicsClass::initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;

	//create D3D object
	m_D3D = new d3dClass;
	if(!m_D3D) {
		return false;
	}

	//initialize D3D object
	result = m_D3D->initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void graphicsClass::shutdown() {
	if(m_D3D) {
		m_D3D->shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool graphicsClass::frame() {
	bool result;

	//render graphics scene
	result = render();
	if(!result) {
		return false;
	}

	return true;
}

bool graphicsClass::render() {
	//clear buffers to begin scene
	m_D3D->beginScene(0.5f, 0.5f, 0.5f, 1.0f);

	//present rendered scene to the screen
	m_D3D->endScene();

	return true;
}