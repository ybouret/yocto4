#include "yocto/math/kernel/jacobi.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/multi-arrays.hpp"

namespace yocto {
	
	namespace math {
	        
#define _YOCTO_JACOBI(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau)
		
		static inline bool almost_equal( const real_t X, const real_t Y ) throw() {
			static real_t fac = REAL(0.5) * REAL_EPSILON;
			return ( Fabs(X-Y) <= fac * ( Fabs(X) + Fabs(Y) ) );
		}
		
		template <>
		bool jacobi<real_t>:: build( matrix<real_t> &a, array<real_t> &d, matrix<real_t> &v )
        {
			assert(a.rows>0);
			assert(a.cols == a.rows);
			assert(d.size() == a.rows);
			
			const size_t   n = a.rows;
            size_t         nrot_ = 0;
            vector<real_t> b(n,numeric<real_t>::zero);
            vector<real_t> z(n,numeric<real_t>::zero);
          			
			
            //==================================================================
			// initialize eigenvectors
            //==================================================================
			for(size_t ip=1;ip<=n; ++ip)
            {
				for(size_t iq=1;iq<=n;++iq)
					v[ip][iq]=REAL(0.0);
				v[ip][ip]=REAL(1.0);
			}
			
            //==================================================================
			// initialize workspace
            //==================================================================
			for(size_t ip=1;ip<=n; ++ip)
            {
				b[ip]=d[ip]=a[ip][ip];
				z[ip]=REAL(0.0);
			}
			
			nrot_ = 0;
			
            //==================================================================
			// looping over sweeps
            //==================================================================
			for(size_t iter=1;iter<=64;++iter)
            {
				real_t sm = 0;
				for(size_t ip=1;ip<n;++ip) {
					for(size_t iq=ip+1;iq<=n; ++iq)
						sm += Fabs(a[ip][iq]);
				}
				if (sm <= REAL_MIN )
                {
					return true; // OK
				}
				
				const real_t tresh= iter < 4 ? REAL(0.2)*sm/(n*n) : REAL(0.0);
				
				
				for(size_t ip=1;ip<n;++ip) {
					for(size_t iq=ip+1;iq<=n;iq++) {
						real_t g=REAL(100.0) * Fabs(a[ip][iq]);
						if (iter > 4 && almost_equal( Fabs(d[ip])+g, Fabs(d[ip]))
						    && almost_equal( Fabs(d[iq])+g, Fabs(d[iq])) )
						{
							a[ip][iq]=REAL(0.0);
						}
						else
							if (Fabs(a[ip][iq]) > tresh) {
								real_t h = d[iq]-d[ip];
								real_t t = 0;
								if ( almost_equal(Fabs(h)+g,Fabs(h)) )
									t=(a[ip][iq])/h;
								else {
									const real_t theta=REAL(0.5)*h/(a[ip][iq]);
									t=REAL(1.0)/(Fabs(theta)+Sqrt(REAL(1.0)+theta*theta));
									if (theta < 0.0)
										t = -t;
								}
								const real_t c   = REAL(1.0)/Sqrt(REAL(1.0)+t*t);
								const real_t s   = t*c;
								const real_t tau = s/(REAL(1.0)+c);
								h=t*a[ip][iq];
								z[ip] -= h;
								z[iq] += h;
								d[ip] -= h;
								d[iq] += h;
								a[ip][iq]=REAL(0.0);
								for(size_t j=1;j<ip;++j) {
									_YOCTO_JACOBI(a,j,ip,j,iq);
								}
								for(size_t j=ip+1;j<iq;++j) {
									_YOCTO_JACOBI(a,ip,j,j,iq);
								}
								for(size_t j=iq+1;j<=n;++j) {
									_YOCTO_JACOBI(a,ip,j,iq,j);
								}
								for(size_t j=1;j<=n;++j) {
									_YOCTO_JACOBI(v,j,ip,j,iq);
								}
								++nrot_;
							}
					}
				}
				for(size_t ip=1;ip<=n;++ip) {
					b[ip] += z[ip];
					d[ip] = b[ip];
					z[ip] = REAL(0.0);
				}
			}
			return false;
		}
		
		template <>
		void jacobi<real_t>:: eigsrt( array<real_t> &d, matrix<real_t> &v) throw()
		{
			const size_t n = v.rows;
			assert( d.size() >= v.rows );
			for (size_t i=1;i<n;i++) 
			{
				size_t k = i;
				real_t p = d[k];
				for (size_t j=i+1;j<=n;j++)
					if (d[j] >= p)
						p=d[k=j];
				if (k != i) {
					d[k]=d[i];
					d[i]=p;
					for(size_t j=1;j<=n;j++) {
						const real_t tmp=v[j][i];
						v[j][i]=v[j][k];
						v[j][k]=tmp;
					}
				}
			}
		}
		
		
	}
	
}
