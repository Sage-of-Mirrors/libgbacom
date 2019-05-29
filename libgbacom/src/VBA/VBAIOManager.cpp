#include "../../include/VBA/VBAIOManager.h"

namespace GBACom {
	namespace VBA {
		int VBAIOManager::Read(char* buffer, char& status) {
			char read_cmd[5] = { GBACom::READ, 0, 0, 0, 0 };
			
			int received_bytes = m_Device.RunBuffer(read_cmd, 5);

			for (int i = 0; i < 4; i++) {
				buffer[i] = read_cmd[i];
			}

			status = read_cmd[4];

			if (received_bytes == NOT_READY || received_bytes != 5 || (status & REG_VALID_MASK) != 0) {
				status = 0;
				return NOT_READY;
			}

			return READY;
		}

		int VBAIOManager::Write(char* buffer, char& status) {
			char write_cmd[5] = { GBACom::WRITE, buffer[0], buffer[1], buffer[2], buffer[3] };

			int received_bytes = m_Device.RunBuffer(write_cmd, 5);

			status = write_cmd[0];

			if (received_bytes == NOT_READY || received_bytes != 1 || (status & REG_VALID_MASK) != 0) {
				status = 0;
				return NOT_READY;
			}

			return READY;
		}

		int VBAIOManager::Reset(char& status) {
			char reset_cmd[5] = { GBACom::RESET, 0, 0, 0, 0 };

			int received_bytes = m_Device.RunBuffer(reset_cmd, 5);

			status = reset_cmd[2];

			if (received_bytes == NOT_READY || received_bytes != 3 || (status & REG_VALID_MASK) != 0) {
				status = 0;
				return NOT_READY;
			}

			return READY;
		}

		int VBAIOManager::GetStatus(char& status) {
			char status_cmd[5] = { GBACom::STATUS, 0, 0, 0, 0 };

			int received_bytes = m_Device.RunBuffer(status_cmd, 5);

			status = status_cmd[2];

			if (received_bytes == NOT_READY || received_bytes != 3 || (status & REG_VALID_MASK) != 0) {
				status = 0;
				return NOT_READY;
			}

			return READY;
		}

		int VBAIOManager::JoyBoot(char* program, int length, int game_code, char& status) {
			int return_code = NOT_READY;
			status = 0;
			char read_buffer[4] = { 0, 0, 0, 0 };

			Read(read_buffer, status);

			int timeout = 0;
			// Wait until the GBA is ready to JoyBoot - when PSF0 is set in the JoyBus register
			while (!(status & REG_PSF0)) {
				Reset(status);
				GetStatus(status);

				if (timeout > 200000) {
					return JOYBOOT_CONNECTION_FAILURE;
				}

				timeout++;
			}

			// Get the session key from the GBA
			return_code = Read(read_buffer, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			// Decrypt the session key
			uint32_t session_key = Util::BytesToUint32_T(read_buffer);
			session_key ^= 0x7365646F;
			session_key = _byteswap_ulong(session_key);

			// Calculate and send our key to the GBA
			uint32_t gc_key = Util::CalculateGCKey(length);

			char gc_key_bytes[4];
			Util::Uint32_TToBytes(gc_key, gc_key_bytes);
			return_code = Write(gc_key_bytes, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			// Send the ROM header to the GBA
			for (int i = 0; i < 0xC0; i += 4) {
				return_code = Write(program + i, status);

				if (return_code == NOT_READY) {
					return NOT_READY;
				}
			}

			uint32_t fcrc = 0x15A0;
			uint32_t i;

			// Send the rest of the ROM to the GBA, encrypted
			for (i = 0xC0; i < length; i += 4) {
				char encrypted_bytes[4];
				Util::Encrypt(program + i, encrypted_bytes, i, session_key, fcrc);

				return_code = Write(encrypted_bytes, status);

				if (return_code == NOT_READY) {
					return NOT_READY;
				}
			}

			// Calculate final CRC
			fcrc |= (length << 16);
			session_key = (session_key * 0x6177614B) + 1;
			fcrc ^= session_key;
			fcrc ^= ((~(i + (0x20 << 20))) + 1);
			fcrc ^= 0x20796220;
			fcrc = _byteswap_ulong(fcrc);

			// Send final CRC to the GBA
			char final_crc[4];
			Util::Uint32_TToBytes(fcrc, final_crc);
			return_code = Write(final_crc, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			// Grab the GBA's response to the final CRC. Not useful?
			return_code = Read(read_buffer, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			status = 0;

			// Wait for the GBA to be ready to start handshake - when it's ready to send the game code
			while (!(status & REG_SEND)) {
				return_code = GetStatus(status);

				if (return_code == NOT_READY) {
					return NOT_READY;
				}
			}

			// Read in the GBA's game code and send our game code to the GBA
			return_code = Read(read_buffer, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			char game_code_bytes[4];
			Util::Uint32_TToBytes(game_code, game_code_bytes);
			return_code = Write(game_code_bytes, status);

			if (return_code == NOT_READY) {
				return NOT_READY;
			}

			// If the game code we received wasn't ours, return an error. The GBA will do the same on its side.
			if (Util::BytesToUint32_T(read_buffer) != game_code) {
				return GAME_CODE_ERROR;
			}

			return READY;
		}
	} // VBA
} // GBACom