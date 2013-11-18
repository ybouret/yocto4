#ifndef YOCTO_REGEX_MATCHER_INCLUDED
#define YOCTO_REGEX_MATCHER_INCLUDED 1

#include "yocto/rx/source.hpp"


namespace yocto 
{
	
	namespace regex
	{
				
		class pattern;
		class pattern_dict;
		
		class matcher : public object
		{
		public:
			explicit matcher( const string &expr, pattern_dict *dict = NULL );
			explicit matcher( const char   *expr, pattern_dict *dict = NULL );
			virtual ~matcher() throw();
			
			void reset( const string &expr, pattern_dict *dict = NULL );
			void reset( const char   *expr, pattern_dict *dict = NULL );
			
			bool exactly( const string & );
			bool partly(  const string & );
			
		private:
			pattern            *pattern_;
			YOCTO_DISABLE_COPY_AND_ASSIGN(matcher);
			void init() throw();
			
		public:
			token              output;             
			
		};
	}
	
}

#endif
