#pragma once

#include "RenderApp.h"

struct EngineCreateInfo final
{
	WindowSystemCreateInfo window;
};

class EngineApp : public RenderApp
{
public:
	EngineApp();
	virtual ~EngineApp();

	void Run();

	virtual EngineCreateInfo GetCreateInfo() const { return {}; }
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnFrame() = 0;

private:
	bool create();
	void tick();
	void destroy();
};