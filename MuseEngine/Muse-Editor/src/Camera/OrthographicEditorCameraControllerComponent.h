#pragma once

namespace Muse
{
	struct OrthographicEditorCameraControllerComponent
	{
        float m_MoveSpeed = 5;
        float m_ScrollSpeed = 0.1;
        float m_MinZoomLevel = 0.1;
        float m_MaxZoomLevel = 10;
	};
}