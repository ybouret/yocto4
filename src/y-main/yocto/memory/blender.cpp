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
		
	}
	
	
}

