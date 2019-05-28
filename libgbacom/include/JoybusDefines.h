#pragma once

#include "Export.h"

namespace GBACom {
#define REG_PSF1 0x20
#define REG_PSF0 0x10
#define REG_SEND 0x08
#define REG_RECV 0x02

#define READY 0
#define NOT_READY 1
#define BUSY 2
#define JOYBOOT_UNKNOWN_STATE 3
#define JOYBOOT_ERR_INVALID 4
}

