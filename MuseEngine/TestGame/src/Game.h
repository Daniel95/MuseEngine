#pragma once

#include "Core/Layer/Layer.h"
#include "Core/Timestep.h"

class Game : public Muse::Layer
{
public:
	Game();
	virtual ~Game() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Muse::Timestep ts) override;
	virtual void OnImGuiRender() override;

};
