#pragma once

#include "../Export.h"

#include <SFML/Network.hpp>
#include <queue>
#include <thread>
#include <mutex>

namespace GBACom {
	namespace VBA {
		extern LIBGBACOM_API std::thread s_connection_thread;
		extern LIBGBACOM_API std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_socks;
		extern LIBGBACOM_API std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_clocks;

		extern LIBGBACOM_API std::mutex s_cs_gba;
		extern std::mutex s_cs_gba_clk;

		extern LIBGBACOM_API std::chrono::steady_clock::time_point s_Time_Point_Zero;

		extern LIBGBACOM_API int s_num_connected;
		extern LIBGBACOM_API uint32_t GC_CPU_Clock;
		extern LIBGBACOM_API bool IsServerRunning;
	} // VBA
} // GBACom