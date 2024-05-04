#pragma once

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

	// Main Loop
	void Exit();
protected:
	bool m_isExit = false;

private:
	BaseApp() = delete;
	BaseApp(const BaseApp&) = delete;
	BaseApp(BaseApp&&) = delete;
	BaseApp& operator=(const BaseApp&) = delete;
	BaseApp& operator=(BaseApp&&) = delete;

	BaseAppPrivate* m_baseAppPrivate = nullptr;
};