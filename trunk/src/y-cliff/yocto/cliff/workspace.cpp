#include "yocto/cliff/workspace.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/hsort.hpp"
#include <cerrno>

namespace yocto
{
	namespace cliff
	{
		
		void workspace_base:: check_ghosts( const void *ghosts_lo, const void *ghosts_up, const void *w, size_t n)
		{
			assert( ghosts_lo );
			assert( ghosts_up );
			assert( w );
			assert(n>0);
			const unit_t *ng_lo = (const unit_t *) ghosts_lo;
			const unit_t *ng_up = (const unit_t *) ghosts_lo;
			const unit_t *width = (const unit_t *) w;
			for( unsigned i=0; i < n; ++i )
			{
				const unit_t ng_lo_i = ng_lo[i];
				const unit_t ng_up_i = ng_up[i];
				const unit_t width_i = width[i];
				if( ng_lo_i < 0 ) 
					throw libc::exception( EDOM, "lower ghosts: negative value in dimension %u", i );
				
				if( ng_up_i < 0 ) 
					throw libc::exception( EDOM, "upper ghosts: negative value in dimension %u", i );
				
				if( ng_lo_i + ng_up_i >= width_i )
					throw libc::exception( EDOM, "too many ghosts in dimension %u", i );
			}
		}
		
		void workspace_base:: check_widths( const unit_t *w, size_t n )
		{
			assert(w);
			for( size_t i=0; i < n; ++i )
			{
				if( w[i] < 2 ) throw libc::exception( EDOM, "invalid workspace witdh in dimension %u", unsigned(i) );
			}
		}
		
		void workspace_base:: check_indices( const array<size_t> &cid, const components &cdb )
		{
			const size_t nc = cid.size();
			if( nc <= 0 )
				throw libc::exception( EDOM, "no component indices!" );
			for( size_t i=1; i <= nc; ++i)
			{
				const size_t j = cid[i];
				if( j > cdb.cmax || j < cdb.cmin )
					throw libc::exception( EDOM, "component index #%u=%u not in [%u;%u]", unsigned(i), unsigned(j), unsigned(cdb.cmin), unsigned(cdb.cmax) );
			}
		}
		
		
		
	}
	
}
