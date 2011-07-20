#ifndef YOCTO_RX_LOGIC_INCLUDED
#define YOCTO_RX_LOGIC_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/code/fourcc.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace logical
		{
			
			class Operator : public pattern
			{
			public:
				p_list operands;
			
				virtual ~Operator() throw();
				virtual void sendto( source &src ) throw();
				
			protected:
				Operator(uint32_t t) throw();
				Operator( const Operator & );
				YOCTO_DISABLE_ASSIGN(Operator);
			};
			
			class AND : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('<','&','&', '>');
				static AND *create();
				virtual ~AND() throw();
				
				virtual pattern *clone() const { return new AND( *this ); }
				virtual bool     accept( source &src );
				
			private:
				AND() throw();
				AND( const AND & );
				YOCTO_DISABLE_ASSIGN(AND);
			};
			
		}
		
	}
	
}


#endif
