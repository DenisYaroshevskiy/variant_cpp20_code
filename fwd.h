#pragma once

#define TOOLS_FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)
