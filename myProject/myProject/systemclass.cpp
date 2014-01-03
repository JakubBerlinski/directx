#include "systemclass.h"

systemClass::systemClass() {
	m_input = 0;
	m_graphics = 0;
}

systemClass::systemClass(const systemClass& other) {

}

systemClass::~systemClass() {

}

bool systemClass::initialize() {
	int screenWidth, screenHeight;
	bool result;

	//initialize screen size
	screenWidth = 0;
	screenHeight = 0;

	//initialize windows api
	initializeWindows(screenWidth, screenHeight);

	//create input object which handles reading the keyboard input from user
	m_input = new inputClass;
	if(!m_input) {
		return false;
	}

	//initialize input object
	m_input->initialize();

	//create graphics object which hangles rendering the graphics for the application
	m_graphics = new graphicsClass;
	if(!m_graphics) {
		return false;
	}

	//initialize the graphics object
	result = m_graphics->initialize(screenWidth, screenHeight, m_hwnd);
	if(!result) {
		return false;
	}

	return true;
}

void systemClass::shutdown() {
	//release the graphics object
	if(m_graphics) {
		m_graphics->shutdown();
		delete m_graphics;
		m_graphics = 0;
	}

	//release the input object
	if(m_input) {
		delete m_input;
		m_input = 0;
	}

	//shutdown the window
	shutdownWindows();

	return;
}

void systemClass::run() {
	MSG msg;
	bool done, result;

	//initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//loop until quit message from window or user
	done = false;
	while(!done) {
		//handle the windows messages
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if windows signals to end the application then quit
		if(msg.message == WM_QUIT) {
			done = true;
		}
		else {
			//otherwise do frame processing
			result = frame();
			if(!result) {
				done = true;
			}
		}
	}

	return;
}

bool systemClass::frame() {
	bool result;

	//check if the user pressed escape
	if(m_input->isKeyDown(VK_ESCAPE)) {
		return false;
	}

	//do frame processing for the graphics object
	result = m_graphics->frame();
	if(!result) {
		return false;
	}

	return true;
}

LRESULT CALLBACK systemClass::messegeHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch(umsg) {
		//check if key pressed on keyboard
		case WM_KEYDOWN:
			//if a key is pressed send it to the input object so it can record that state
			m_input->keyDown((unsigned int)wparam);
			return 0;
		
		// check if key is released
		case WM_KEYUP:
			//if a key is released send it to the input object so it can unset that state
			m_input->keyUp((unsigned int)wparam);
			return 0;

		//send other messeges to the default message handler because they are unused
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void systemClass::initializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//get an external pointer to thie object
	applicationHandle = this;

	//get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//give the application a name
	m_applicationName = L"Engine";

	//setup the windows class with default settings
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//register the window class
	RegisterClassEx(&wc);

	//determine the resolution of the clients desktop
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//setup the screen settings depending on whether it is running in full or windowed mode
	if(FULL_SCREEN) {
		//if full set the screen to maximum size of users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields =	DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//set the position of the window to the top left corner
		posX = posY = 0;
	}
	else {
		//if windowed then set to 800x600
		screenWidth = 800;
		screenHeight = 600;

		//place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//create the window with screen settings and get handle to it
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//bring window up on screen and set as main focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//hide mouse
	ShowCursor(false);

	return;
}

void systemClass::shutdownWindows() {
	//show the mouse cursor
	ShowCursor(true);

	//fix the displat settings if leaving full screen
	if(FULL_SCREEN) {
		ChangeDisplaySettings(NULL, 0);
	}

	//remove window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//remove the application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//release pointer to the class
	applicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch(umessage) {
		//check if the window is being destroyed
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		//check if the window is being closed
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

		//all other messages pass to the message handler in the system class
		default:
			return applicationHandle->messegeHandler(hwnd, umessage, wparam, lparam);
	}
}