#include "yocto/math/sig/data-smoother.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/lu.hpp"
#include "yocto/code/utils.hpp"

namespace yocto {
    
	namespace math {
        
        
		template <>
		smoother<real_t>:: ~smoother() throw()
		{
		}
        
		template <>
		size_t smoother<real_t>:: degree() const throw()
		{
			return degree_;
		}
        
        
		template <>
		smoother<real_t>:: smoother(
                                    const size_t                     nl,
                                    const size_t                     nr,
                                    real_t (*K)(real_t),
                                    const size_t                     d
                                    ) :
        nl_( nl ),
        nr_( nr ),
        np_( nr+nl+1 ),
        weight_( np_, 0),
        degree_( min_of<size_t>(d,np_) ),
        ncoeff_( degree_ + 1 ),
        filter_( ncoeff_, np_),
        dataQ_( np_, as_capacity )
		{
			if( nl_ < 0 )
				throw exception("[smoother] nl=%u overflow", unsigned(nl));
            
			if( nr_ < 0 )
				throw exception("[smoother] nr=%u overflow", unsigned(nr));
            
			//------------------------------------------------------------------
			//-- Compute the weights
			//-- -(nl+0.5) => K(-1)
			//-- +(nr+0.5) => K(1)
			//------------------------------------------------------------------
			for( size_t i=np_;i>0;--i)
			{
				const real_t x = (real_t(i-1) + REAL(0.5))/real_t(np_); //-- reduced position
				const real_t y = (x+x) - REAL(1.0);                     //-- kernel agument
				weight_[i]     = K(y);
			}
            
            //------------------------------------------------------------------
			//
			// compute the matrix of moments
			//
			//------------------------------------------------------------------
			matrix<real_t> mu(ncoeff_,ncoeff_);
            
			for( size_t q=1; q <= ncoeff_; ++q )
			{
				for( size_t p=q; p <= ncoeff_; ++p )
				{
					real_t mu_pq = 0;
					ptrdiff_t j=-nl_;
					for( size_t i=1; i<= np_; ++i,++j)
					{
						real_t j_pow = 1;
						for( size_t k=(p+q-2);k>0;--k)
							j_pow *= j;
						mu_pq += weight_[i] * j_pow;
					}
					mu[p][q] = mu[q][p] = mu_pq;
				}
			}
			//std::cout << "M="   << degree_ << std::endl;
			//std::cout << "nL="  << nl_     << std::endl;
			//std::cout << "nR="  << nr_     << std::endl;
			//std::cout << "mu="  << mu      << std::endl;
            
			//------------------------------------------------------------------
			//
			// LU decomposition
			//
			//------------------------------------------------------------------
			//linsys<real_t> lss( ncoeff_ );
            lu<real_t> LU(ncoeff_);
            vector<real_t> rhs( ncoeff_, 0);
            
			if( !LU.build(mu) )
				throw exception( "[smoother] singular moments, check kernel" );
            
			//------------------------------------------------------------------
			//
			// Compute the modified direct coefficients
			//
			//------------------------------------------------------------------
			for( size_t q=1; q <= ncoeff_; ++q )
			{
				ptrdiff_t j=-ptrdiff_t(nl);
				for( size_t i=1; i <= np_; ++i,++j )
				{
					real_t j_pow = 1;
					for( size_t k=q-1;k>0;--k)
						j_pow *= j;
					filter_[q][i] = weight_[i] * j_pow;
				}
			}
            
			//std::cout << "phi=" << filter_  << std::endl;
			//------------------------------------------------------------------
			//
			// Compute the filter coefficient coefficients
			//
			//------------------------------------------------------------------
			for( size_t j=1; j <= np_; ++j )
			{
				//-- extract column j of filter
				for( size_t i=ncoeff_; i>0; --i )
				{
					rhs[i] = filter_[i][j];
				}
                
				//-- solve using internal solver memory
                LU.solve( mu, rhs );
                
				//-- replace column j of filter
				for( size_t i=ncoeff_; i>0; --i )
				{
					filter_[i][j] = rhs[i];
				}
                
			}
			//std::cerr << "filter=" << filter_  << std::endl;
            
		}
        
        
		template <>
		void smoother<real_t>:: run(array<real_t>           &odata,
                                    const extender<real_t>  &idata,
                                    const size_t             level) const
		{
            
			if( level >= degree_ )
				throw exception("[smoother.run] level=%u exceeds degree=%u", unsigned(level), unsigned(degree_) );
            
            //------------------------------------------------------------------
			//-- pre-fill the Q
            //------------------------------------------------------------------
			dataQ_.free();
			ptrdiff_t j=1-nl_;
			for( ;j<=nr_;++j)
				dataQ_.push_back( idata[j] );
            assert( dataQ_.size() == np_-1 );
            
            //------------------------------------------------------------------
			//-- select the coefficients
            //------------------------------------------------------------------
			const matrix<real_t>::row &coeff = filter_[level+1];
            
            //------------------------------------------------------------------
			//-- run over odata size
            //------------------------------------------------------------------
			const size_t n = odata.size();
			for( size_t i=1; i <= n; ++i,++j )
			{
				dataQ_.push_back( idata[j] );
				assert( dataQ_.size() == np_ );
				real_t ans = 0;
                list<real_t>::reverse_iterator input = dataQ_.rbegin();
				for( size_t k=np_;k>0;--k,++input)
                {
					ans += (*input) * coeff[k];
                }
				odata[i] = ans;
				dataQ_.pop_front();
			}
            
		}
        
		template <>
		real_t smoother<real_t>:: uniform( real_t x )
		{
			return (x < -1) || (x > 1) ? REAL(0.0) : REAL(1.0);
		}
        
		template <>
		real_t smoother<real_t>:: triangle( real_t x )
		{
			return   (x < -1) || (x > 1) ? 0 : (REAL(1.0)- Fabs(x));
		}
        
		template <>
		real_t smoother<real_t>:: quadratic( real_t x )
		{
			return  (x < -1) || (x > 1) ? 0 : REAL(0.75)*(REAL(1.0)-x*x);
		}
        
		template <>
		real_t smoother<real_t>:: quartic( real_t x )
		{
			if( (x < -1) || (x > 1) )
				return 0;
			else
			{
				const real_t arg = REAL(1.0)-x*x;
				return REAL(15.0)/REAL(16.0)*arg*arg;
			}
		}
        
		template <>
		real_t smoother<real_t>:: triweight( real_t x )
		{
			if( (x < -1) || (x > 1)  )
				return 0;
			else
			{
				const real_t arg = REAL(1.0)-x*x;
				return REAL(35.0)/REAL(32.0)*arg*arg*arg;
			}
		}
        
		template <>
		real_t smoother<real_t>:: gaussian( real_t x )
		{
			return Exp( -x*x*REAL(0.5) ) / Sqrt( numeric<real_t>::two_pi );
		}
        
		template <>
		real_t smoother<real_t>:: cosine( real_t x )
		{
			return  (x < -1) || (x > 1) ? 0 : (REAL(0.25)*numeric<real_t>::pi) * Cos( REAL(0.5) * numeric<real_t>::pi * x );
		}
        
        
	}
    
}
