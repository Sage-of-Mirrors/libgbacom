#pragma once

#include "../Export.h"
#include "../Constants.h"
#include "../JoybusDefines.h"
#include "VBAServer.h"
#include "VBAFunctions.h"

#include <chrono>
#include <iostream>

namespace GBACom {
	namespace VBA {
		class LIBGBACOM_API VBADevice {
			VBAServer m_Server;

			JoybusNextAction m_NextAction = JoybusNextAction::SEND;
			char m_LastCommand;
			uint64_t m_TimestampSent = 0;
		public:
			//GBADevice();

			int RunBuffer(char* buffer, int length);
			int TransferInterval();
			bool GetData(uint32_t& hi, uint32_t& lo);
			void SendCommand(uint32_t command, char poll);
		};
	} // VBA
} // GBACom