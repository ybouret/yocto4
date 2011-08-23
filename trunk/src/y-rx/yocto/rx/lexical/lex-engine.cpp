#include "yocto/rx/lexical/engine.hpp"

#include <cstring>

namespace yocto
{
	namespace regex
	{
		
		namespace lexical
		{
			
			engine:: engine( label_type l ) throw() :
			label(l),
			accepters(),
			callbacks(),
			name_()
			{
				memset( name_, 0, sizeof(name_) );
				for( size_t i=1,j=0; i <= sizeof(label_type); ++i )
				{
					const unsigned B = ( label >> ( ( sizeof(label_type) - i ) << 3 ) ) & 0xff;
					if( B >= 32 && B < 127 )
					{
						name_[j++] = char(B);
					}
					else 
					{
						name_[j++] = '.';
					}

				}
			}
			
			engine:: ~engine() throw() {}
			
			engine:: engine( const engine &other ) :
			label( other.label ),
			accepters( other.accepters ),
			callbacks( other.callbacks ),
			name_()
			{
				for( size_t i=0; i < sizeof(name_); ++i ) name_[i] = other.name_[i];
			}
			
			const char * engine:: name() const throw()
			{
				return name_;
			}

			void engine:: operator()( pattern *p, rule *r ) throw()
			{
				assert( p != NULL );
				assert( r != NULL );
				assert( accepters.size == callbacks.size );
				accepters.push_back( p );
				callbacks.push_back( r );
			}

			void engine:: make( pattern *p, const action &a ) 
			{
				try 
				{
					(*this)(p, make::create(a) );
				}
				catch(...)
				{
					delete p;
				}
			}
			
		}
		
	}
	
}
