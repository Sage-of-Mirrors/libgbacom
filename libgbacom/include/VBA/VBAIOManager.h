#pragma once

#define NOMINMAX

#include "../GBAIOManager.h"
#include "../JoybusDefines.h"
#include "VBADevice.h"
#include "Util.h"

#include <SFML/Network.hpp>

namespace GBACom {
	namespace VBA {
		class VBAIOManager : GBACom::GBAIOManager {
			VBADevice m_Device;
		public:
			int Read(char* buffer, char& status);
			int Write(char* buffer, char& status);
			int Reset(char& status);
			int GetStatus(char& status);
			int JoyBoot(char* program, int length, int game_code, char& status);
		};
	} // VBA
} // GBACom