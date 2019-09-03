#pragma once

#include "../Constants.h"
#include "../JoybusEnums.h"
#include "VBAVariables.h"

#include <SFML/Network.hpp>
#include <Windows.h>

namespace GBACom {
	namespace VBA {
		int GetTransferTime(char cmd);

		template <typename T>
		std::unique_ptr<T> MoveFromFront(std::queue<std::unique_ptr<T>>& ptrs);

		std::unique_ptr<sf::TcpSocket> GetNextSocket();
		std::unique_ptr<sf::TcpSocket> GetNextClock();

		void VBAConnectionWaiter();
		void VBAConnectionWaiter_Shutdown();
	}
}