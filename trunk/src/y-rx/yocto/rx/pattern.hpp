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
			virtual pattern *clone() const = 0;
			virtual bool     accept( source &src) = 0;
			virtual void     sendto( source &src); //!< can be overriden, default is back_to(src.pool)
			
			void graphviz( ios::ostream &, const string &graph_name ) const;
			virtual void viz( ios::ostream & ) const = 0;
			void         tag( ios::ostream & ) const;
			static void  out( ios::ostream &, char c );

			
		protected:
			explicit pattern(uint32_t t) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
		};
		
		typedef core::meta_list<pattern> p_list;
		
	}
	
}


#endif
