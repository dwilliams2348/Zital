#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

//data structures
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//logging
#include "Zital/Core/Log.h"

//Debug
#include "Zital/Debug/Instrumentation.h"

#ifdef ZT_PLATFORM_WINDOWS
	#include <Windows.h>
#endif