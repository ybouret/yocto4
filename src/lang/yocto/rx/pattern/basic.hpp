#ifndef YOCTO_RX_PATTERN_BASIC_INCLUDED
#define YOCTO_RX_PATTERN_BASIC_INCLUDED 1

#include "yocto/rx/pattern.hpp"
#include "yocto/code/fourcc.hpp"
#include "yocto/rx/symbols.hpp"

namespace yocto
{
	
	namespace regex
	{
		
		namespace basic 
		{
		
			//! one char output
			class one_char : public pattern
			{
			public:
				virtual ~one_char() throw();
				virtual  bool is_valid( char ) const throw() = 0;
				
				virtual bool accept( source &src ); //!< accept char if is_valid(char)
				
			protected:
				explicit one_char( uint32_t t) throw();
				
				
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(one_char);
			};
			
			//! any one char
			class any1 : public one_char
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('A','N','Y','1');
				
				static   any1  *create(); //!< data=NULL
				virtual ~any1() throw();

				virtual pattern *clone() const { return new any1(); }
				virtual bool     is_valid( char ) const throw(); //!< always true
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				virtual void     brx( ios::ostream & ) const;
				
			private:
				any1() throw();
				any1( const any1 & ) throw();
				YOCTO_DISABLE_ASSIGN(any1);
			};
			
			
			//! any single char
			class single: public one_char
			{
			public:
				const char value;

				static const uint32_t id = YOCTO_FOURCC('S','N','G','L');
				static   single *create( char c ); //!< data = &value
				virtual ~single() throw();
				
				virtual pattern *clone() const  { return new single( value ); }
				virtual bool     is_valid(char) const throw();
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				virtual void     brx( ios::ostream & ) const;

			private:
				explicit single( char c ) throw();
				YOCTO_DISABLE_COPY_AND_ASSIGN(single);
			};
			
			
			//! any char in a range
			class range : public one_char
			{
			public:
				const uint8_t lower;
				const uint8_t upper;
				
				static const uint32_t id = YOCTO_FOURCC('R','N','G','E');
				static   range *create( char a, char b );
				virtual ~range() throw();
				
				virtual pattern *clone() const { return new range(lower,upper); }
				virtual bool     is_valid(char) const throw();
				virtual void     viz( ios::ostream & ) const;
				virtual void     gather( first_chars &fch ) const;
				virtual void     brx( ios::ostream & ) const;

			private:
				explicit range( uint8_t a, uint8_t b) throw();
				YOCTO_DISABLE_COPY_AND_ASSIGN(range);
			};
			
			
			//! any char within symbols
			class within : public one_char
			{
			public:
				static const uint32_t id = YOCTO_FOURCC('W','/','I','N');
				static  within *create();
				virtual ~within() throw();
				
				virtual pattern *clone() const { return new within( *this ); }
				virtual bool     is_valid(char) const throw();
				virtual void     viz( ios::ostream & ) const;

				void reserve( size_t n );
				void insert( char );
				void insert( char, char );
				
				virtual void     gather( first_chars &fch ) const;
				virtual void     brx( ios::ostream & ) const;

			private:
				symbols symbols_;
				explicit within() throw();
				within( const within &);
				YOCTO_DISABLE_ASSIGN(within);
			};
			
		}
		
	}
	
}

#endif
