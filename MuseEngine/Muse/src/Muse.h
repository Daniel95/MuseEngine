#pragma once

//Core
#include "Core/Application.h"

//Gameplay
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/BoxCollider2D.h"
#include "Core/Gameplay/Component/TransformComponent.h"
#include "Core/Gameplay/Component/CameraComponent.h"
#include "Core/Gameplay/Component/RenderComponent.h"

//Input
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/Input/MouseButtonCodes.h"

//Scene
#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Scene.h"

//Resource
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource.h"

//Renderer
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Buffer/BufferLayout.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"
#include "Core/Renderer/VertexArray.h"

//Utilities
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "Core/Utilities/Utilities.h"

//Event
#include "Core/Event/Event.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"