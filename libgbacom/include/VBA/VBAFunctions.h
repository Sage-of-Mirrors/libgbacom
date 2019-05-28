#pragma once

#include "../Export.h"
#include "../Constants.h"
#include "../JoybusEnums.h"
#include "VBAVariables.h"

#include <SFML/Network.hpp>
#include <Windows.h>

namespace GBACom {
	namespace VBA {
		LIBGBACOM_API int GetTransferTime(char cmd);

		template <typename T>
		LIBGBACOM_API std::unique_ptr<T> MoveFromFront(std::queue<std::unique_ptr<T>>& ptrs);

		LIBGBACOM_API std::unique_ptr<sf::TcpSocket> GetNextSocket();
		LIBGBACOM_API std::unique_ptr<sf::TcpSocket> GetNextClock();

		LIBGBACOM_API void VBAConnectionWaiter();
		LIBGBACOM_API void VBAConnectionWaiter_Shutdown();
	}
}