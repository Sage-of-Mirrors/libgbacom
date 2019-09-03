#pragma once

namespace GBACom {
#define REG_VALID_MASK 0xC5
#define REG_PSF1 0x20
#define REG_PSF0 0x10
#define REG_SEND 0x08
#define REG_RECV 0x02

#define JOYBOOT_CONNECTION_FAILURE -3
#define GAME_CODE_ERROR -2
#define NOT_READY -1
#define READY 0
#define BUSY 2
#define JOYBOOT_UNKNOWN_STATE 3
#define JOYBOOT_ERR_INVALID 4

#define JOYBOOT_TIMEOUT 100000
}

