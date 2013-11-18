#ifndef YOCTO_MEMORY_LOCKED_INCLUDED
#define YOCTO_MEMORY_LOCKED_INCLUDED 1

#include "yocto/memory/allocator.hpp"
#include "yocto/type/ints.hpp"

namespace yocto
{
    namespace memory
    {
        
        class locked : public singleton<locked>, public allocator
        {
        public:
            const size_t page_size;
            
            virtual void *acquire( size_t &n );
			virtual void  release( void * &p, size_t &n ) throw();
            
            void *try_acquire( size_t &n, bool &safe);
            
            template <typename T>
			inline T *try_acquire_as( size_t &n, bool &safe)
			{
				size_t bytes = n * sizeof(T);
				try
				{
					T     *ptr  = static_cast<T*>( try_acquire(bytes,safe) );
					assert( bytes >= n * sizeof(T) );
					n = bytes / sizeof(T);
					return ptr;
				}
				catch(...)
				{
					assert( bytes == 0 );
					n = 0;
					throw;
				}
			}
            
        private:
            explicit locked();
            virtual ~locked() throw();
            friend class singleton<locked>;
            
        public:
            static const char name[];
			static const threading::longevity life_time = limit_of<threading::longevity>::maximum-1;
        };
        
        
    }
}

#endif
