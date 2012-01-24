#include "yocto/cliff/workspace.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/hsort.hpp"
#include <cerrno>

namespace yocto
{
	namespace cliff
	{
		
		void workspace_base:: check_ghosts( const void *outer_lo, const void *outer_up, const void *inner_lo, const void *inner_up,  const void *w, size_t n )
		{
			assert( inner_lo );
			assert( inner_up );
			assert( outer_lo );
			assert( outer_up );
			assert( w );
			assert(n>0);
			
			const unit_t *ng_outer_lo = (const unit_t *) outer_lo;
			const unit_t *ng_outer_up = (const unit_t *) outer_up;
			
			const unit_t *ng_inner_lo = (const unit_t *) inner_lo;
			const unit_t *ng_inner_up = (const unit_t *) inner_up;
			
			const unit_t *width = (const unit_t *) w;
			
			for( unsigned i=0; i < n; ++i )
			{
				
				const unit_t width_i = width[i];
				if( ng_outer_lo[i] < 0 ) 
					throw libc::exception( EDOM, "outer lower ghosts: negative value in dimension %u", i );
				
				if( ng_outer_up[i] < 0 ) 
					throw libc::exception( EDOM, "outer upper ghosts: negative value in dimension %u", i );
				
				if( ng_inner_lo[i] < 0 ) 
					throw libc::exception( EDOM, "inner lower ghosts: negative value in dimension %u", i );
				
				if( ng_inner_up[i] < 0 ) 
					throw libc::exception( EDOM, "inner upper ghosts: negative value in dimension %u", i );
							
				if( ng_inner_lo[i] + ng_inner_up[i] >= width_i )
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
				if( j > cdb.number || j < 1 )
					throw libc::exception( EDOM, "component index #%u=%u not in [1;%u]", unsigned(i), unsigned(j),  unsigned(cdb.number) );
			}
		}
		
		
		
	}
	
}
