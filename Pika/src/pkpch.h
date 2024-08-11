#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <exception>
#include <random>
#include <limits>
#include <utility>
#include <math.h>
#include <filesystem>

//data structure
#include <optional>
#include <string>
#include <string_view>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>

//windows api
#ifdef PK_PLATFORM_WINDOWS
	#define NOMINMAX //禁用Windows.h中定义的min、max宏
	#include <Windows.h>
#endif


//Pika engine
#include "Pika/Core/Log.h"
#include "Pika/Debug/Instrumentor.h"