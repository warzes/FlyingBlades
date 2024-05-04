#pragma once

struct WindowSystemCreateInfo final
{
	int width = 1024;
	int height = 768;
	const wchar_t* title = L"Game";
	bool fullscreen = false;
};

class BaseAppPrivate;

class BaseApp
{
	friend BaseAppPrivate;
public:
	virtual ~BaseApp() = default;

	// Filesystem

	// Log system
	void PrintLog(const std::string& message);
	void Warning(const std::string& message);
	void Error(const std::string& message);
	void Fatal(const std::string& message);

	// Time system

	// Event system

	// Input system

	// Window system
	uint32_t GetFrameWidth() const;
	uint32_t GetFrameHeight() const;
	float GetFrameAspect() const;
	HWND GetHWND() const;
	HINSTANCE GetHINSTANCE() const;

	// Main Loop
	void Exit();
	bool IsEnd() const;

protected:
	BaseApp() = default;

	bool initBaseApp(const WindowSystemCreateInfo& createInfo);
	void closeBaseApp();
	bool peekMessage();

private:
	BaseAppPrivate* m_baseAppPrivate = nullptr;
	HINSTANCE m_hInstance = nullptr;
	HWND m_hwnd = nullptr;
	MSG m_msg{};
	uint32_t m_frameWidth = 0;
	uint32_t m_frameHeight = 0;
	bool m_isFullscreen = false;
	bool m_isExit = false;
};