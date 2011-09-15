#include "yocto/math/dat/extrapolate.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto {
	
	
	namespace math {
		
		template <>
		real_t extrapolate<real_t>:: get(size_t               n,
										 const array<real_t> &xa,
										 const array<real_t>  &ya,
										 real_t               &dy,
										 array<real_t>        &c,
										 array<real_t>        &d) throw() 
		{
			assert( n > 0 );
			assert(xa.size()>=n);
			assert(ya.size()== xa.size());
			assert(c.size() == xa.size());
			assert(d.size() == xa.size());
			
			
			
			
#if !defined(NDEBUG)
			for( size_t i=1; i < n; ++i ) {
				assert(xa[i]>xa[i+1]);
			}
			assert(xa[n]>0);
#endif

#define YOCTO_EXTR_SET(INDEX) d[INDEX] = c[INDEX] = ya[INDEX]
			
			YOCTO_LOOP_FUNC_(n,YOCTO_EXTR_SET,1);
			
			size_t ns = n;
			real_t y  = ya[ns--];
			
			for(register size_t m=1;m<n;++m) 
			{
				for(register size_t i=1;i<=n-m;++i)
				{
					const real_t ho  = xa[i]  ;
					const real_t hp  = xa[i+m];
					real_t       den = ho - hp;	    assert(den>0);
					const real_t w   = c[i+1]-d[i];
					
					den  = w/den;
					d[i] = hp*den;
					c[i] = ho*den;
				}
				y += d[ns--];
			}
			dy = d[1];
			return y;
		}
		
		
		
		template <>
		void extrapolate<real_t>:: get( array<real_t>         &yxtr,
									   array<real_t>          &yerr,
									   const array<real_t>    &xa,
									   const matrix<real_t>   &ya,
									   size_t                  n,
									   matrix<real_t>         &c,
									   matrix<real_t>         &d ) throw()
		{
			
			const size_t nvar = yxtr.size();
			assert(nvar>0);
			assert(yerr.size() == nvar);
			
			assert(n>0);
			assert(n<=xa.size());
			assert(ya.rows   == xa.size());
			assert(ya.cols   == nvar);
			assert(c.rows    == ya.rows  );
			assert(d.rows    == ya.rows  );
			assert(c.cols    == ya.cols  );
			assert(d.cols    == ya.cols  );
			
#if !defined(NDEBUG)
			for( size_t i=1; i < n; ++i ) {
				assert(xa[i]>xa[i+1]);
			}
			assert(xa[n]>0);
#endif
			
			for(size_t i=n; i>0; --i ) 
			{
				matrix<real_t>::row       &c_i  = c[i], &d_i = d[i];
				const matrix<real_t>::row &ya_i = ya[i];
				
				for( size_t k=nvar;k>0;--k) 
				{
					c_i[k]  = d_i[k] = ya_i[k];
				}
			}
			size_t ns = n;
			{
				const matrix<real_t>::row &ya_ns = ya[ns];
				for( size_t k=nvar;k>0;--k) {
					yxtr[k] = ya_ns[k];
				}
			}
			--ns;
			
			for(register size_t m=1;m<n;++m) 
			{
				for(register size_t i=1;i<=n-m;++i) 
				{
					const real_t ho  = xa[i]  ;
					const real_t hp  = xa[i+m];
					const real_t den = ho - hp;	   assert(den>0);
					
					matrix<real_t>::row       &c_i  = c[i], &d_i = d[i];
					const matrix<real_t>::row &c_ip = c[i+1];
					
					for( size_t k=nvar;k>0;--k) 
					{
						const real_t w   = c_ip[k]-d_i[k];
						const real_t fac = w/den;
						
						d_i[k] = hp*fac;
						c_i[k] = ho*fac;
					}
					
				}
				
				{
					const matrix<real_t>::row &d_ns = d[ns];
					for( size_t k=nvar;k>0;--k)
						yxtr[k] += d_ns[k];
				}
				--ns;
			}
			
			{
				const matrix<real_t>::row &d_1 = d[1];
				for( size_t k=nvar; k>0; --k )
				{
					yerr[k] = d_1[k];
				}
			}
			
		}
		
	}
}
