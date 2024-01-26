#pragma once
#include "Common.hpp"

using GetSaveFileName_t = int64_t (*)(char *output_path, int64_t size_of_output_path, const char *input_path);
