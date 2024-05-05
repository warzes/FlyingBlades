#include "stdafx.h"
#include "EngineApp.h"
//-----------------------------------------------------------------------------
EngineApp::EngineApp()
{
}
//-----------------------------------------------------------------------------
EngineApp::~EngineApp()
{
}
//-----------------------------------------------------------------------------
void EngineApp::Run()
{
	if (create())
	{
		if (OnCreate())
		{
			while (!IsEnd())
			{
				tick();
			}
			OnDestroy();
		}
	}
}
//-----------------------------------------------------------------------------
bool EngineApp::create()
{
	EngineCreateInfo createInfo = GetCreateInfo();

	if (!initBaseApp(createInfo.window))
		return false;

	if (!initRenderApp(createInfo.render))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void EngineApp::tick()
{
	if (!peekMessage()) return;

	OnUpdate(0.001f);
	OnFrame();
}
//-----------------------------------------------------------------------------
void EngineApp::destroy()
{
	closeRenderApp();
	closeBaseApp();
}
//-----------------------------------------------------------------------------