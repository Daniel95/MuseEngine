﻿#include "MusePCH.h"
#include "ViewPort.h"
#include "imgui.h"
#include "Core/Gameplay/Component/CameraComponent.h"

namespace Muse
{
	void ViewPort::Render(uint32_t a_ColorAttachmentRendererId)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		/*
		float posX = ImGui::GetCursorScreenPos().x;
		float posY = ImGui::GetCursorScreenPos().y;
		glm::vec2 windowPosition = Get().GetWindow().GetWindowPosition();
		posX -= windowPosition.x;
		posY -= windowPosition.y;
		//LOG_ENGINE_INFO("{0}, {1}", posX, posY);
		*/

		auto viewportSize = ImGui::GetContentRegionAvail();
		const float aspectRatio = viewportSize.x / viewportSize.y;
		CameraComponent::GetMain()->SetProjection(aspectRatio, CameraComponent::GetMain()->GetZoomLevel());

		ImGui::Image((void*)a_ColorAttachmentRendererId, viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}

}
