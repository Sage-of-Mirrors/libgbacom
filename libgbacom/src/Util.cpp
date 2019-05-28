#include "../include/Util.h"

#include <intrin.h>

namespace GBACom {
	namespace Util {
		uint64_t IntervalToTicks(std::chrono::steady_clock::time_point a, std::chrono::steady_clock::time_point b) {
			uint64_t ticks = 0;

			auto interval_as_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(a - b);
			double seconds = interval_as_milliseconds.count() / 1000.0;

			ticks = seconds * TICKS_PER_SECOND;

			return ticks;
		}

		uint32_t BytesToUint32_T(char* bytes) {
			uint32_t integer = ((int)(((unsigned char)bytes[0]) << 24) |
				                (int)(((unsigned char)bytes[1]) << 16) |
				                (int)(((unsigned char)bytes[2]) << 8)  |
				                (int)(((unsigned char)bytes[3])));

			return integer;
		}

		char* Uint32_TToBytes(uint32_t integer) {
			char bytes[4] = { 0, 0, 0, 0 };

			bytes[0] = (integer & 0xFF000000) >> 24;
			bytes[1] = (integer & 0x00FF0000) >> 16;
			bytes[2] = (integer & 0x0000FF00) >> 8;
			bytes[3] = (integer & 0x000000FF);

			return bytes;
		}

		uint32_t CalculateGCKey(uint32_t size) {
			unsigned int ret = 0;
			size = (size - 0x200) >> 3;
			int res1 = (size & 0x3F80) << 1;
			res1 |= (size & 0x4000) << 2;
			res1 |= (size & 0x7F);
			res1 |= 0x380000;
			int res2 = res1;
			res1 = res2 >> 0x10;
			int res3 = res2 >> 8;
			res3 += res1;
			res3 += res2;
			res3 <<= 24;
			res3 |= res2;
			res3 |= 0x80808080;

			if ((res3 & 0x200) == 0)
			{
				ret |= (((res3) & 0xFF) ^ 0x4B) << 24;
				ret |= (((res3 >> 8) & 0xFF) ^ 0x61) << 16;
				ret |= (((res3 >> 16) & 0xFF) ^ 0x77) << 8;
				ret |= (((res3 >> 24) & 0xFF) ^ 0x61);
			}
			else
			{
				ret |= (((res3) & 0xFF) ^ 0x73) << 24;
				ret |= (((res3 >> 8) & 0xFF) ^ 0x65) << 16;
				ret |= (((res3 >> 16) & 0xFF) ^ 0x64) << 8;
				ret |= (((res3 >> 24) & 0xFF) ^ 0x6F);
			}
			return ret;
		}

		uint32_t DoCRC(uint32_t crc, uint32_t value) {
			int i;
			for (i = 0; i < 0x20; i++)
			{
				if ((crc ^ value) & 1)
				{
					crc >>= 1;
					crc ^= 0xa1c1;
				}
				else
					crc >>= 1;
				value >>= 1;
			}
			return crc;
		}

		char* Encrypt(char* data, uint32_t i, uint32_t & session_key, uint32_t & fcrc) {
			uint32_t plaintext = ((int)(((unsigned char)data[0]) << 24) | (int)(((unsigned char)data[1]) << 16) | (int)(((unsigned char)data[2]) << 8) | (int)(((unsigned char)data[3])));
			plaintext = _byteswap_ulong(plaintext);

			fcrc = DoCRC(fcrc, plaintext);
			session_key = (session_key * 0x6177614B) + 1;

			uint32_t encrypted = plaintext ^ session_key;
			encrypted ^= ((~(i + (0x20 << 20))) + 1);
			encrypted ^= 0x20796220;

			char enc_bytes[4] = { (encrypted & 0xFF000000) >> 24, (encrypted & 0x00FF0000) >> 16, (encrypted & 0x0000FF00) >> 8, encrypted & 0x000000FF };
			return enc_bytes;
		}
	}
}