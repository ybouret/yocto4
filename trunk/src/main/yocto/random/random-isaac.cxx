#include "yocto/random/isaac.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/sys/wtime.hpp"
#include "yocto/sys/pid.hpp"
#include "yocto/hashing/hash64.hpp"

#include <cstring>

namespace yocto 
{
	
	
	namespace Random
	{
		
		
		template <>
		isaac<RANDSIZL>:: ~isaac() throw()
		{
			
		}
		
#define ind(mm,x)  (*(uint32_t *)((uint8_t *)(mm) + ((x) & ((RANDSIZ-1)<<2))))
#define rngstep(mix,a,b,mm,m,m2,r,x) \
{ \
x = *m;  \
a = (a^(mix)) + *(m2++); \
*(m++) = y = ind(mm,x) + a + b; \
*(r++) = b = ind(mm,y>>RANDSIZL) + x; \
}
		
		template <>
		void isaac<RANDSIZL>::call( randctx *ctx ) throw()
		{
			register uint32_t a,b,x,y,*m,*mm,*m2,*r,*mend;
			mm=ctx->randmem; r=ctx->randrsl;
			a = ctx->randa; b = ctx->randb + (++ctx->randc);
			for (m = mm, mend = m2 = m+(RANDSIZ/2); m<mend; )
			{
				rngstep( a<<13, a, b, mm, m, m2, r, x);
				rngstep( a>>6 , a, b, mm, m, m2, r, x);
				rngstep( a<<2 , a, b, mm, m, m2, r, x);
				rngstep( a>>16, a, b, mm, m, m2, r, x);
			}
			for (m2 = mm; m2<mend; )
			{
				rngstep( a<<13, a, b, mm, m, m2, r, x);
				rngstep( a>>6 , a, b, mm, m, m2, r, x);
				rngstep( a<<2 , a, b, mm, m, m2, r, x);
				rngstep( a>>16, a, b, mm, m, m2, r, x);
			}
			ctx->randb = b; ctx->randa = a;
		}
		
#define mix(a,b,c,d,e,f,g,h) \
{ \
a^=b<<11; d+=a; b+=c; \
b^=c>>2;  e+=b; c+=d; \
c^=d<<8;  f+=c; d+=e; \
d^=e>>16; g+=d; e+=f; \
e^=f<<10; h+=e; f+=g; \
f^=g>>4;  a+=f; g+=h; \
g^=h<<8;  b+=g; h+=a; \
h^=a>>9;  c+=h; a+=b; \
}
		
		/* if (flag==TRUE), then use the contents of randrsl[] to initialize mm[]. */
		template <>
		void isaac<RANDSIZL>::randinit( randctx *ctx, const bool flag) throw()
		{
			int i;
			uint32_t  a, b,c,d,e,f,g,h;
			uint32_t *m,*r;
			ctx->randa = ctx->randb = ctx->randc = 0;
			m=ctx->randmem;
			r=ctx->randrsl;
			a=b=c=d=e=f=g=h=0x9e3779b9;  /* the golden ratio */
			
			for (i=0; i<4; ++i)          /* scramble it */
			{
				mix(a,b,c,d,e,f,g,h);
			}
			
			if (flag)
			{
				/* initialize using the contents of r[] as the seed */
				for (i=0; i<RANDSIZ; i+=8)
				{
					a+=r[i  ]; b+=r[i+1]; c+=r[i+2]; d+=r[i+3];
					e+=r[i+4]; f+=r[i+5]; g+=r[i+6]; h+=r[i+7];
					mix(a,b,c,d,e,f,g,h);
					m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
					m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
				}
				/* do a second pass to make all of the seed affect all of m */
				for (i=0; i<RANDSIZ; i+=8)
				{
					a+=m[i  ]; b+=m[i+1]; c+=m[i+2]; d+=m[i+3];
					e+=m[i+4]; f+=m[i+5]; g+=m[i+6]; h+=m[i+7];
					mix(a,b,c,d,e,f,g,h);
					m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
					m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
				}
			}
			else
			{
				/* fill in m[] with messy stuff */
				for (i=0; i<RANDSIZ; i+=8)
				{
					mix(a,b,c,d,e,f,g,h);
					m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
					m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
				}
			}
			
			isaac<RANDSIZL>::call( ctx );    /* fill in the first set of results */
			ctx->randcnt=RANDSIZ;            /* prepare to use the first set of results */
		}
		
		
		static inline 
		uint64_t brew_isaac( uint64_t x ) throw()
		{
			return hashing::hash64::mix64( x, hashing::hash64::BJ );
		}
		
		static inline void
		fill_isaac( uint32_t dw[], const size_t n ) throw()
		{
			const size_t m = n >> 1;
			assert( (n & 1) == 0 );
			uint64_t seed = brew_isaac( get_process_id() );
			for( size_t i=0, j=0; i < m; ++i )
			{
				seed = brew_isaac( seed + wtime::ticks() );
				const union
				{
					uint64_t qw;
					uint32_t dw[2];
				} value = { seed };
				dw[j++] = value.dw[0]; assert( j < n );
				dw[j++] = value.dw[1];
			}
			
		}
		
		template <>
		isaac<RANDSIZL>:: isaac( ISAAC_INIT how ) throw() :
		ctx_()
		{
			memset( &ctx_, 0, sizeof(ctx_) );
			switch( how )
			{
				case ISAAC_INIT_NULL:
					randinit( &ctx_, true  );
					break;
					
				case ISAAC_INIT_AUTO:
					randinit( &ctx_, false );
					break;
					
				case ISAAC_INIT_RAND:
					fill_isaac( ctx_.randrsl, RANDSIZ );
					randinit( &ctx_, true  );
					break;
			}
		}
		
		template <>
		void isaac<RANDSIZL>:: reset( const memory::ro_buffer &key ) throw()
		{
			memset( &ctx_, 0, sizeof(ctx_) );
			for( size_t i=0; i < RANDSIZ; ++i )
			{
				const size_t   j  = i << 2;
				const uint32_t a = key.get_byte(j );
				const uint32_t b = key.get_byte(j+1);
				const uint32_t c = key.get_byte(j+2);
				const uint32_t d = key.get_byte(j+3);
				ctx_.randrsl[i]  = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
				
			}
			randinit( &ctx_, true );
		}
		
		template <>
		isaac<RANDSIZL>:: isaac( const memory::ro_buffer &key ) throw() :
		ctx_()
		{
			reset(key);
		}
		
		
		
		
		
		template <>
		uint32_t isaac<RANDSIZL>:: rand( randctx *r ) throw() {
			return (!(r)->randcnt-- ?
			        (isaac<RANDSIZL>::call(r), (r)->randcnt=RANDSIZ-1, (r)->randrsl[(r)->randcnt]) :
			        (r)->randrsl[(r)->randcnt]);
		}
		
		
		
		template <>
		uint32_t isaac<RANDSIZL>:: rand32() throw() {
			return rand( &ctx_ );
		}
		
		template <>
		uint32_t &isaac<RANDSIZL>:: rsl( const size_t index ) throw() {
			assert(index<SIZE);
			return ctx_.randrsl[index];
		}
		
		template <>
		const uint32_t &isaac<RANDSIZL>:: rsl( const size_t index ) const throw() {
			assert(index<SIZE);
			return ctx_.randrsl[index];
		}
		
		template <>
		void  isaac<RANDSIZL>:: ini( bool flag ) throw()
		{
			randinit( &ctx_, flag );
		}
		
		template <>
		void  isaac<RANDSIZL>:: run() throw()
		{
			call( &ctx_ );
		}
		
		static inline
		uint32_t isaac_ini32( uint32_t &saux, uint32_t rword ) throw()
		{
			uint32_t lword = saux++;
			hashing::hash64::NR( &lword, &rword );
			return rword;
		}
		
		template <>
		isaac<RANDSIZL>:: isaac( const uint32_t wksp[], const size_t wlen ) throw() :
		ctx_()
		{
			assert( !(wksp==NULL&&wlen>0) );
			memset( &ctx_, 0, sizeof(ctx_) );
			if( wlen > 0 )
			{
				uint32_t saux = uint32_t( wlen );
				if( wlen < SIZE )
				{
					for( size_t i=0; i < SIZE; ++i )
					{
						
						ctx_.randrsl[ i ] = isaac_ini32( saux, wksp[ i % wlen ] );
					}
				}
				else
				{
					// wlen >= SIZE
					for( size_t i=0; i < wlen; ++i )
					{
						ctx_.randrsl[ i % SIZE ] = isaac_ini32( saux, wksp[ i ] );
					}
				}
				
			}
			randinit( &ctx_, true );
			
		}
		
		
	}
	
	
}
