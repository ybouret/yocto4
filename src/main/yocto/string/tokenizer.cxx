#include "yocto/string/tokenizer.hpp"

namespace yocto {
	
	namespace core 
	{
	
		template <>
		tokenizer<yCH>:: ~tokenizer() throw()
		{
		}
		
		template <>
		tokenizer<yCH>:: tokenizer( const basic::string<yCH> &input ) throw() :
		curr_( &input[0] ),
		last_( curr_ + input.size() ),
		token_( NULL ),
		units_( 0 ),
		count_( 0 )
		{
		}
		
		template <>
		bool tokenizer<yCH>:: get_next( bool (*is_separator)( yCH ) throw() ) throw()
		{
			assert(NULL!=is_separator);
			token_ = NULL;
			units_ = 0;
			for(;;)
			{
				if( curr_ >= last_ )
					return false;
				
				if( is_separator( *curr_ ) )
				{
					++curr_;
					continue;
				}
				else
					break;
			}
			assert( !is_separator( *curr_ ) );
			token_ = curr_;
			
            units_ = 1;
			++curr_;

            
            
			for(;;)
			{
				if( curr_ >= last_)
					break;
					
				if( is_separator(*curr_) )
				{
					++curr_;
					break;
				}
				
				++units_;
				++curr_;
			}
			++count_;
			return true;
		}
	
		template <>
		void tokenizer<yCH>::split( sequence< basic::string<yCH> > &seq, const basic::string<yCH> &input, bool (*is_separator)( yCH ) throw())
		{
			tokenizer tkn( input );
			while( tkn.get_next( is_separator ) )
			{
				const basic::string<yCH> s( tkn.token_, tkn.units_ );
				seq.push_back( s );
			}
			
		}
		
		

		
	}
	
}


