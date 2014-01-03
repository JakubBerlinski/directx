#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "inputclass.h"
#include "graphicsclass.h"

class systemClass{
public:
	systemClass();
	systemClass(const systemClass&);
	~systemClass();
	
	bool initialize();
	void shutdown();
	void run();

	LRESULT CALLBACK messegeHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool frame();
	void initializeWindows(int&, int&);
	void shutdownWindows();

	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	inputClass* m_input;
	graphicsClass* m_graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static systemClass* applicationHandle = 0;

#endif
