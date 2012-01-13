#include "yocto/string/conv.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/ipower.hpp"

#include <cerrno>

namespace yocto 
{
	

	

	size_t strconv:: to_size( const string &txt, const char *ctx  )
	{
		static const char fmt[] = "strconv::to_size(invalid char '%c') for <%s>";
		if( !ctx )        ctx   = "";
		assert( txt.size() > 0 );
		size_t ans = 0;
		for( size_t i=0; i < txt.size(); ++i )
		{
			const char C = txt[i];
			ans *= 10;
			switch( C )
			{
				case '0': break;
				case '1': ++ans;    break;
				case '2': ans += 2; break;
				case '3': ans += 3; break;
				case '4': ans += 4; break;
				case '5': ans += 5; break;
				case '6': ans += 6; break;
				case '7': ans += 7; break;
				case '8': ans += 8; break;
				case '9': ans += 9; break;
					
				default:
					if( ctx )
					{
						throw imported::exception( ctx, fmt, C, ctx );
					}
					else
					{
						throw libc::exception( EINVAL, fmt, C, ctx );
					}

			}
		}
		return ans;
	}

#define real_t float
#include "conv.cxx"
	
#undef real_t
#define real_t double
#include "conv.cxx"
	
}

