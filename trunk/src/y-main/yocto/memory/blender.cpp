#include "yocto/memory/blender.hpp"


namespace yocto
{
	
	namespace memory
	{
	
		blender:: ~blender() throw() {}
		blender::  blender() throw() {}
		
		void blender:: operator()( rw_buffer &buffer ) throw()
		{
			scramble( buffer.rw(), buffer.length() );
		}
	
        
        no_blender:: ~no_blender() throw() {}
		no_blender::  no_blender() throw() {}

        void no_blender:: schedule(const memory::ro_buffer &) throw() {}
        void no_blender:: scramble(void *, size_t) throw() {}
        
        
    }
	
	
}

