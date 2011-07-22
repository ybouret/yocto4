#ifndef YOCTO_RX_PATTERN_DB_INCLUDED
#define YOCTO_RX_PATTERN_DB_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto
{
	namespace regex
	{
		typedef shared_ptr<pattern> pattern_ptr;
		
		typedef map<string,pattern_ptr,key_hasher<string>,memory::pooled::allocator> pattern_map;
		
		class pattern_db : public pattern_map 
		{
		public:
			explicit pattern_db() throw();
			virtual ~pattern_db() throw();
			
			void     record( const string &name, pattern *p ); //!< pattern is handled...
			void     record( const char   *name, pattern *p ); //!< pattern is handled...
			pattern *create( const string &name ) const;
			pattern *create( const char   *name ) const;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern_db);
		};
		
		
		
	}
}

#endif
