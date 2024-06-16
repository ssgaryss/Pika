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

//data structure
#include <optional>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
#include <unordered_map>

//windows api
#define NOMINMAX //禁用Windows.h中定义的min、max宏
#include <Windows.h>

//Pika engine
#include "Pika/Core/Log.h"
#include "Pika/Debug/Instrumentor.h"