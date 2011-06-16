#ifndef YOCTO_ISAAC_INCLUDED
#define YOCTO_ISAAC_INCLUDED 1

#include "yocto/memory/buffer.hpp"
#include "yocto/random/bits.hpp"

namespace yocto 
{
	
	
	namespace Random
	{
		
		enum ISAAC_INIT
		{
			ISAAC_INIT_NULL, //!< init with null space
			ISAAC_INIT_AUTO, //!< init with isaac auto procedure
			ISAAC_INIT_RAND  //!< init with time based seed
		};
		
		template <const size_t ALPHA>
		class isaac {
		public:
			static const size_t SIZE = ( 1<<ALPHA );
			
			virtual ~isaac() throw();
			explicit isaac( ISAAC_INIT how = ISAAC_INIT_NULL ) throw(); //!< default init, with null workspace
			explicit isaac( const memory::ro_buffer &key ) throw();     //!< fill and init
			explicit isaac( const uint32_t wksp[], const size_t wlen ) throw();
			
			//-- manual access
			uint32_t         &rsl( const size_t index ) throw();       //!< 0 - SIZE-1
			const uint32_t   &rsl( const size_t index ) const throw(); //!< 0 - SIZE-1
			
			void              ini(bool flag) throw(); //!< call randinit
			void              run() throw();          //!< call isaac
			
			
			//-- automatic access
			uint32_t rand32() throw();
			void     reset( const memory::ro_buffer &key ) throw();  //!< fill and init
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(isaac);
			struct randctx
			{
				uint32_t randcnt;
				uint32_t randrsl[(1<<ALPHA)];
				uint32_t randmem[(1<<ALPHA)];
				uint32_t randa;
				uint32_t randb;
				uint32_t randc;
			};
			randctx ctx_;
			
			static void     randinit( randctx *ctx, bool flag ) throw();  //!< initialize
			static void     call(randctx *ctx) throw();                   //!< perform isaac
			static uint32_t rand(randctx *ctx) throw();                   //!< generate a next 32 bits value
			
		public:
			class BitsGenerator : public Bits
			{
			public:
				explicit BitsGenerator( ISAAC_INIT how ) throw() :
				isaac_( how )
				{
				}
				
				explicit BitsGenerator( const memory::ro_buffer &key ) throw() :
				isaac_( key )
				{
				}
				
				explicit BitsGenerator( const uint32_t wksp[], const size_t wlen ) throw() :
				isaac_( wksp, wlen )
				{
				}
				
				virtual ~BitsGenerator() throw() {}
				
				virtual bool operator()( void ) throw()
				{
					static const uint32_t half = 0x80000000;
					return isaac_.rand32() >= half;
				}
				
				inline uint32_t rand32() throw()
				{
					return isaac_.rand32();
				}
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(BitsGenerator);
				isaac<ALPHA> isaac_;
			};
		};
		
		
		typedef isaac<8> ISAAC;
		typedef isaac<4> ISAAC_FAST;
		
		
		
	}
	
}

#endif
