#include "../../include/VBA/VBAVariables.h"

namespace GBACom {
	namespace VBA {
		std::thread s_connection_thread;
		std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_socks;
		std::queue<std::unique_ptr<sf::TcpSocket>> s_waiting_clocks;

		std::mutex s_cs_gba;
		std::mutex s_cs_gba_clk;

		std::chrono::steady_clock::time_point s_Time_Point_Zero = std::chrono::steady_clock::time_point::time_point(std::chrono::duration<int>(0));

		int s_num_connected;
		uint32_t GC_CPU_Clock = 486000000u;
		bool IsServerRunning;
	}
}