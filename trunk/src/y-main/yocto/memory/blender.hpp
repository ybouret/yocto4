#ifndef YOCTO_MEMORY_BLENDER_INCLUDED
#define YOCTO_MEMORY_BLENDER_INCLUDED 1

#include "yocto/memory/buffer.hpp"

namespace yocto
{
	namespace memory
	{
		
        //! in place memory blending
		class blender : public object
		{
		public:
			virtual ~blender() throw();
			virtual void schedule( const memory::ro_buffer &instr ) throw() = 0;
			virtual void scramble( void *buffer, size_t buflen    ) throw() = 0;
			void operator()( memory::rw_buffer &buffer ) throw();
			
		protected:
			explicit blender() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(blender);
		};
		
	}
	
}

#endif
