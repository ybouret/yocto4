#include "yocto/math/io/data-set.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exception.hpp"

#include "yocto/code/unroll.hpp"
#include "yocto/string/tokenizer.hpp"
#include "yocto/chars.hpp"
#include "yocto/string/conv.hpp"

#include <cstdlib>

namespace yocto
{
	
	namespace math
	{
		
		template <>
		data_set<z_type>:: ~data_set() throw()
		{
		}
		
		template <>
		data_set<z_type>:: data_set() throw() : map_()
		{
		}
		
		template <>
		void data_set<z_type>:: use( size_t idx, column &col )
		{
			if( idx <= 0 )
				throw exception( "[data_set( invalid index )]" );
			
			if( ! map_.insert( idx, &col ) )
			{
				throw exception("[data_set( index %u already in use! )]", unsigned( idx ) );
			}
		}
		
		template <>
		void data_set<z_type>:: free_all() throw()
		{
			for( colmap::iterator i = map_.begin(); i != map_.end(); ++i )
			{
				(*i)->free();
			}
		}
		
		template<>
		void data_set<z_type>:: parse_line( const string &line ) const
		{
			tokenizer tkn( line );
			
			if( !tkn.get_next( character<char>::is_space ) )
			{
				return; //! empty line
			}
			assert( tkn.token() != NULL );
			if( tkn.token()[0] == '#' )
				return;
			
			colmap::iterator p = map_.begin(); 
			for( size_t i = map_.size(); i >0; --i, ++p )
			{
				const size_t idx = p->key;
				while( tkn.count() != idx )
				{
					if( !tkn.get_next( character<char>::is_space  ) )
					{
						throw exception("[data_set( missing column %u for data #%u )]", unsigned(idx), unsigned( (*p)->size()) );
					}
				}
				
				const string res( tkn.token(), tkn.units() );
				const z_type ans = strconv::to_real<real_t>( res );
				(*p)->push_back( ans );
			}
			
		}
		
		template <>
		void data_set<z_type>:: load( ios::istream &in, callback *keep, size_t skip, size_t nmax ) const
		{
			const size_t     n = map_.size();
			if( n <= 0 )
				throw exception("[can't load empty data_set]"); 
			
			//-- empty all columns
			const colmap::iterator first_col = map_.begin();
			const colmap::iterator last_col  = map_.end();
			{
				colmap::iterator p = first_col;
				YOCTO_LOOP_(n, (*p)->free(); ++p);
			}
			
			{
				for( colmap::iterator i = first_col; i != last_col; ++i )
				{
					std::cerr << "load column " << i->key << std::endl;
				}
			}
			
			//-- and go
			string line;
			size_t iline = 0;
			while( iline < skip )
			{
				if( in.read_line(line) < 0 )
					throw exception("[data_set: can't skip line %u/%u]", unsigned( iline ), unsigned( skip ));
				++iline;
			}
			
			const bool limited = nmax > 0;
            size_t     count   = 0;
			while( in.read_line(line) >= 0 )
			{
				parse_line(line);
                if(keep)
                {
                    colmap::iterator        i = first_col;
                    const sequence<real_t> &f = **i;
                    if( ! (*keep)(f.back()) )
                    {
                        //-- suppress the point
                        for( i=first_col; i != last_col; ++i)
                        {
                            (**i).pop_back();
                        }
                        
#if !defined(NDEBUG)
                        const size_t chk = (**first_col).size();
                        for( i=first_col; i != last_col; ++i)
                        {
                            assert( (**i).size() == chk );
                        }
#endif
                        
                    }
                }
				if( limited && ++count >= nmax )
					return;
				line.clear();
				++iline;
			}
			
			
		}
		
		
	}
	
}
