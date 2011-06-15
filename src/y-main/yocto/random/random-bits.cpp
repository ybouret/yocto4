#include "yocto/random/bits.hpp"

namespace yocto
{

	namespace Random
	{

		Bits:: ~Bits() throw()
		{

		}

		Bits:: Bits() throw()
		{
		}

		void Bits:: fill( memory::rw_buffer &buf ) throw()
		{
			uint8_t *q = (uint8_t *) buf.rw();
			for( size_t i=0; i < buf.length(); ++i )
			{
				q[i] = full<uint8_t>();
			}
		}

	}
}
