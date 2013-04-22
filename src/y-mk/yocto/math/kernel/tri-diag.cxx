#include "yocto/math/kernel/tri-diag.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"

#include <cmath>
#include <cerrno>

#include <iostream>

namespace yocto {

	namespace math {

		template <>
		tridiag<z_type>:: tridiag() throw() :
				size_(0),
				wksp_( ),
				a_( 0 ),
				b_( 0 ),
				c_( 0 ),
				g_( 0 )
		{

		}

		template <>
		tridiag<z_type>:: tridiag( size_t n ) :
				size_( n ),
				wksp_( size_ * 4, numeric<z_type>::zero),
				a_( wksp_(-1) ),
				b_( a_ + size_ ),
				c_( b_ + size_ ),
				g_( c_ + size_ )
		{

		}

		template <>
		tridiag<z_type>:: ~tridiag() throw() {

		}

		template <>
		size_t tridiag<z_type>:: size() const throw() {
			return size_;
		}




		template <>
		void tridiag<z_type>:: make( size_t n ) {
			if( n > 0 ) {
				size_t req = n * 4;
				wksp_.make( req, numeric<z_type>::zero );
				size_ = n;
				a_ = wksp_(-1);
				b_ = a_ + size_;
				c_ = b_ + size_;
				g_ = c_ + size_;
			} else {
				wksp_.release();
				size_ = 0;
				a_ = b_ = c_ = g_ = 0;
			}
		}

		template <>
		z_type &tridiag<z_type>:: a( size_t n ) throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(a_!=NULL);
			return a_[n];
		}

		template <>
		const z_type &tridiag<z_type>:: a( size_t n ) const throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(a_!=NULL);
			return a_[n];
		}

		template <>
		z_type &tridiag<z_type>:: b( size_t n ) throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(b_!=NULL);
			return b_[n];
		}

		template <>
		const z_type &tridiag<z_type>:: b( size_t n ) const throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(b_!=NULL);
			return b_[n];
		}

		template <>
		z_type &tridiag<z_type>:: c( size_t n ) throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(c_!=NULL);
			return c_[n];
		}

		template <>
		const z_type &tridiag<z_type>:: c( size_t n ) const throw() {
			assert(n>=1);
			assert(n<=this->size());
			assert(c_!=NULL);
			return c_[n];
		}

		template <>
		z_type tridiag<z_type>:: operator()( size_t i, size_t j) const throw()
		{

			assert(i>=1);
			assert(i<=this->size());
			assert(j>=1);
			assert(j<=this->size());

			switch( int(i) - int(j) ) {
				case 0:
					return b_[i];

				case 1:
					return a_[i];

				case -1:
					return c_[i];

				default:
					return 0;
			}

		}

		template <>
		void tridiag<z_type>:: solve( array<z_type> &u, const array<z_type> &r) const
        {
			assert(size_>0);
			assert(u.size() == this->size());
			assert(r.size() == this->size());
			const size_t n = size_;

			z_type piv = b_[1];
			if( Fabs( piv ) <= REAL_MIN )
				throw libc::exception( EDOM, "tridiag.solve(B(1)=0)" );


			u[1] = r[1] / piv;

			for( size_t j=2, jm=1; j <= n; ++j,++jm )
            {
				assert(j-1==jm);
				g_[j] = c_[jm] / piv;
				piv   = b_[j] - a_[j] * g_[j];
				if( Fabs( piv ) <= REAL_MIN )
					throw libc::exception( EDOM, "tridiag.solve(pivot=0)" );
				u[j] = (r[j] - a_[j] * u[jm])/piv;
			}

			for( size_t j=n-1, jp=n;j>0;--j,--jp)
            {
				assert(j+1==jp);
				u[j] -= g_[jp] * u[jp];
			}

		}
        
        template <>
        void tridiag<z_type>:: solve( matrix<z_type> &M, const matrix<z_type> &A) const
        {
            assert(size_>0);
            const size_t n = size_;
            assert(M.rows==n);
            assert(A.rows==n);
            assert(M.cols==A.cols);
            
            for( size_t c=M.cols;c>0;--c)
            {
                z_type piv = b_[1];
                if( Fabs( piv ) <= REAL_MIN )
                    throw libc::exception( EDOM, "tridiag.solve(B(1)=0)" );
                
                M[1][c] = A[1][c] / piv;
                
                for( size_t j=2, jm=1; j <= n; ++j,++jm )
                {
                    assert(j-1==jm);
                    g_[j] = c_[jm] / piv;
                    piv   = b_[j] - a_[j] * g_[j];
                    if( Fabs( piv ) <= REAL_MIN )
                        throw libc::exception( EDOM, "tridiag.solve(pivot=0)" );
                    M[j][c] = (A[j][c] - a_[j] * M[jm][c])/piv;
                }
                
                for( size_t j=n-1, jp=n;j>0;--j,--jp)
                {
                    assert(j+1==jp);
                    M[j][c] -= g_[jp] * M[jp][c];
                }

            }
            
            
        }


		template <>
		void tridiag<z_type>:: to_scilab( std::ostream &os ) const {

			os << "[";
			for( size_t i=1; i <= size_; ++i ) {
				for( size_t j=1; j <= size_; ++j ) {
					os << " " << (*this)(i,j);
				}
				if( i < size_ )
					os << " ;";
			}
			os << "];" << std::endl;

		}

	}


}

