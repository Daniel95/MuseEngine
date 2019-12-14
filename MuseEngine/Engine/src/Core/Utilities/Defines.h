#pragma once

#include <string>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#ifdef MUSE_DEBUG
#define MUSE_ENABLE_ASSERTS
#endif

#ifdef MUSE_ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { MUSE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define ASSERT_ENGINE(x, ...) { if(!(x)) { MUSE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#define ASSERT_ENGINE(x, ...)
#endif

#define BIT(x) (1 << x)

const std::string ASSETS_PATH = "../Assets/";

const std::string ENGINE_ASSET_PATH =		"../Assets/Engine/";
const std::string ENGINE_TEXTURES_PATH =	"../Assets/Engine/Textures/";

const std::string GAME_ASSET_PATH =			"../Assets/Game/";
const std::string GAME_TEXTURES_PATH =		"../Assets/Game/Textures/";
const std::string GAME_SCENE_PATH =			"../Assets/Game/Scenes/";

typedef unsigned long long ullong;
typedef unsigned int uint;