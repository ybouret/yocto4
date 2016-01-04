#include "yocto/math/fcn/jacobian.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/math/dat/extrapolate.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	namespace math
	{
		
#define kmax_ 8
#define hdiv_ REAL(1.4)
		
		template <>
		jacobian<real_t>:: ~jacobian() throw()
		{
		}
		
		template <>
		jacobian<real_t>:: jacobian() :
		xa_(kmax_,0),
		ylo_(),
		yhi_(),
		dy_(),
		yxtr_(),
		yerr_(),
		ya_(),
		c_(),
		d_()
		{
			xa_[ 1 ] = 1;
			for( size_t i= 2; i <= kmax_; ++i)
			{
				xa_[i] = xa_[i-1] / hdiv_;
			}
		}
		
		template <>
		void jacobian<real_t>:: release() throw()
		{
			ylo_.release();
			yhi_.release();
			dy_.release();
			yxtr_.release();
			yerr_.release();
			ya_.release();
			c_.release();
			d_.release();
		}
		
		template <>
		void jacobian<real_t>:: acquire( size_t n )
		{
			if( n > 0 )
			{
				try
				{
					ylo_.make(n,0);
					yhi_.make(n,0);
					dy_.make(n,0);
					yxtr_.make(n,0);
					yerr_.make(n,0);
					ya_.make(kmax_,n);
					c_.make(kmax_,n);
					d_.make(kmax_,n);
				}
				catch (...) {
					release();
					throw;
				}
			}
			else {
				release();
			}
			
		}
		
		template <>
		size_t jacobian<real_t>:: size() const throw()
		{
			return yxtr_.size();
		}
		
		template<>
		void jacobian<real_t>:: operator()( matrix<real_t> &jac, numeric<real_t>::vector_field &F, const array<real_t> &X, real_t h0 )
		{
			
			static const real_t hmin = numeric<real_t>::tiny * Pow( real_t(hdiv_),real_t(kmax_) );
			static const real_t dtol = Sqrt( numeric<real_t>:: ftol );
			
			const size_t nvar = X.size();
			if( nvar != this->size() )
            {
                this->release();
                this->acquire(nvar);
            }
            assert( nvar == this->size() );
			for( size_t j=1; j <= nvar; ++j)
			{
				real_t       h    = max_of<real_t>( Fabs(h0), hmin );
				real_t      &Xj   = *(real_t *) & X[j];
				const real_t xsav = Xj;
			INI:
				{
					//--------------------------------------------------------------
					// sanity check on h
					//--------------------------------------------------------------
					real_t one_h = h;
					real_t two_h = one_h + one_h;
					real_t fac_h = REAL(1.0)/two_h;
					//--------------------------------------------------------------
					// first step
					//--------------------------------------------------------------
					Xj = xsav + one_h;
					F(yhi_,X);
					Xj = xsav - one_h;
					F(ylo_,X);
					for( size_t i=1; i<=nvar; ++i)
					{
						dy_[i]= Fabs( ya_[1][i] = fac_h * (yhi_[i] - ylo_[i]) );
					}
					
					//--------------------------------------------------------------
					// following steps
					//--------------------------------------------------------------
					for( size_t k=2; k <= kmax_; ++k )
					{
						array<real_t> &y_ = ya_[k];
						one_h /= hdiv_;
						two_h  = one_h + one_h;
						fac_h  = REAL(1.0)/two_h;
						
						Xj = xsav + one_h;
						F(yhi_,X);
						Xj = xsav - one_h;
						F(ylo_,X);
						
						for( size_t i=1; i <= nvar; ++i )
						{
							y_[i] = fac_h * (yhi_[i] - ylo_[i]);
						}
						extrapolate<real_t>::get( yxtr_, yerr_, xa_, ya_, k,  c_, d_ );
						
						//----------------------------------------------------------
						// check not too much divergence
						//----------------------------------------------------------
						for( size_t i=1; i <= nvar; ++i )
						{
							const real_t dy_new = Fabs( yerr_[i] ); yerr_[i] = Fabs( dy_new );
							const real_t dy_old = dy_[i];
							if( dy_new > (dy_old+dy_old) )
							{
								//std::cerr << "  divergence" << std::endl;
								goto END;
							}
						}
						
						
						//----------------------------------------------------------
						// check convergence
						//----------------------------------------------------------
						bool has_converged = true;
						for( size_t i=1; i <= nvar; ++i )
						{
							const real_t dy_new = yerr_[i];
							if( dy_new > Fabs( dtol * y_[i] ) )
							{
								has_converged = false;
								break;
							}
						}
						
						if( has_converged )
						{
							Xj = xsav;
							for( size_t i=1; i <= nvar; ++i )
							{
								jac[i][j] = yxtr_[i];
							}
							goto VAR_DONE;
						}
						
						for( size_t i=1; i <= nvar; ++i )
						{
							dy_[i] = yerr_[i];
						}
						
					}
					//std::cerr << "no convergence" << std::endl;
				END:
					h *= REAL(0.5);
					goto INI;
				}
			VAR_DONE:;
			}
			
			
		}
		
		
	}
	
}

