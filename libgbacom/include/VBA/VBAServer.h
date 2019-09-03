#pragma once

#define NOMINMAX

#include "../Constants.h"
#include "../JoybusEnums.h"
#include "../Util.h"
#include "VBAFunctions.h"
#include "VBAVariables.h"

#include <SFML/Network.hpp>
#include <array>
#include <chrono>

namespace GBACom
{
	namespace VBA {
		class VBAServer {
		public:
			VBAServer();
			~VBAServer();

			bool Connect();
			bool IsConnected();

			void ClockSync();
			void Send(const char* si_buffer);

			int Receive(char* si_buffer);

		private:
			void Disconnect();

			std::unique_ptr<sf::TcpSocket> m_Client;
			std::unique_ptr<sf::TcpSocket> m_ClockSync;

			uint64_t m_LastTimeSlice = 0;
			bool m_IsBooted = false;
		};
	} // VBA
} // GBACom