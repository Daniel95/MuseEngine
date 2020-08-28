#pragma once

//Core
#include "Core/Application.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Window.h"

//Gameplay
#include "Core/Gameplay/GameObject.h"
#include "Core/Gameplay/Component/TransformComponentOld.h"
#include "Core/Gameplay/Component/CameraComponentOld.h"
#include "Core/Gameplay/Component/RenderComponent.h"

//Input
#include "Core/Input/KeyCodes.h"
#include "Core/Input/Input.h"
#include "Core/Input/MouseButtonCodes.h"

//Scene
#include "Core/Scene/SceneManagerOld.h"
#include "Core/Scene/SceneOld.h"

//Resource
#include "Core/Resource/ResourceManager.h"
#include "Core/Resource/Resource.h"

//Renderer
#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/SubTexture2D.h"
#include "Core/Renderer/Buffer/BufferLayout.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"
#include "Core/Renderer/Buffer/FrameBuffer.h"
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

//ECS
#include "Core/ECS/Job/Job.h"
#include "Core/ECS/Job/JobManager.h"
#include "Core/ECS/Job/Render2DJob.h"
#include "Core/ECS/Component/Render2DComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/TransformHelper.h"

//Camera


//Todo:
/////Engine:
//Fix Crash in Game2D
//Translate Rasterizer to its own folder in Renderer
//Fix warnings
//Model loading
//GLM make vec's and mat's conversion to string
//Transform support euler rotations

/////RayTracer
//Implement multithreading
//give each ray a random offset, add the samples together to get anti aliasing
//Raytracer: calculate soft shadows by using a light area, take a random sample to one point of this light, and add the samples together
//sqrtf the results to make add gamma correction
//Fix BVH
//use simd to calculate 4 rays at once.
//Make MaterialOld a resource
//Divide the raytracer into grids

/////PathTracer:
//Debug radiance
//Split screen in 2 for debugging
//Stratification
//Blue noise
//Filtering
//Path Guiding


//to generate a bvh, use throw all the triangles of all the models in a big array with each triangle to a pointer to the material
//check dirty for transforms to only recalculate a part of an BVH
//refitting a BVH : resize the bounding boxes to match they children objects / leaves. (only works if the amount of objects are the same)

//Update Muse.h


