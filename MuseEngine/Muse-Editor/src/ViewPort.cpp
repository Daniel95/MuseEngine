#include "MusePCH.h"
#include "ViewPort.h"
#include "imgui.h"

namespace Muse
{
	void ViewPort::Render(uint32_t a_ColorAttachmentRendererId, glm::vec2& a_ViewPortSize, float& a_AspectRatio)
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

		a_ViewPortSize = { viewportSize.x, viewportSize.y };
		a_AspectRatio = viewportSize.x / viewportSize.y;

		ImGui::Image((void*)a_ColorAttachmentRendererId, viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();
	}
}
