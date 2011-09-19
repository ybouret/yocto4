#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/ztype.hpp"

namespace yocto
{
	namespace math
	{
		
		namespace fit
		{
			
			template <>
			lsf<real_t>:: ~lsf() throw() {}
			
			template <>
			lsf<real_t>:: lsf() :
			nvar_( 0 ),
			ndat_( 0 ),
			harr_( 10 ),
			dFda_( harr_.next_array() ),
			beta_( harr_.next_array() ),
			aorg_( harr_.next_array() ),
			atry_( harr_.next_array() ),
			alpha_(),
			drvs_()
			{
			}
			
			namespace  {
				struct gradient_context
				{
					lsf<real_t>::field *pF;
					array<real_t>      *pA;
					size_t				iA;
					real_t              xi;
					real_t eval( real_t A )
					{
						assert( pF );
						assert( pA );
						assert( iA <= pA->size() );
						array<real_t> &a    = (*pA);
						const real_t   asav = a[iA];
						a[iA] = A;
						try
						{
							const real_t ans = (*pF)( xi, a );
							a[iA] = asav;
							return ans;
						}
						catch(...)
						{
							a[iA] = asav; throw;
						}
						
					}
				};
			}
			
			template <>
			void lsf<real_t>:: gradient( field &func, real_t xi, array<real_t> &a, const array<bool> &used )
			{
				assert( a.size() == nvar_ );
				gradient_context          grad_ctx = { &func, &a, 0, xi };
				numeric<real_t>::function G( &grad_ctx, & gradient_context::eval );
				size_t &i = grad_ctx.iA;
				for( i=nvar_;i>0;--i ) 
				{
					if( used[i] )
						dFda_[i] = drvs_( G, a[i], 1e-4 );				
				}
			}
			
			
			template <>
			void lsf<real_t>:: initialize( sample<real_t> &s, field &f, const array<bool> &used )
			{
				//-- initialize
				s.D = 0;
				alpha_.ldz();
				for( size_t k=nvar_;k>0;--k)
				{
					beta_[k] = 0;
				}
				
				//-- accumulate
				for( size_t i=ndat_;i>0;--i)
				{
					const real_t xi = s.x[i];
					const real_t yi = s.y[i];
					const real_t dy = yi - f( xi, aorg_ );
					s.D += dy * dy;
					gradient( f, xi, aorg_, used );
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
				
				//-- regularize	
				for( size_t j=nvar_;j>0;--j)
				{
					if( used[j] )
					{
						for( size_t k=1; k < j; ++k )
							alpha_[k][j] = alpha_[j][k];
					}
					else {
						alpha_[j][j] = 1;
					}
					
				}
			}
			
			
			
			template <>
			void lsf<real_t>::operator()(sample<real_t>     &s,
										 field              &f,
										 array<real_t>      &aorg,
										 const array<bool>  &used)
			{
				//--------------------------------------------------------------
				// fetch data
				//--------------------------------------------------------------
				nvar_     = aorg.size();
				ndat_     = s.x.size();
				
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
				harr_.prepare(  nvar_  );
				
				
				//--------------------------------------------------------------
				// global init
				//--------------------------------------------------------------
				for( size_t i=nvar_; i>0; --i ) 
					aorg_[i] = aorg[i];
				
				
				initialize(s,f,used);
				std::cerr << "aorg=" << aorg_ << std::endl;
				std::cerr << "beta=" << beta_ << std::endl;
				std::cerr << "alpha=" << alpha_ << std::endl;
			}
			
			
			
			
			
		}
		
	}
	
}
