#pragma once

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

//STD
#include <experimental/filesystem>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <typeinfo> 
#include <memory>
#include <cassert>
#include <chrono>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <crtdbg.h>
#include <algorithm>
#include <functional>
#include <sstream>

//RapidJSON
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

//RTTR
#include <rttr/variant.h>
#include <rttr/registration>

//spdlog
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

//GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>