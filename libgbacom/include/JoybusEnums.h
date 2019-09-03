#pragma once

namespace GBACom {
	enum JoybusCommand {
		RESET = 0xFF,
		STATUS = 0x00,
		READ = 0x14,
		WRITE = 0x15
	};

	enum class JoybusNextAction {
		SEND,
		WAIT,
		RECEIVE
	};
}