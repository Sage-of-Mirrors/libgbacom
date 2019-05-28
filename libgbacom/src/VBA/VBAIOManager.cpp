#include "../../include/VBA/VBAIOManager.h"

namespace GBACom {
	namespace VBA {
		int VBAIOManager::Read(char* buffer, char& status) {
			char read_cmd[5] = { GBACom::READ, 0, 0, 0, 0 };
			
			int received_bytes = m_Device.RunBuffer(read_cmd, 5);

			for (int i = 0; i < 4; i++) {
				buffer[i] = read_cmd[i];
			}

			//status = read_cmd[2];
			return received_bytes;
		}

		int VBAIOManager::Write(char* buffer, char& status) {
			char write_cmd[5] = { GBACom::WRITE, buffer[0], buffer[1], buffer[2], buffer[3] };

			int received_bytes = m_Device.RunBuffer(write_cmd, 5);

			status = write_cmd[2];
			return received_bytes;
		}

		int VBAIOManager::Reset(char& status) {
			char reset_cmd[5] = { GBACom::RESET, 0, 0, 0, 0 };

			int received_bytes = m_Device.RunBuffer(reset_cmd, 5);

			status = reset_cmd[2];
			return received_bytes;
		}

		int VBAIOManager::GetStatus(char& status) {
			char status_cmd[5] = { GBACom::STATUS, 0, 0, 0, 0 };

			int received_bytes = m_Device.RunBuffer(status_cmd, 5);

			status = status_cmd[0];
			return received_bytes;
		}

		int VBAIOManager::JoyBoot(char* program, int length, int game_code, char& status) {
			char read_buffer[4] = { 0, 0, 0, 0 };

			// Wait until the GBA is ready to JoyBoot
			while (!(status & REG_PSF0)) {
				Reset(status);
				GetStatus(status);
			}

			// Get the session key from the GBA
			Read(read_buffer, status);

			// Decrypt the session key
			uint32_t session_key = Util::BytesToUint32_T(read_buffer);
			session_key ^= 0x7365646F;
			session_key = _byteswap_ulong(session_key);

			// Calculate and send our key to the GBA
			uint32_t gc_key = Util::CalculateGCKey(length);
			Write(Util::Uint32_TToBytes(gc_key), status);

			// Send the ROM header to the GBA
			for (int i = 0; i < 0xC0; i += 4) {
				Write(program + i, status);
			}

			uint32_t fcrc = 0x15A0;
			uint32_t i;

			// Send the rest of the ROM to the GBA, encrypted
			for (i = 0xC0; i < length; i += 4) {
				char* encrypted_bytes = Util::Encrypt(program + i, i, session_key, fcrc);
				Write(encrypted_bytes, status);
			}

			// Calculate final CRC
			fcrc |= (length << 16);
			session_key = (session_key * 0x6177614B) + 1;
			fcrc ^= session_key;
			fcrc ^= ((~(i + (0x20 << 20))) + 1);
			fcrc ^= 0x20796220;
			fcrc = _byteswap_ulong(fcrc);

			// Send final CRC to the GBNA
			Write(Util::Uint32_TToBytes(fcrc), status);

			// Grab the GBA's response to the final CRC. Not useful?
			Read(read_buffer, status);

			status = 0;

			// Wait for the GBA to be ready to start handshake
			while (!(status & REG_SEND)) {
				GetStatus(status);
			}

			// Read in the GBA's game code and send our game code to the GBA
			Read(read_buffer, status);
			Write(Util::Uint32_TToBytes(game_code), status);

			// If the game code we received wasn't ours, return an error. The GBA will do the same on its side.
			if (Util::BytesToUint32_T(read_buffer) != game_code) {
				return -1;
			}

			return 0;
		}
	} // VBA
} // GBACom