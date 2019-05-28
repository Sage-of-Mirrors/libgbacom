#pragma once

#include "Export.h"

namespace GBACom {
	constexpr auto BITS_PER_SECOND = 115200;
	constexpr auto BYTES_PER_SECOND = BITS_PER_SECOND / 8;
	constexpr auto SEND_MAX_SIZE = 5, RECV_MAX_SIZE = 5;
	constexpr auto TICKS_PER_SECOND = 486000000u;
}
