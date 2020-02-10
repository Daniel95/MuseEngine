#pragma once

#include "Core/Utilities/Log.h"

#include <string>

#define GLM_FORCE_XYZW_ONLY

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define MUSE_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define MUSE_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define MUSE_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define MUSE_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define MUSE_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

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