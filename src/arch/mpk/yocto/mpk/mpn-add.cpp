#include "yocto/mpk/natural.hpp"

#define YMPK_ADD_UNROLL 1
#if     YMPK_ADD_UNROLL == 1
#	include "yocto/code/unroll.hpp"
#endif

#include "yocto/code/bswap.hpp"
#include "yocto/code/cast.hpp"


namespace yocto
{
	namespace mpk
	{
		
		natural operator+( const natural &lhs, const natural &rhs )
		{
			return natural::add_( lhs, rhs );
		}
		
		natural & natural:: operator+=( const natural &rhs )
		{
			natural tmp = add_( *this, rhs );
			xch( tmp );
			return *this;
		}
		
		natural & natural:: operator++()
		{
			natural tmp = inc_(1);
			xch( tmp );
			return *this;
		}

		
		natural natural:: add_( const natural &lhs, const natural &rhs )
		{
			
			//------------------------------------------------------------------
			// Organize so that L is the largest one
			//------------------------------------------------------------------
			const natural *L = & lhs;
			const natural *R = & rhs;
			if( R->size_ > L->size_ ) cswap(L,R);
			assert( L->size_ >= R->size_);
			
			const size_t top_size = L->size_;
			const size_t com_size = R->size_;
			natural      ans( top_size+1, as_capacity );
			const uint8_t *l = L->byte_;
			const uint8_t *r = R->byte_;
			uint8_t       *s = ans.byte_;
			
			//------------------------------------------------------------------
			// Initialize
			//------------------------------------------------------------------
			unsigned carry = 0;
			
#			if     YMPK_ADD_UNROLL	== 1
			
#			define YMPK_ADD_COM(J)     \
/*			*/ carry += l[J] + r[J];   \
/*			*/ s[J]   = uint8_t(carry);\
/*			*/ carry >>= 8
			YOCTO_LOOP_FUNC(com_size,YMPK_ADD_COM,0);
			
#			define YMPK_ADD_EXT(J)     \
/*			*/ carry += l[J];   \
/*			*/ s[J]   = uint8_t(carry);\
/*			*/ carry >>= 8
			YOCTO_LOOP_FUNC(top_size-com_size,YMPK_ADD_EXT,com_size);
			
#			else
			size_t   i     = 0;
			for( ; i < com_size; ++i )
			{
				carry += l[i] + r[i];
				s[i]   = uint8_t(carry);
				carry >>= 8;
			}
			
			for( ; i < top_size; ++i )
			{
				carry += l[i];
				s[i]   = uint8_t(carry);
				carry >>= 8;
			}
#			endif
			
			//------------------------------------------------------------------
			// finalize
			//------------------------------------------------------------------
			s[top_size] = uint8_t(carry);
			
			
			ans.update();
			return ans;
		}
		
		
		natural natural:: inc_( uint8_t x ) const
		{
			uint64_t wksp[ YOCTO_U64_FOR_ITEM(natural) ];
			natural &rhs = *_cast::trans<natural,uint64_t>(wksp);
			rhs.size_ = 1;
			rhs.byte_ = &x;
			return add_( *this, rhs );
		}
		
		natural & natural:: inc( uint8_t x)
		{
			natural tmp( inc_(x) );
			xch( tmp );
			return *this;
		}
		
		
	}
	
}
