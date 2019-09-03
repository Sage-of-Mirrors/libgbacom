#pragma once

#include <stdint.h>

namespace GBACom {
	class GBAIOManager {
	public:
		 virtual int Read(char* buffer, char& status) = 0;
		 virtual int Write(char* buffer, char& status) = 0;
		 virtual int Reset(char& status) = 0;
		 virtual int GetStatus(char& status) = 0;
		 virtual int JoyBoot(char* program, int length, int game_code, char& status) = 0;
		 //virtual int GetProcessStatus(); // Currently can't do this one, don't know what to send to the GBA
	};
}