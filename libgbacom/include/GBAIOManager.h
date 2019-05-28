#pragma once

#include <stdint.h>

namespace GBACom {
	class GBAIOManager {
	public:
		 virtual int Read(char& status);
		 virtual int Write(char* buffer, char& status);
		 virtual int Reset(char& status);
		 virtual int GetStatus(char& status);
		 virtual int JoyBoot(char* program, int length, char& status);
		 //virtual int GetProcessStatus(); // Currently can't do this one, don't know what to send to the GBA
	};
}