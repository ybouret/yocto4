#ifndef YOCTO_CODE_CAST_INCLUDED
#define YOCTO_CODE_CAST_INCLUDED


#include "yocto/os.hpp"


namespace yocto
{
	
	class _cast
	{
	public:
		template <typename T>
		static inline void *load( T *item )    throw() { return addr2addr(item); }
		
		template <typename T>
		static inline T    *from( void *addr ) throw() { return (T*)addr; }
        
		template <typename T, typename U>
		static inline T *trans( U *p ) throw() { return (T *)addr2addr(p); }
        
        static inline void *shift(void *addr, size_t num_bytes) throw()
        {
            return static_cast<uint8_t*>(addr)+num_bytes;
        }
        
        template <typename T>
        static T *from(void *addr, size_t shifted_by) throw()
        {
            return _cast::from<T>( _cast::shift(addr,shifted_by) );
        }
        
	private:
		static void * addr2addr( void * ) throw();
	};
    
}

#endif
