#pragma once

#include <SFML/Network.hpp>
#include <queue>
#include <thread>
#include <mutex>

namespace GBACom {
	namespace VBA {
		extern std::thread s_connection_thread;
		extern std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_socks;
		extern std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_clocks;

		extern std::mutex s_cs_gba;
		extern std::mutex s_cs_gba_clk;

		extern std::chrono::steady_clock::time_point s_Time_Point_Zero;

		extern int s_num_connected;
		extern uint32_t GC_CPU_Clock;
		extern bool IsServerRunning;
	} // VBA
} // GBACom