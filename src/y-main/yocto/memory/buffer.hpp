#ifndef YOCTO_MEMORY_BUFFER_INCLUDED
#define YOCTO_MEMORY_BUFFER_INCLUDED 1


#include "yocto/object.hpp"

namespace yocto
{
	
	namespace memory
	{
		
		class ro_buffer : public object
		{
		public:
			virtual ~ro_buffer() throw();
			
			const void * ro() const throw();
			virtual size_t length() const throw() = 0; //!< in bytes
			
		protected:
			explicit ro_buffer() throw();
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(ro_buffer);
			virtual const void *get_address() const throw() = 0;
		};
		
		
		class rw_buffer : public ro_buffer
		{
		public:
			virtual ~rw_buffer() throw();
			void *rw() throw();
			
		protected:
			explicit rw_buffer() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(rw_buffer);
		};
	}
	
}

#endif
