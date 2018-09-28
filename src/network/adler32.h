#pragma once
#include <cstdint>

namespace happy
{
	namespace utils
	{
		namespace network
		{

			// http://en.wikipedia.org/wiki/Adler-32
			// no error check, invoker must insure rbuf is valid
			uint32_t adler32(const void* rbuf, uint32_t length, uint32_t adler = 1);
		}
	} 
} 