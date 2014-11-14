#ifndef YOCTO_MEMORY_SMALL_OBJECT_INCLUDED
#define YOCTO_MEMORY_SMALL_OBJECT_INCLUDED 1


#include "yocto/threading/singleton.hpp"
#include "yocto/memory/kblocks.hpp"
#include "yocto/memory/blocks.hpp"
#include "yocto/memory/global.hpp"

namespace yocto
{
    namespace memory
    {
        
        template <size_t LIMIT_SIZE,size_t CHUNK_SIZE>
        class small_object
        {
        public:
            static const size_t limit_size = LIMIT_SIZE;
            static const size_t chunk_size = CHUNK_SIZE;
            
            inline explicit small_object() throw() {}
            inline virtual ~small_object() throw() {}
            
            static inline void *operator new(size_t block_size)
            {
                assert( block_size > 0 );
                static provider &mgr = provider::instance();
                return mgr.acquire_block(block_size);
            }
            
            static inline void operator delete(void *p, size_t block_size) throw()
            {
                assert(block_size>0);
                if( p )
                {
                    assert( provider::exists() );
                    static provider &mgr = *provider::location();
                    mgr.release_block(p,block_size);
                }
            }
            
            static inline void *operator new[] ( size_t block_size )
            {
                assert(block_size>0);
                return kind<global>::acquire( block_size );
            }
            
            static inline void  operator delete[] ( void *p, size_t block_size ) throw()
            {
                assert(block_size>0);
                kind<global>::release(p,block_size);
            }
            
            //! placement new
            static inline void *operator new( size_t
#if !defined(NDEBUG)
                                             block_size
#endif
                                             , void *addr ) throw()
            {
                assert(block_size>0);
                assert(addr!=NULL);
                return addr;
            }
            
            //! placement delete
            static inline void  operator delete( void *, void *) throw() {}
            
            
            class provider : public singleton<provider>
            {
            public:
                
                inline void * acquire_block( size_t block_size )
                {
                    YOCTO_LOCK( provider::access );
                    assert(block_size>0);
                    return (block_size <= LIMIT_SIZE) ? blocks_.acquire( block_size ) : kind<global>::acquire(block_size);
                }
                
                inline void release_block( void *p, size_t block_size ) throw()
                {
                    YOCTO_LOCK( provider::access );
                    if(block_size <= LIMIT_SIZE)
                    {
                        blocks_.release( p, block_size );
                    }
                    else
                    {
                        kind<global>::release(p,block_size);
                    }
                }
                
            private:
                explicit provider() throw() : blocks_( CHUNK_SIZE ) {}
                virtual ~provider() throw() {}
                
                YOCTO_DISABLE_COPY_AND_ASSIGN(provider);
                
                kBlocks blocks_;
                //blocks blocks_;
                
                friend class singleton<provider>;
                static const char name[];
                
            public:
                static const threading::longevity life_time = global::life_time - 1;
                
            };
            
            
            template <typename T> static inline T   *acquire1() { return static_cast<T*>(small_object::operator new(sizeof(T)) );      }
            template <typename T> static inline void release1( T *p ) throw() { small_object::operator delete(p,sizeof(T)); }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(small_object);
        };
        
        
    }
}

#endif
