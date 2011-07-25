#ifndef YOCTO_RX_PATTERN_INCLUDED
#define YOCTO_RX_PATTERN_INCLUDED 1


#include "yocto/rx/token.hpp"
#include "yocto/core/meta-list.hpp"
#include "yocto/rx/symbols.hpp"

namespace yocto
{
	
	namespace ios
	{
		class ostream;
	}
	
	namespace regex
	{
		
		class source;
		
		class pattern : public token
		{
		public:
			const uint32_t type; //!< pattern class identifier
			void          *data; //!< pattern special data address
			pattern       *next; //!< for the list
			pattern       *prev; //!< for the list
			
			virtual ~pattern() throw();
			virtual pattern *clone() const = 0;        //!< virtual copy constructor
			virtual bool     accept( source &src) = 0; //!< matching ?
			virtual void     sendto( source &src);     //!< can be overriden, default is back_to(src.pool)
			
			//! convert this pattern as a graphviz graph
			void graphviz( ios::ostream &, const string &graph_name ) const;
			virtual void viz( ios::ostream & ) const = 0; //!< for each pattern
			void         tag( ios::ostream & ) const;     //!< uniq GraphViz node identifier
			static void  out( ios::ostream &, char c );   //!< GrapViz node readable label
			
			//! logical optimization, default is do nothing
			virtual void optimize();
			
		protected:
			explicit pattern(uint32_t t) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
		};
		
		typedef core::meta_list<pattern> p_list;
		
	}
	
}


#endif
