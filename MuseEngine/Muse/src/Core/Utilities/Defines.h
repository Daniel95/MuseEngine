#pragma once

#include "Core/Utilities/Log.h"

#include <string>

#ifdef MUSE_DEBUG
#define MUSE_ENABLE_ASSERTS
#endif

#ifdef MUSE_ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { LOG_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define ASSERT_ENGINE(x, ...) { if(!(x)) { LOG_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#define ASSERT_ENGINE(x, ...)
#endif

#define SUB_FN(fn, ...) this, std::bind(&fn, this, __VA_ARGS__)

#define PI 3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f
#define DEG2RAD(x) (x*PI)/180
#define RAD2DEG(x) x*(180/PI)

const std::string ASSETS_PATH =             "../Assets/";

const std::string ENGINE_ASSET_PATH =		"../Assets/Engine/";
const std::string ENGINE_TEXTURES_PATH =	"../Assets/Engine/Textures/";

const std::string GAME_ASSET_PATH =			"../Assets/Game/";
const std::string GAME_TEXTURES_PATH =		"../Assets/Game/Textures/";
const std::string GAME_SCENE_PATH =			"../Assets/Game/Scenes/";

typedef unsigned long long ullong;
typedef unsigned int uint;