#pragma once

//Core
#include "Core/Application.h"

//Gameplay
#include "Core/Gameplay/GameObject.h"
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

//Todo:
//Return a distance in intersection functions to efficiently detect the first hit.
//Filter blocked lights by retrieving the indexes of the non blocked lights (with a local), instead of creating sending around an std::vector
//Fix Reflection
//Fix intersection plane
//calculate shadows by giving using a light area, take a random sample to one point of this light, and add the samples together
//give each ray a random offset, add the samples together to get anti aliasing
//Reset samples when moving the camera
//sqrtf the results to make add gamma correction
//Fix BVH
//Update/Remove raytracer camera class
//Make Material a resource

//to generate a bvh, use throw all the triangles of all the models in a big array with each triangle to a pointer to the material
//check dirty for transforms to only recalculate a part of an BVH
//refitting a BVH : resize the bounding boxes to match they children objects / leaves. (only works if the amount of objects are the same)

//Update Muse.h


