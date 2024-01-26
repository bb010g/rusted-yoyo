#pragma once
#include "Common.hpp"

#if 0
#include <string/hash.hpp>
#endif

#if 0
inline std::unordered_map<std::string, int, big::string::transparent_string_hash, std::equal_to<>> script_asset_cache;
#endif

using Script_Data_t = CScript* (*)(int script_function_index);
