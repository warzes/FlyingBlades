#include "stdafx.h"
#include "BaseApp.h"
//-----------------------------------------------------------------------------
#pragma comment(lib,"runtimeobject.lib")
//-----------------------------------------------------------------------------
constexpr const wchar_t* ClassName = L"EngineApp";
//-----------------------------------------------------------------------------
class BaseAppPrivate final
{
public:
	BaseAppPrivate(BaseApp* app) : app(app) { }

	void OnWindowSizeChanged(uint32_t width, uint32_t height) 
	{
		app->sizeChanged(width, height);
	}

	bool& AppPaused() { return app->m_paused; }
	GameTimer& Timer() { return app->m_timer; }
	bool& Minimized() { return app->m_minimized; }
	bool& Maximized() { return app->m_maximized; }
	bool& Resizing() { return app->m_resizing; }

	BaseApp* app;
};
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool s_in_sizemove = false;
	static bool s_in_suspend = false;
	static bool s_minimized = false;
	static bool s_fullscreen = false;
	auto app = reinterpret_cast<BaseAppPrivate*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	// TODO: вместо постоянных проверок app is null сделать данную проверку один раз, а дальше их поубирать.


	switch (message)
	{
	case WM_CREATE:
		if (lParam)
		{
			auto params = reinterpret_cast<LPCREATESTRUCTW>(lParam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(params->lpCreateParams));
		}
		break;
	case WM_CLOSE:
		//EngineExit();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		if (s_in_sizemove && app)
		{
			//app->Tick();
		}
		else
		{
			PAINTSTRUCT ps;
			std::ignore = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		}
		break;
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			// Implements the classic ALT+ENTER fullscreen toggle
			if (s_fullscreen)
			{
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, 0);

				int width = 800;
				int height = 600;
				//if (app)
				//	app->GetDefaultSize(width, height);

				ShowWindow(hwnd, SW_SHOWNORMAL);

				SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			else
			{
				SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
				SetWindowLongPtr(hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

				SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowWindow(hwnd, SW_SHOWMAXIMIZED);
			}

			s_fullscreen = !s_fullscreen;
		}
		break;

	case WM_MENUCHAR:
		// A menu is active and the user presses a key that does not correspond to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_KEYDOWN:
		//pPlatformApp->OnKeyPressed((uint32_t)wParam);
		break;
	case WM_KEYUP:
		//pPlatformApp->OnKeyUp((uint32_t)wParam);
		break;
	case WM_LBUTTONDOWN:

		if (app) app->app->OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		if (app) app->app->OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MBUTTONDOWN:
		if (app) app->app->OnMouseDown(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		if (app) app->app->OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONUP:
		if (app) app->app->OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MBUTTONUP:
		if (app) app->app->OnMouseUp(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		//	short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_MOUSEMOVE:
		if (app) app->app->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			if (app)
			{
				app->AppPaused() = true;
				app->Minimized() = true;
				app->Maximized() = false;
			}
			if (!s_minimized)
			{
				s_minimized = true;
		//		if (!s_in_suspend && app)
		//			app->OnSuspending();
				s_in_suspend = true;
			}
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			if (app)
			{
				app->AppPaused() = false;
				app->Minimized() = false;
				app->Maximized() = true;
				RECT rc;
				GetClientRect(hwnd, &rc);
				app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
			}
			if (s_minimized)
			{
				s_minimized = false;
				//	if (s_in_suspend && app)
				//		app->OnResuming();
				s_in_suspend = false;
			}
		}
		else if (wParam == SIZE_RESTORED)
		{
			if (app)
			{
				// Restoring from minimized state?
				if (app->Minimized())
				{
					app->AppPaused() = false;
					app->Minimized() = false;
					RECT rc;
					GetClientRect(hwnd, &rc);
					app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
				}
				// Restoring from maximized state?
				else if (app->Maximized())
				{
					app->AppPaused() = false;
					app->Maximized() = false;
					RECT rc;
					GetClientRect(hwnd, &rc);
					app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
				}
			}
			if (s_minimized)
			{
				s_minimized = false;
				//	if (s_in_suspend && app)
				//		app->OnResuming();
				s_in_suspend = false;
			}
		}
		else if (!s_in_sizemove && app && !app->Resizing())
		{
			RECT rc;
			GetClientRect(hwnd, &rc);
			app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);
		minMaxInfo->ptMinTrackSize.x = 320;
		minMaxInfo->ptMinTrackSize.y = 320;
		break;
	}
	case WM_ENTERSIZEMOVE:
		s_in_sizemove = true;
		if (app)
		{
			app->AppPaused() = true;
			app->Resizing() = true;
			app->Timer().Stop();
		}
		break;
	case WM_EXITSIZEMOVE:
		s_in_sizemove = false;
		if (app)
		{
			app->AppPaused() = false;
			app->Resizing() = false;
			app->Timer().Start();
			RECT rc;
			GetClientRect(hwnd, &rc);
			app->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
		}
		break;
	// WM_ACTIVATE is sent when the window is activated or deactivated. We pause the game when the window is deactivated and unpause it when it becomes active.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			if (app)
			{
				app->AppPaused() = true;
				app->Timer().Stop();
			}
		}
		else
		{
			if (app)
			{
				app->AppPaused() = false;
				app->Timer().Start();
			}
		}
		return 0;


	case WM_ACTIVATEAPP:
		if (app)
		{
			if (wParam)
			{
				//app->OnActivated();
			}
			else
			{
				//app->OnDeactivated();
			}
		}
		break;

	case WM_POWERBROADCAST:
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			//if (!s_in_suspend && app)
			//	app->OnSuspending();
			s_in_suspend = true;
			return TRUE;

		case PBT_APMRESUMESUSPEND:
			if (!s_minimized)
			{
				//if (s_in_suspend && app)
				//	app->OnResuming();
				s_in_suspend = false;
			}
			return TRUE;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
void BaseApp::PrintLog(const std::string& message)
{
	puts(message.c_str());
}
//-----------------------------------------------------------------------------
void BaseApp::PrintLog(const std::wstring& message)
{
	_putws(message.c_str());
}
//-----------------------------------------------------------------------------
void BaseApp::Warning(const std::string& message)
{
	PrintLog("WARNING: " + message);
}
//-----------------------------------------------------------------------------
void BaseApp::Error(const std::string& message)
{
	PrintLog("ERROR: " + message);
}
//-----------------------------------------------------------------------------
void BaseApp::Fatal(const std::string& message)
{
	PrintLog("FATAL: " + message);
	Exit();
}
//-----------------------------------------------------------------------------
uint32_t BaseApp::GetFrameWidth() const
{
	return m_frameWidth;
}
//-----------------------------------------------------------------------------
uint32_t BaseApp::GetFrameHeight() const
{
	return m_frameHeight;
}
//-----------------------------------------------------------------------------
float BaseApp::GetFrameAspectRatio() const
{
	assert(m_frameHeight > 0);
	return (float)m_frameWidth / (float)m_frameHeight;
}
//-----------------------------------------------------------------------------
HWND BaseApp::GetHWND() const
{
	return m_hwnd;
}
//-----------------------------------------------------------------------------
HINSTANCE BaseApp::GetHINSTANCE() const
{
	return m_hInstance;
}
//-----------------------------------------------------------------------------
void BaseApp::Exit()
{
	m_isExit = true;
}
//-----------------------------------------------------------------------------
bool BaseApp::IsEnd() const
{
	return m_isExit;
}
//-----------------------------------------------------------------------------
bool BaseApp::initBaseApp(const WindowSystemCreateInfo& createInfo)
{
	if (!DirectX::XMVerifyCPUSupport())
		return false;
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		return false;

	assert(!m_baseAppPrivate);
	m_baseAppPrivate = new BaseAppPrivate(this);

	m_hInstance = GetModuleHandle(nullptr);
	m_isFullscreen = createInfo.fullscreen;
	m_title = createInfo.title;

	// Register class
	WNDCLASSEX wndClass{};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wndClass.lpszClassName = ClassName;
	if (!RegisterClassEx(&wndClass))
	{
		Fatal("RegisterClassEx failed.");
		return false;
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwExStyle;
	DWORD dwStyle;

	if (m_isFullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	}

	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = createInfo.fullscreen ? (long)screenWidth : (long)createInfo.width;
	windowRect.bottom = createInfo.fullscreen ? (long)screenHeight : (long)createInfo.height;

	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	m_hwnd = CreateWindowEx(0, ClassName, m_title.c_str(), dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, m_hInstance, m_baseAppPrivate);

	if (!m_hwnd)
	{
		Fatal("Could not create window!");
		return false;
	}

	if (!m_isFullscreen)
	{
		// Center on screen
		uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right) / 2;
		uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom) / 2;
		SetWindowPos(m_hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	GetClientRect(m_hwnd, &windowRect);
	m_frameWidth = windowRect.right - windowRect.left;
	m_frameHeight = windowRect.bottom - windowRect.top;

	m_timer.Reset();

	return true;
}
//-----------------------------------------------------------------------------
void BaseApp::closeBaseApp()
{
	if (m_hwnd)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = nullptr;
	}
	if (m_hInstance)
	{
		UnregisterClass(ClassName, m_hInstance);
		m_hInstance = nullptr;
	}

	delete m_baseAppPrivate;
	m_baseAppPrivate = nullptr;
}
//-----------------------------------------------------------------------------
bool BaseApp::peekMessage()
{
	if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
			Exit();

		return false;
	}
	else
	{
		m_timer.Tick();

		if (!m_paused)
		{
			calculateFrameStats();
			return true;
		}
		else
		{
			Sleep(100);
			return false;
		}
	}
}
//-----------------------------------------------------------------------------
void BaseApp::sizeChanged(uint32_t width, uint32_t height)
{
	m_frameWidth = width;
	m_frameHeight = height; // TODO: а размеры тут учитывают размер рамок окна?
	OnWindowSizeChanged();
}
//-----------------------------------------------------------------------------
void BaseApp::calculateFrameStats()
{
	// Code computes the average frames per second, and also the  average time it takes to render one frame. These stats are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);

		std::wstring windowText = m_title +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;

		SetWindowText(m_hwnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}
//-----------------------------------------------------------------------------