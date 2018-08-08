// include the basic windows header file
#include <windows.h>
#include <windowsx.h>

#include "Animation.h"
#include "Audio.h"
#include "GameState.h"
#include "ObjectManager.h"
#include "Physics.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "UI.h"


Animation *animator = nullptr;
Audio *audio = nullptr;
GameState* gameState = nullptr;
ObjectManager* heirarchy = nullptr;
Physics* physics = nullptr;
Renderer* renderer = nullptr;
SceneManager* sceneManager = nullptr;
UI* canvas = nullptr;

// the handle for the window, filled by a function
HWND hWnd;
// this struct holds information for the window class
WNDCLASSEXW wc;

void Init(void);
void Shutdown(void);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassExW(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_POPUP, FALSE);

	// create the window and use the result as the handle
	hWnd = CreateWindowExW(NULL,
		L"WindowClass1",    // name of the window class
		L"Our First Windowed Program",   // title of the window
		WS_POPUP,    // window style
		10,    // x-position of the window
		10,    // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// enter the main loop:

	/*
	*	HERE IS WHERE WE INITIALIZE SYSTEMS
	*/
	Init();


	// this struct holds Windows event messages
	MSG msg = { 0 };

	// wait for the next message in the queue, store the result in 'msg'
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			gameState->Update();
			renderer->Update();
		}
	}

	Shutdown();

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:

			// Process the LEFT ARROW key. 
			
			break;

		case VK_RIGHT:

			// Process the RIGHT ARROW key. 

			break;

		case VK_UP:

			// Process the UP ARROW key. 

			break;

		case VK_DOWN:

			// Process the DOWN ARROW key. 

			break;

		case VK_HOME:

			// Process the HOME key. 

			break;

		case VK_END:

			// Process the END key. 

			break;

		case VK_INSERT:

			// Process the INS key. 

			break;

		case VK_DELETE:

			// Process the DEL key. 

			break;

		case VK_F2:

			// Process the F2 key. 

			break;


			// Process other non-character keystrokes. 

		default:
			break;
		}
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}


	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void Shutdown()
{
	animator->Shutdown();
	delete animator;
	audio->Shutdown();
	delete audio;
	gameState->Shutdown();
	delete gameState;
	heirarchy->Shutdown();
	delete heirarchy;
	physics->Shutdown();
	delete physics;
	renderer->Shutdown();
	delete renderer;
	sceneManager->Shutdown();
	delete sceneManager;
	canvas->Shutdown();
	delete canvas;

}

void Init()
{
	animator = new Animation();
	animator->Init();
	audio = new Audio();
	audio->Init();
	gameState = new GameState();
	gameState->Init();
	heirarchy = new ObjectManager();
	heirarchy->Init();
	physics = new Physics();
	physics->Init();
	renderer = new Renderer();
	renderer->Init(hWnd);
	sceneManager = new SceneManager();
	sceneManager->Init();
	canvas = new UI();
	canvas->Init();
}