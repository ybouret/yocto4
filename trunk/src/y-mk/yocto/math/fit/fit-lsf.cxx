#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/linsys.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
			
			template <>
			lsf<real_t>:: ~lsf() throw() {}
			
			template <>
			real_t lsf<real_t>::grad_fn( real_t A )
			{
				assert( func_   );
				assert( iA_   > 0 );
				assert( nvar_ > 0 );
				assert( iA_   <= nvar_ );
				array<real_t> &a    = aorg_;
				const real_t   asav = a[iA_];
				a[iA_] = A;
				try
				{
					const real_t ans = (*func_)( xi_, a );
					a[iA_] = asav;
					return ans;
				}
				catch(...)
				{
					a[iA_] = asav; throw;
				}
			}
			
			template <>
			real_t lsf<real_t>::eval_fn( real_t u )
			{
				for( size_t i=nvar_;i>0;--i)
				{
					atry_[i] = aorg_[i] + u * step_[i];
				}
				real_t ans = 0;
				for( size_t i=ndat_;i>0;--i)
				{
					const real_t xi = samp_->x[i];
					const real_t yi = samp_->y[i];
					const real_t dy = yi - (*func_)( xi, atry_ );
					ans += dy * dy;
				}
				return ans;
			}
			
			template <>
			lsf<real_t>:: lsf() :
			samp_(NULL),
			func_(NULL),
			used_(NULL),
			nvar_( 0 ),
			ndat_( 0 ),
			harr_( 10 ),
			dFda_( harr_.next_array() ),
			beta_( harr_.next_array() ),
			aorg_( harr_.next_array() ),
			atry_( harr_.next_array() ),
			step_( harr_.next_array() ),
			alpha_(),
			curv_(),
			xi_(0),
			iA_(0),
			drvs_(),
			grad_( this, & lsf<real_t>::grad_fn ),
			eval_( this, & lsf<real_t>::eval_fn )
			{
			}
			
			
			template <>
			void lsf<real_t>:: gradient(  real_t xi )
			{
				xi_ = xi;
				size_t &i = iA_;
				for( i=nvar_;i>0;--i ) 
				{
					if( (*used_)[i] )
						dFda_[i] = drvs_( grad_, aorg_[i], 1e-4 );				
				}
			}
			
			
			
			template <>
			real_t lsf<real_t>:: initialize( )
			{
				//--------------------------------------------------------------
				//-- initialize
				//--------------------------------------------------------------
				sample<real_t>    &s    = *samp_;
				field             &f    = *func_;
				const array<bool> &used = *used_;
				
				real_t ans = 0;
				alpha_.ldz();
				for( size_t k=nvar_;k>0;--k)
				{
					beta_[k] = 0;
				}
				
				//--------------------------------------------------------------
				//-- accumulate
				//--------------------------------------------------------------
				for( size_t i=ndat_;i>0;--i)
				{
					const real_t xi = s.x[i];
					const real_t yi = s.y[i];
					const real_t dy = yi - ( s.z[i] = f( xi, aorg_ ) );
					ans += dy * dy;
					gradient( xi );
					for( size_t j=nvar_;j>0;--j)
					{
						if( used[j] )
						{
							const real_t dFda_j = dFda_[j];
							beta_[j] += dy * dFda_j;
							for( size_t k=j;k>0;--k)
								alpha_[j][k] += dFda_j * dFda_[k];
						}
					}
				}
				
				//--------------------------------------------------------------
				//-- regularize
				//--------------------------------------------------------------
				for( size_t j=nvar_;j>0;--j)
				{
					step_[j] = beta_[j];
					if( used[j] )
					{
						for( size_t k=1; k < j; ++k )
							alpha_[k][j] = alpha_[j][k];
					}
					else 
					{
						alpha_[j][j] = 1;
					}
				}
				
				return ans;
			}
			
			
			
			template <>
			void lsf<real_t>::operator()(sample<real_t>     &s,
										 field              &f,
										 array<real_t>      &aorg,
										 const array<bool>  &used,
										 real_t              ftol)
			{
				static const real_t LAMBDA_EXP = Floor( -Log10( numeric<real_t>::epsilon ) );
				static const real_t LAMBDA_MIN = Pow( real_t(10), -LAMBDA_EXP);
				static const real_t LAMBDA_MAX = Pow( real_t(10),  LAMBDA_EXP);
				static const real_t LAMBDA_INI = Pow( real_t(10), Ceil( -LAMBDA_EXP/2) );
				static const real_t LAMBDA_DEC = 0.1;
				static const real_t LAMBDA_INC = 10;
				
				//std::cerr << "LAMBDA_EXP=" << LAMBDA_EXP << std::endl;
				//std::cerr << "LAMBDA_MIN=" << LAMBDA_MIN << std::endl;
				//std::cerr << "LAMBDA_MAX=" << LAMBDA_MAX << std::endl;
				//std::cerr << "LAMBDA_INI=" << LAMBDA_INI << std::endl;
				
				ftol = Fabs( ftol );
				
				//--------------------------------------------------------------
				// fetch data
				//--------------------------------------------------------------
				samp_     = &s;
				func_     = &f;
				used_     = &used;
				nvar_     = aorg.size();
				ndat_     = s.x.size();
				
				if( ndat_ < nvar_ )
					throw exception("LeastSquareFit(NOT ENOUGH DATA)");
				
				//--------------------------------------------------------------
				// sanity check
				//--------------------------------------------------------------
				assert( used.size() == nvar_ );
				assert( s.x.size()  == s.y.size() );
				assert( s.x.size()  == s.z.size() );
				
				//--------------------------------------------------------------
				// memory allocation
				//--------------------------------------------------------------
				alpha_.make(nvar_,nvar_);
				curv_.make(nvar_,nvar_);
				harr_.prepare(  nvar_  );
				linsys<real_t> lss( nvar_ );
				
				//--------------------------------------------------------------
				// global init
				//--------------------------------------------------------------
				for( size_t i=nvar_; i>0; --i ) 
					aorg_[i] = aorg[i];
				
				real_t lam        = LAMBDA_INI;
				real_t &Dold      = s.D;
				bool    converged = false;
				
				while( true )
				{
					//----------------------------------------------------------
					// initial curvature and beta @aorg_
					//----------------------------------------------------------
					Dold=initialize();
					std::cerr << "Dold=" << Dold << std::endl;
					//-- are we done ?
					if( converged )
					{
						std::cerr << "[LeastSquareFit.Converged]" << std::endl;
						const size_t df = ndat_ - nvar_;
						if( df > 0 )
						{
							if( ! lss.LU( alpha_ ) )
							{
								std::cerr << "[LeastSquareFit.Singular2]" << std::endl;
								s.status = fit_failure;
								return;
							}
							curv_.ld1();
							lss( alpha_, curv_ );
							std::cerr << "covar=" << curv_ << std::endl;
						}
						else 
						{
							
						}
						
						s.status = fit_success;
						return;
					}
				RESTART:
					//----------------------------------------------------------
					// try to find a good matrix @aorg_
					//----------------------------------------------------------
					{
						//------------------------------------------------------
						// create the matrix
						//------------------------------------------------------
						const real_t fac = real_t(1) + lam;
						curv_.assign( alpha_ );
						for( size_t i=nvar_;i>0;--i) 
							curv_[i][i] *= fac;
						
						//------------------------------------------------------
						// try to solve it
						//------------------------------------------------------
						if( ! lss.LU( curv_ ) )
						{
							lam *= LAMBDA_INC;
							if( lam > LAMBDA_MAX )
							{
								//-- singular point...
								std::cerr << "[LeastSquareFit.Singular]" << std::endl;
								s.status = fit_failure;
								return;
							}
							goto RESTART;
						}
					}
					
					//----------------------------------------------------------
					// compute the step
					//----------------------------------------------------------
					lss( curv_, step_ );
					
					std::cerr << "aorg="  << aorg_   << std::endl;
					std::cerr << "lam="   << lam    << std::endl;
					//----------------------------------------------------------
					// take the full step
					//----------------------------------------------------------
					real_t Dnew = eval_( 1 ); //-- update atry_
					std::cerr << "Dnew=" << Dnew << std::endl;
					if( Dnew > Dold )
					{
						lam *= LAMBDA_INC;
						if( lam > LAMBDA_MAX )
						{
							std::cerr << "[LeastSquareFit.Spurious]" << std::endl;
							s.status = fit_spurious;
							return;
						}
						goto RESTART;
						
					}
					
					//----------------------------------------------------------
					// direct winner !
					//----------------------------------------------------------
					std::cerr << "[LeastSquareFit.Step.Success]" << std::endl;
					lam *= LAMBDA_DEC;
					if( lam <= LAMBDA_MIN ) lam = LAMBDA_MIN;
					
					//----------------------------------------------------------
					// - at this point, atry_ must be OK and Dnew <= Dold
					// - update parameters and check convergence
					//----------------------------------------------------------
					converged = true;
					for( size_t i=nvar_;i>0;--i)
					{
						const real_t tol =  ftol * ( Fabs( atry_[i] ) + Fabs( aorg_[i] ) );
						if( Fabs(aorg_[i]-atry_[i]) > tol )
							converged = false;
						aorg[i] = aorg_[i] = atry_[i];
					}
					
					
				}
			}
			
			
			
			
			
		}
		
	}
	
}
