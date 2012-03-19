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
				virtual void clear() throw(); //!< clear this and all operands
				
				friend inline Operator & operator<<( Operator &op, pattern *p ) throw()
				{
					op.operands.push_back(p);
					return op;
				}
				
				virtual void     brx( ios::ostream & ) const;
				
			protected:
				Operator(uint32_t t) throw();
				Operator( const Operator & );
				YOCTO_DISABLE_ASSIGN(Operator);
			};
			
			//! logical AND
			class AND : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('[','&','&', ']');
				static AND *create();
				virtual ~AND() throw();
				
				virtual pattern *clone() const { return new AND( *this ); }
				virtual bool     accept( source &src );
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				
				virtual void optimize();
				
			private:
				AND() throw();
				AND( const AND & );
				YOCTO_DISABLE_ASSIGN(AND);
			};
			
			
			
			//! logical OR
			class OR : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('[','|','|', ']');
				static OR *create();
				virtual ~OR() throw();
				
				virtual pattern *clone() const { return new OR( *this ); }
				virtual bool     accept( source &src );
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				
				virtual void optimize();
				
			private:
				OR() throw();
				OR( const OR & );
				YOCTO_DISABLE_ASSIGN(OR);
			};
			
			//! logical NONE
			class NONE : public Operator
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('N','O','N','E');
				static NONE *create();
				virtual ~NONE() throw();
				
				virtual pattern *clone() const { return new NONE( *this ); }
				virtual bool     accept( source &src );
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				
			private:
				NONE() throw();
				NONE( const NONE & );
				YOCTO_DISABLE_ASSIGN(NONE);
			};
			
			
			
			Operator *EQUAL( const string &s );
			Operator *AMONG( const string &s );
			Operator *EXCEPT( const string &s );
			
			inline Operator *EQUAL(  const char *t ) { const string s(t); return EQUAL(s);  }
			inline Operator *AMONG(  const char *t ) { const string s(t); return AMONG(s);  }
			inline Operator *EXCEPT( const char *t ) { const string s(t); return EXCEPT(s); }
		}
		
	}
	
}


#endif
