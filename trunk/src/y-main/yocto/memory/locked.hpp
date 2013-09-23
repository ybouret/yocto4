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
