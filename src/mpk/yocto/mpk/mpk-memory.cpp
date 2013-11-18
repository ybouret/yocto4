#include "yocto/mpk/natural.hpp"
#include "yocto/memory/blocks.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	namespace mpk
	{
		
		class memmgr : public singleton<memmgr>
		{
		public:
			static const size_t limit_size = 8192;
			static const size_t chunk_size = 8*limit_size;
			
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
			
			explicit memmgr() throw() : 
			blocks_( chunk_size )
			{
			}
			
			virtual ~memmgr() throw()
			{
			}
			
			friend class singleton<memmgr>;
			static const threading::longevity life_time = memory::global::life_time - 2;
			static const char name[];
		};
		
		const char memmgr:: name[] = "mpk";
		
	}
	
	
	namespace mpk
	{
		///////////////////////////////////////////////////////////////////////
		uint8_t * mem_acquire( size_t &n ) 
		{
			static memmgr & mgr = memmgr::instance();
			return mgr.acquire( n );
		}
		
		
		size_t mem_round( size_t n )
		{
			memmgr:: _round(n);
			return n;
		}
		
		void mem_release( uint8_t * &p, size_t &n ) throw()
		{
			static memmgr & mgr = * memmgr::location();
			mgr.release(p,n);
		}

		size_t   mem_limit_size() throw()
		{
			static memmgr & mgr = memmgr::instance();
			return mgr.limit_size;
		}
		
	}
	
}

