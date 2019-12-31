#pragma once

#include "Core/Layer/Layer.h"

namespace Muse {
    class Scene;
}

class Game : public Muse::Layer
{
public:
	Game();
	virtual ~Game() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(float a_DeltaTime) override;
	virtual void OnImGuiRender() override;

private:

};
