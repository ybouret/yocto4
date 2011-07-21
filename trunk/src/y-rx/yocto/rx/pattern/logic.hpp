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
				virtual void sendto( source &src ) throw(); //!< clean this and all operands

			protected:
				Operator(uint32_t t) throw();
				Operator( const Operator & );
				YOCTO_DISABLE_ASSIGN(Operator);
			};

			//! logical AND
			class AND : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('<','&','&', '>');
				static AND *create();
				virtual ~AND() throw();

				virtual pattern *clone() const { return new AND( *this ); }
				virtual bool     accept( source &src );
				void    append( pattern *p ) throw(); //!< optimized push_back

			private:
				AND() throw();
				AND( const AND & );
				YOCTO_DISABLE_ASSIGN(AND);
			};

			//! logical OR
			class OR : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('<','|','|', '>');
				static OR *create();
				virtual ~OR() throw();

				virtual pattern *clone() const { return new OR( *this ); }
				virtual bool     accept( source &src );
				void    append( pattern *p ) throw(); //!< optimized push_back

			private:
				OR() throw();
				OR( const OR & );
				YOCTO_DISABLE_ASSIGN(OR);
			};

		}

	}

}


#endif
