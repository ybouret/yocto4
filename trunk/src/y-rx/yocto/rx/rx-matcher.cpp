#include "yocto/rx/matcher.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/ios/imstream.hpp"

namespace yocto
{
	namespace regex 
	{
		
		matcher:: ~matcher() throw() 
		{
			assert( pattern_ );
			delete pattern_;
			pattern_ = NULL;
		}
		
		
		matcher:: matcher( const string &expr, pattern_db *dict ) :
		source_(),
		pattern_( compile( expr, dict ) ),
		output()
		{
		}
		
		matcher:: matcher( const char *expr, pattern_db *dict ) :
		source_(),
		pattern_( compile( expr, dict ) ),
		output()
		{
		}
		
		
		void matcher:: reset( const string &expr, pattern_db *dict )
		{
			assert( pattern_ );
			pattern *p = compile(expr, dict);
			delete pattern_;
			pattern_ = p;
		}
		
		void matcher:: reset( const char *expr, pattern_db *dict )
		{
			assert( pattern_ );
			pattern *p = compile(expr, dict);
			delete pattern_;
			pattern_ = p;
		}
		
		void matcher:: init() throw()
		{
			pattern_->clear();
			output.release();
		}
		
		bool matcher:: exactly( const string &input )
		{
			ios::imstream in( input );
			init();
			
			source_.connect( in );
			if( pattern_->accept( source_ ) )
			{
				output.merge_back( *pattern_ );
				return true;
			}
			else 
				return false;
		}
		
		bool matcher:: partly( const string &input )
		{
			ios::imstream in( input );
			init();
			
			source_.connect( in );
			while( source_.peek() )
			{
				if( pattern_->accept( source_ ) )
				{
					output.merge_back( *pattern_ );
					return true;
				}
				else
				{
					source_.skip(1);
				}
				
			}
			
			return false;
		}
		
	}
	
}
