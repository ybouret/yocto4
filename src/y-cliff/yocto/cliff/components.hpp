//! \file

#ifndef YOCTO_CLIFF_COMPONENTS_INCLUDED
#define YOCTO_CLIFF_COMPONENTS_INCLUDED 1

#include "yocto/cliff/types.hpp"
#include "yocto/string.hpp"

#include "yocto/sequence/vector.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
	namespace cliff
	{
		
		//! database for components indices/name
		class components
		{
		public:
			//! special constructor
			/**
			 \param a if name_list!=NULL then cmin = a else cmin=min(a,b)
			 \param b if name_list!=NULL then cmax = cmin + b-1 (b is #name_list,b>0) else cmax=max(a,b)
			 \param name_list  if name_list != NULL then b=#name_list.
			*/
			explicit components( size_t a, size_t b, const char *name_list[] );
			virtual ~components() throw();
			
			const size_t cmin;
			const size_t cmax;
			const size_t size;
			
			const string & name( size_t i ) const throw();
			const char   * text( size_t i ) const throw();
			void           name( size_t i, const string &id );
			void           name( size_t i, const char   *id );
			size_t operator()( const string &id ) const;
			size_t operator()( const char   *id ) const;
			
		private:
			vector<string>     names;
			map<string,size_t> idxDB;
			YOCTO_DISABLE_ASSIGN(components);
		};
		
	}
	
}


#endif
