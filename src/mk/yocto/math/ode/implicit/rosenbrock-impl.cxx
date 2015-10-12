#include "yocto/math/ztype.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace math
    {
        namespace ode
        {
            template <> STIFF_STEP<real_t>:: STIFF_STEP()
            {
            }
            
            template <> STIFF_STEP<real_t>:: ~STIFF_STEP() throw()
            {
            }
            
            template <>
            void STIFF_STEP<real_t>:: operator()(array<real_t>       &y,
                                                 array<real_t>       &dydx,
                                                 real_t              &x,
                                                 real_t               htry,
                                                 real_t               eps,
                                                 array<real_t>       &yscal,
                                                 real_t              &hdid,
                                                 real_t              &hnext,
                                                 equation            &derivs,
                                                 jacobian            &jacobn)
            {
                static const real_t SAFETY = REAL(0.9);
                static const real_t GROW   = REAL(1.5);
                static const real_t PGROW  = REAL(-0.25);
                static const real_t SHRNK  = REAL(0.5);
                static const real_t PSHRNK = REAL(-1.0)/REAL(3.0);
                static const real_t ERRCON = Pow( GROW/SAFETY, (REAL(1.0)/PGROW) );
                
                const size_t n = y.size();
                assert( n > 0 );
                assert( n == dydx.size() );
                assert( n == yscal.size() );
                assert( n == size      );
                assert( n == dfdy.rows );
                assert( n == dfdy.cols );
                assert( n == a.rows );
                assert( n == a.cols );
                //std::cerr << "** " << STIFF_NAME << ": y=" << y << ", x=" << x << ", htry=" << htry << ", yscal=" << yscal << std::endl;
                //==============================================================
                // Saving data
                //==============================================================
                real_t xsav = x;
                for(size_t i=n;i>0;--i)
                {
                    ysav[i]  =    y[i];
                    dysav[i] = dydx[i];
                }
                
                //==============================================================
                // initial jacobian
                //==============================================================
                jacobn( dfdx,dfdy, xsav, ysav );
                real_t h = htry;
                for( size_t jtry=1;; ++jtry )
                {
                    const real_t __diag = REAL(1.0)/(GAM*h);
                    for( size_t i=n;i>0;--i)
                    {
                        for( size_t j=n; j > 0; --j )
                            a[i][j] = -dfdy[i][j];
                        a[i][i] += __diag;
                    }
                    
                    if( !LU<real_t>::build(a) )
                    {
                        throw exception("%s step: singular jacobian", STIFF_NAME);
                    }
                    
                    for( size_t i=n; i>0 ; --i )
                        g1[i]=dysav[i]+h*C1X*dfdx[i];
                    LU<real_t>::solve(a,g1);
                    for (size_t i=1;i<=n;i++)
                        y[i]=ysav[i]+A21*g1[i];
                    
                    x=xsav+A2X*h;
                    derivs(dydx,x,y);
                    for( size_t i=n; i>0 ; --i )
                        g2[i]=dydx[i]+h*C2X*dfdx[i]+C21*g1[i]/h;
                    LU<real_t>::solve(a,g2);
                    for( size_t i=n; i>0 ; --i )
                        y[i]=ysav[i]+A31*g1[i]+A32*g2[i];
                    
                    x=xsav+A3X*h;
                    derivs(dydx,x,y);
                    for( size_t i=n; i>0 ; --i )
                        g3[i]=dydx[i]+h*C3X*dfdx[i]+(C31*g1[i]+C32*g2[i])/h;
                    LU<real_t>::solve(a,g3);
                    
                    for( size_t i=n; i>0 ; --i )
                        g4[i]=dydx[i]+h*C4X*dfdx[i]+(C41*g1[i]+C42*g2[i]+C43*g3[i])/h;
                    LU<real_t>::solve(a,g4);
                    
                    for( size_t i=n; i>0 ; --i )
                    {
                        y[i]   = ysav[i]+B1*g1[i]+B2*g2[i]+B3*g3[i]+B4*g4[i];
                        err[i] = E1*g1[i]+E2*g2[i]+E3*g3[i]+E4*g4[i];
                    }
                    
                    //-- update x
                    x=xsav+h;
                    if ( Fabs(x - xsav) <= 0 )
                        throw exception("%s: step size not significant", STIFF_NAME);
                    
                    //-- error control
                    real_t errmax=0;
                    for( size_t i=n; i>0 ; --i )
                        errmax=max_of<real_t>(errmax,Fabs(err[i]/yscal[i]));
                    errmax /= eps;
                    if (errmax <= REAL(1.0))
                    {
                        hdid  = h;
                        hnext = (errmax > ERRCON ? SAFETY*h*Pow(errmax,PGROW) : GROW*h);
                        return;
                    }
                    else
                    {
                        hnext=SAFETY*h*Pow(errmax,PSHRNK);
                        h=(h >= REAL(0.0) ? max_of<real_t>(hnext,SHRNK*h) : min_of<real_t>(hnext,SHRNK*h));
                    }
                }
                
            }
            
        }
    }
}
