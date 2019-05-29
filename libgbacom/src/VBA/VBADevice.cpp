#include "../../include/VBA/VBADevice.h"

namespace GBACom {
	namespace VBA {
		int VBADevice::RunBuffer(char* buffer, int length) {
			switch (m_NextAction) {
			case JoybusNextAction::SEND:
			{
				m_Server.ClockSync();

				if (m_Server.Connect()) {
					m_Server.Send(buffer);
				}
				else {
					return NOT_READY;
				}

				m_LastCommand = buffer[0];
				m_TimestampSent = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero);
				m_NextAction = JoybusNextAction::WAIT;
			}
			case JoybusNextAction::WAIT:
			{
				uint64_t cur_time = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero);
				uint64_t elapsed_time = cur_time - m_TimestampSent;

				int trans_time = GetTransferTime(m_LastCommand);

				while (trans_time > elapsed_time) {
					cur_time = Util::IntervalToTicks(std::chrono::high_resolution_clock::now(), s_Time_Point_Zero);
					elapsed_time = cur_time - m_TimestampSent;
				}

				m_NextAction = JoybusNextAction::RECEIVE;
			}
			case JoybusNextAction::RECEIVE:
			{
				int num_bytes_recieved = m_Server.Receive(buffer);
				m_NextAction = JoybusNextAction::SEND;

				if (num_bytes_recieved == 0) {
					return NOT_READY;
				}

				return num_bytes_recieved;
			}
			}

			return 0;
		}
	}
}