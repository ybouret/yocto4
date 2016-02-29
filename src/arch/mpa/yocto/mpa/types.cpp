#include "yocto/mpa/types.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/memory/kblocks.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/random/bits.hpp"
#include "yocto/math/types.hpp"

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
            memory::kBlocks blocks_;
            
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
        
#define SIN_OF(LN2) sin(math::numeric<double>::two_pi/(size_t(1)<<(LN2)))
        const double memIO::sin_table[32] =
        {
            SIN_OF(0),  SIN_OF(1),  SIN_OF(2),  SIN_OF(3),  SIN_OF(4),
            SIN_OF(5),  SIN_OF(6),  SIN_OF(7),  SIN_OF(8),  SIN_OF(9),
            SIN_OF(10), SIN_OF(11), SIN_OF(12), SIN_OF(13), SIN_OF(14),
            SIN_OF(15), SIN_OF(16), SIN_OF(17), SIN_OF(18), SIN_OF(19),
            SIN_OF(20), SIN_OF(21), SIN_OF(22), SIN_OF(23), SIN_OF(24),
            SIN_OF(25), SIN_OF(26), SIN_OF(27), SIN_OF(28), SIN_OF(29),
            SIN_OF(30), SIN_OF(31)
        };
        
    }
    
}
