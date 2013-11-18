#include "yocto/ocl/types.hpp"
#include "yocto/ocl/_query.hpp"

namespace yocto
{
	
	namespace ocl
	{
		
		namespace Core
		{
			static inline bool __is_space( const int C ) throw()
			{
				return C == ' ';
			}
			
			static inline bool __skip_spaces( const char * & curr, const char *last, bool zero ) throw()
			{
				while( curr < last )
				{
					if( __is_space(curr[0]) )
					{
						if( zero ) *(char *)curr = '\0';
						++curr;
					}
					else
						return true;
					
				}
				return false;
			}
			
			
			
			
			size_t CountExtensions( const string & EXTENSIONS ) throw()
			{
				const char *curr =  &EXTENSIONS[0];
				const char *last =  curr + EXTENSIONS.size();
				size_t count = 0;
				
				while( __skip_spaces(curr, last, false) )
				{
					++count;
					while( ++curr < last && ! __is_space(curr[0]) );
				}
				
				return count;
			}
			
			
			
			void ParseExtensions( const string & EXTENSIONS, const Extensions & __ext ) throw()
			{
				const char * curr =  &EXTENSIONS[0];
				const char * last =  curr + EXTENSIONS.size();
				Extensions &  ext = (Extensions &) __ext;
				size_t count = 0;
				while( __skip_spaces(curr, last, true) )
				{
					assert( count < ext.size );
					
					ext[ count ] = (char *)curr;
					while( ++curr < last && ! __is_space(curr[0]) );
					++count;
					
				}
			}
			
		}
		
	}
	
}
