#pragma once

#include "Constants.h"

#include <memory>
#include <queue>
#include <chrono>
#include <Windows.h>

namespace GBACom {
	namespace Util {
		uint64_t IntervalToTicks(std::chrono::steady_clock::time_point a, std::chrono::steady_clock::time_point b);

		uint32_t BytesToUint32_T(char* bytes);
		char* Uint32_TToBytes(uint32_t integer);

		uint32_t CalculateGCKey(uint32_t size);
		uint32_t DoCRC(uint32_t crc, uint32_t value);
		char* Encrypt(char* data, uint32_t i, uint32_t& session_key, uint32_t& fcrc);
	} // Util
} // GBACom
