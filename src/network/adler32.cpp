#include <network/adler32.h>

namespace happy
{
	namespace utils
	{
		namespace network
		{
			static const uint32_t BASE = 65521UL;    /* largest prime smaller than 65536 */
			static const uint32_t NMAX = 5552;

			// Used to expand the loop to boost the speed.
#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

			uint32_t adler32(const void* rbuf, uint32_t length, uint32_t adler/* = 1*/) {
				auto buf = reinterpret_cast<const unsigned char*>(rbuf);
				uint64_t sum2 = (adler >> 16) & 0xffff;
				adler &= 0xffff;
				if (length == 1) {
					adler += buf[0];
					if (adler >= BASE)
						adler -= BASE;
					sum2 += adler;
					if (sum2 >= BASE)
						sum2 -= BASE;
					return adler | static_cast<uint32_t>(sum2 << 16);
				}

				// deferred CHECK_NOTNULL make length == 1 fase
				if (buf == nullptr)
					return 1;

				if (length < 16) {
					while (length--) {
						adler += *buf++;
						sum2 += adler;
					}
					if (adler >= BASE)
						adler -= BASE;
					sum2 %= BASE;
					return static_cast<uint32_t>(adler | (sum2 << 16));
				}

				while (length >= NMAX) {
					length -= NMAX;
					uint32_t n = NMAX / 16;
					do {
						DO16(buf);
						buf += 16;
					} while (--n);
					adler %= BASE;
					sum2 %= BASE;
				}

				if (length) {
					while (length >= 16) {
						length -= 16;
						DO16(buf);
						buf += 16;
					}
					while (length--) {
						adler += *buf++;
						sum2 += adler;
					}
					adler %= BASE;
					sum2 %= BASE;
				}
				return static_cast<uint32_t>(adler | (sum2 << 16));
			}
		}
	}
}
