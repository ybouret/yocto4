#ifndef YOCTO_RX_PATTERN_INCLUDED
#define YOCTO_RX_PATTERN_INCLUDED 1


#include "yocto/rx/token.hpp"
#include "yocto/core/meta-list.hpp"

namespace yocto
{
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
						
		protected:
			explicit pattern(uint32_t t) throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
		};
		
		typedef core::meta_list<pattern> p_list;
		
	}
	
}


#endif
