#ifndef YOCTO_AQ_INITIALIZER_INCLUDED
#define YOCTO_AQ_INITIALIZER_INCLUDED 1

#include "yocto/aqueous/chemsys.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/shared-ptr.hpp"

namespace yocto 
{
	namespace aqueous
	{

		class initializer;
		typedef functor<double,TL1(double)> initproc;

		class constraint 
		{
		public:
			typedef map<string,double>     weights;
			typedef shared_ptr<constraint> ptr;

			//! fixed constant
			explicit constraint( const library &L, double v );

			//! with a time dependant value
			explicit constraint( const library &L, const initproc &v);
			virtual ~constraint() throw();
			mutable initproc value;

			void add( const string &id, double w );
            void add( const char   *id, double w );
            
            size_t size() const throw();
            
            typedef weights::const_iterator iterator;
            iterator begin() const throw();
            iterator end() const throw();
            
            
		private:
			YOCTO_DISABLE_ASSIGN(constraint);
			weights        coefficients;
			friend class   initializer;
			const double   fixedValue;
			double         fixed( double ) const throw();

		public:
			const library &lib;
		};

		class initializer
		{
		public:
			explicit initializer(const library &L ) throw();
			virtual ~initializer() throw();

			constraint & create( double v );
			constraint & create( const initproc &v );

			void electroneutrality();

			void   operator()( chemsys &cs , double t);
			size_t size() const throw();

            friend std::ostream & operator<<( std::ostream &, const initializer & );
            
		private:
			vector< constraint::ptr > constraints;
			YOCTO_DISABLE_COPY_AND_ASSIGN(initializer);
		public:
			const library &lib;
			const bool     is_variable; //!< default: false
		};


	}

}

#endif
