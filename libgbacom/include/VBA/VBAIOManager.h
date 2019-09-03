#pragma once

#define NOMINMAX

#include "../GBAIOManager.h"
#include "../JoybusDefines.h"
#include "VBADevice.h"
#include "Util.h"

#include <SFML/Network.hpp>
#include <intrin.h>

namespace GBACom {
	namespace VBA {
		class VBAIOManager : GBAIOManager {
			VBADevice m_Device;
		public:
			virtual int Read(char* buffer, char& status) override;
			virtual int Write(char* buffer, char& status) override;
			virtual int Reset(char& status) override;
			virtual int GetStatus(char& status) override;
			virtual int JoyBoot(char* program, int length, int game_code, char& status) override;
		};
	} // VBA
} // GBACom