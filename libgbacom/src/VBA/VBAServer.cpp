#include "../../include/VBA/VBAServer.h"

namespace GBACom {
	namespace VBA {
		VBAServer::VBAServer() {
			if (!s_connection_thread.joinable()) {
				s_connection_thread = std::thread(VBAConnectionWaiter);
			}

			s_num_connected = 0;
		}

		VBAServer::~VBAServer() {
			Disconnect();
		}

		void VBAServer::Disconnect() {
			if (m_Client) {
				s_num_connected--;
				m_Client->disconnect();
				m_Client = nullptr;
			}

			if (m_ClockSync) {
				m_ClockSync->disconnect();
				m_ClockSync = nullptr;
			}

			m_LastTimeSlice = 0;
			m_IsBooted = false;
		}

		void VBAServer::ClockSync() {
			if (!m_ClockSync) {
				if (!(m_ClockSync = GetNextClock())) {
					return;
				}
			}

			uint64_t time_slice = 0;

			if (m_LastTimeSlice == 0) {
				s_num_connected++;
				m_LastTimeSlice = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero);
				time_slice = GC_CPU_Clock / 60;
			}
			else {
				time_slice = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero) - m_LastTimeSlice;
			}

			time_slice = (time_slice * 16777216) / GC_CPU_Clock;
			m_LastTimeSlice = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero);

			char bytes[4] = { 0, 0, 0, 0 };
			bytes[0] = (time_slice >> 24) & 0xFF;
			bytes[1] = (time_slice >> 16) & 0xFF;
			bytes[2] = (time_slice >> 8) & 0xFF;
			bytes[3] = time_slice & 0xFF;

			sf::Socket::Status socket_status = m_ClockSync->send(bytes, 4);

			if (socket_status == sf::Socket::Status::Disconnected) {
				m_ClockSync->disconnect();
				m_ClockSync = nullptr;
			}
		}

		bool VBAServer::Connect() {
			if (!IsConnected()) {
				m_Client = GetNextSocket();
			}

			return IsConnected();
		}

		bool VBAServer::IsConnected() {
			return static_cast<bool>(m_Client);
		}

		void VBAServer::Send(const char* si_buffer) {
			if (!Connect()) {
				return;
			}

			std::array<char, SEND_MAX_SIZE> send_data;
			for (size_t i = 0; i < send_data.size(); i++) {
				send_data[i] = si_buffer[i];
			}

			char cmd = send_data[0];
			if (cmd != JoybusCommand::STATUS)
				m_IsBooted = true;

			m_Client->setBlocking(false);
			sf::Socket::Status socket_status;

			if (cmd == JoybusCommand::WRITE) {
				socket_status = m_Client->send(send_data.data(), send_data.size());
			}
			else {
				socket_status = m_Client->send(send_data.data(), 1);
			}

			if (socket_status == sf::Socket::Status::Disconnected)
				Disconnect();
		}

		int VBAServer::Receive(char* si_buffer) {
			if (!m_Client) {
				return  0;
			}

			if (m_IsBooted) {
				sf::SocketSelector socket_selector;
				socket_selector.add(*m_Client);
				socket_selector.wait(sf::milliseconds(1000));
			}

			size_t num_bytes_recieved = 0;
			std::array<char, RECV_MAX_SIZE> recieved_data;

			sf::Socket::Status recieve_status = m_Client->receive(recieved_data.data(), recieved_data.size(), num_bytes_recieved);

			if (recieve_status == sf::Socket::Status::Disconnected) {
				Disconnect();
				return 0;
			}

			if (recieve_status == sf::Socket::Status::NotReady || num_bytes_recieved == 0) {
				m_IsBooted = false;
				return 0;
			}

			for (size_t i = 0; i < recieved_data.size(); i++) {
				si_buffer[i] = recieved_data[i];
			}

			return static_cast<int>(std::min(num_bytes_recieved, recieved_data.size()));
		}
	}
}