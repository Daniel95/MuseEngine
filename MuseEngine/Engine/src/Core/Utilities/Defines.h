#pragma once

#include <string>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#define BIT(x) (1 << x)

const std::string ASSETS_PATH = "../Assets/";

const std::string ENGINE_ASSET_PATH =		"../Assets/Engine/";
const std::string ENGINE_TEXTURES_PATH =	"../Assets/Engine/Textures/";

const std::string GAME_ASSET_PATH =			"../Assets/Game/";
const std::string GAME_TEXTURES_PATH =		"../Assets/Game/Textures/";
const std::string GAME_SCENE_PATH =			"../Assets/Game/Scenes/";

const std::string TEST_LEVEL = "TestLevel";
const std::string LEVEL_1_NAME = "PLevel01";
const std::string LEVEL_2_NAME = "PLevel02";
const std::string LEVEL_3_NAME = "PLevel03";

typedef unsigned long long ullong;
typedef unsigned int uint;