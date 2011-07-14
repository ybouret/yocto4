#include "yocto/math/fcn/zroot.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
	
	namespace math 
	{
		
		static inline real_t
		normsq( const array<real_t> &F )
		{
			real_t f2 = 0;
			for( size_t i= F.size();i>0;--i)
			{
				const real_t fi = F[i];
				f2 += fi*fi;
			}
			return REAL(0.5) * f2;
		}
		
		template <>
		real_t zroot<real_t>:: hF2_( real_t lambda )
		{
			for( size_t i=xcur_.size();i>0;--i)
			{
				xnew_[i] = xcur_[i] + lambda * step_[i];
			}
			(*pF_)( fnew_, xnew_ );
			return normsq( fnew_ );
		}
		
		template <>
		zroot<real_t>:: ~zroot() throw() {}
		
		template <>
		zroot<real_t>:: zroot() :
		ftol(0),
		xcur_(),
		fcur_(),
		step_(),
		xnew_(),
		fnew_(),
		jac_(),
		lss_(),
		pF_(NULL),
		lmin_( this, & zroot<real_t>::hF2_ )
		{}
		
		
		template <>
		size_t zroot<real_t>::  size() const throw()
		{
			return xcur_.size();
		}
		
		template <>
		void zroot<real_t>:: release() throw()
		{
			jac_.release();
			fnew_.release();
			xnew_.release();
			step_.release();
			fcur_.release();
			xcur_.release();
			
		}
		
		
		template <>
		void zroot<real_t>:: acquire( size_t n )
		{
			if( n > 0 )
			{
				try {
					xcur_.make(n,0);
					fcur_.make(n,0);
					step_.make(n,0);
					xnew_.make(n,0);
					fnew_.make(n,0);
					jac_.make(n,n);
					lss_.ensure(n);
				}
				catch (...) {
					release();
					throw;
				}
			}
			else 
			{
				release();
			}
			
		}
		
		
		
		
		template <>
		bool zroot<real_t>::  operator()(  numeric<real_t>::vector_field &F, array<real_t> &X , const real_t h, jacobian<real_t> & dFdX )
		{
			//==================================================================
			// Initialize
			//==================================================================
			const size_t nvar = X.size(); assert( nvar == this->size() );
			const real_t FTOL = max_of<real_t>( Fabs(ftol), numeric<real_t>:: ftol );
			for( size_t i=1; i <= nvar; ++i )
			{
				xcur_[i] = X[i];
			}
			pF_ = &F;
			
			F(fcur_,xcur_);
			real_t f0 = normsq(fcur_);
			
			for(;;)
			{
				//--------------------------------------------------------------
				// compute step
				//--------------------------------------------------------------
				dFdX( jac_, F, xcur_, h );
				//std::cerr << "J=" << jac_ << std::endl;
				if( ! lss_.LU( jac_ ) )
				{
					return false; // singular !
				}
				
				for( size_t i=1; i <= nvar; ++i)
				{
					step_[i] = -fcur_[i];
				}
				lss_( jac_, step_ );
				//std::cerr << "step=" << step_ << std::endl;
				
				//--------------------------------------------------------------
				// take full step
				//--------------------------------------------------------------
				for( size_t i=1; i <= nvar; ++i )
				{
					xnew_[i] = xcur_[i] + step_[i];
					
				}
				
				//--------------------------------------------------------------
				// Evaluate
				//--------------------------------------------------------------
				F( fnew_, xnew_ );
				real_t f1 = normsq(fnew_);
				//std::cerr << f0 << " -> " << f1 << std::endl;
				
				if( f1 <= f0 )
				{
					// successfull full step
					bool has_converged = true;
					for( size_t i=1; i <= nvar; ++i )
					{
						if( Fabs( step_[i] ) > FTOL * ( Fabs( xnew_[i] ) + Fabs( xcur_[i] ) ) )
							has_converged = false;
					}
					
					if( has_converged )
						goto SUCCESS;
				}
				else 
				{
					//std::cerr << "backtrack !" << std::endl;
					// f1 > f0
					triplet<real_t> XX = { 0,  0,  1 };
					triplet<real_t> FF = { f0, 0, f1 };
					if( ! bracket<real_t>::inside( lmin_, XX, FF ) )
					{
						//std::cerr << "spurious..." << std::endl;
						return false; //-- spurious ?
					}
					//std::cerr << "minimizing with XX= " << XX << " and FF=" << FF << std::endl;
					minimize<real_t> opt = { FTOL };
					opt( lmin_, XX, FF );
					//std::cerr << "                XX= " << XX << " and FF=" << FF << std::endl;
					f1 = lmin_( XX.b );
				}
				
				
				//--------------------------------------------------------------
				// update
				//--------------------------------------------------------------
				f0 = f1;
				for( size_t i=1; i <= nvar; ++i )
				{
					xcur_[i] = xnew_[i];
					fcur_[i] = fnew_[i];
				}
				
			} 
			
		SUCCESS:
			for( size_t i=1; i <= nvar; ++i )
			{
				X[i] = xnew_[i];
			}
			return true;
		}
		
		
	}
}
