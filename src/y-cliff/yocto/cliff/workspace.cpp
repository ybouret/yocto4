#include "yocto/cliff/workspace.hpp"
#include "yocto/exceptions.hpp"
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
			for( size_t i=0; i < n; ++i )
			{
				if( w[i] <=1 ) throw libc::exception( EDOM, "invalid workspaced witdh in dimension %u", unsigned(i) );
			}
		}
		
	}
	
}
