#include "stdafx.h"
#include "BaseApp.h"
//-----------------------------------------------------------------------------
class BaseAppPrivate final
{
public:
	BaseAppPrivate(BaseApp* app) : m_app(app) { }

private:
	BaseApp* m_app;
};
//-----------------------------------------------------------------------------
void BaseApp::PrintLog(const std::string& message)
{
	puts(message.c_str());
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
void BaseApp::Exit()
{
	m_isExit = true;
}
//-----------------------------------------------------------------------------