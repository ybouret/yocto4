#include "yocto/cliff/workspace.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/hsort.hpp"
#include <cerrno>

namespace yocto
{
	namespace cliff
	{
		
		void workspace_base:: check_ghosts( const void *ghosts_value, size_t n, const char *ctx )
		{
			assert(ghosts_value);
			assert(n>0);
			const unit_t *ng = (const unit_t *) ghosts_value;
			for( size_t i=0; i < n; ++i )
			{
				if( ng[i] < 0 ) throw libc::exception( EDOM, "workspace %s: negative value in dimension %u", ctx, unsigned(i) );
			}
		}
		
		void workspace_base:: check_widths( const unit_t *w, size_t n )
		{
			assert(w);
			for( size_t i=0; i < n; ++i )
			{
				if( w[i] <= 3 ) throw libc::exception( EDOM, "invalid workspaced witdh in dimension %u", unsigned(i) );
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
