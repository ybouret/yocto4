#ifndef YOCTO_RX_PATTERN_DICT_INCLUDED
#define YOCTO_RX_PATTERN_DICT_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
	namespace regex
	{
		typedef shared_ptr<pattern> pattern_ptr;
		
		typedef map<string,pattern_ptr,key_hasher<string>,memory::pooled::allocator> pattern_map;
		
        //! a dictionary of patterns
		class pattern_dict : public pattern_map 
		{
		public:
			explicit pattern_dict() throw();
			virtual ~pattern_dict() throw();
			
			void     record( const string &name, pattern *p );      //!< pattern is handled...
			void     record( const char   *name, pattern *p );      //!< pattern is handled...
            void     record( const char   *name, const char *expr); //!< compiled with this
            
			pattern *create( const string &name ) const; //!< by cloning
			pattern *create( const char   *name ) const; //!< by cloning
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern_dict);
		};
		
		
		
	}
}

#endif
