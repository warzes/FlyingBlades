#pragma once

class GameApp final : public EngineApp
{
public:
	virtual EngineCreateInfo GetCreateInfo() const final;
	bool OnCreate() final;
	void OnDestroy() final;
	void OnUpdate(float deltaTime)  final;
	void OnFrame() final;
};