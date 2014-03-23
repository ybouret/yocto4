#include "yocto/mpa/types.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/blocks.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/random/bits.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        class memmgr : public singleton<memmgr>
        {
        public:
			static const size_t chunk_size = 4096;
            static const size_t limit_size = chunk_size/4;
            
			static inline void _round( size_t &n ) throw()
			{
                n = (n<16) ? 16 : next_power_of_two(n);
			}
			
			inline uint8_t * acquire( size_t &n )
			{
				YOCTO_LOCK(access);
				_round(n);
				if( n > limit_size )
                {
					return memory::kind<memory::global>::acquire_as<uint8_t>(n);
                }
				else
                {
					try
                    {
						return (uint8_t *)blocks_.acquire( n );
					}
					catch(...){ n=0; throw; }
				}
			}
			
			inline void      release( uint8_t * &p, size_t &n ) throw()
			{
				YOCTO_LOCK(access);
				assert( p != NULL );
				assert( n > 0 );
                assert(is_a_power_of_two(n));
				if( n > limit_size )
				{
					memory::kind<memory::global>::release_as<uint8_t>( p, n );
				}
				else
				{
					blocks_.release(p,n);
					p = NULL;
					n = 0;
				}
			}
            
            
        private:
            memory::blocks blocks_;
            
            explicit memmgr() :
            blocks_( chunk_size )
            {
            }
            
            virtual ~memmgr() throw()
            {
            }
            
            YOCTO_DISABLE_COPY_AND_ASSIGN(memmgr);
            friend class singleton<memmgr>;
			static const threading::longevity life_time = memory::global::life_time - 2;
			static const char name[];
            
        };
        
        const char memmgr::name[] = "mpa";
        
        uint8_t *memIO::acquire(size_t &n)
        {
            static memmgr &mgr = memmgr::instance();
            return mgr.acquire(n);
        }
        
        void     memIO:: release(uint8_t *&p, size_t &n) throw()
        {
            static memmgr &mgr = * memmgr::location();
            mgr.release(p,n);
        }
        
        bool memIO:: random_bit()
        {
            static Random::Bits &mgr = Random::CryptoBits();
            return mgr();
        }
        
    }
    
}
