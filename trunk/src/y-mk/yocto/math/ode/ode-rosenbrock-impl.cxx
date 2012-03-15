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
                static const real_t SAFETY = 0.9;
                static const real_t GROW   = 1.5;
                static const real_t PGROW  = -0.25;
                static const real_t SHRNK  = 0.5;
                static const real_t PSHRNK = (-1.0/3.0);
                static const real_t ERRCON = Pow( GROW/SAFETY, (1/PGROW) );
                //std::cerr << "ERRCON = " << ERRCON << std::endl;
                
                const size_t n = y.size();
                assert( n > 0 );
                assert( n == dydx.size() );
                assert( n == yscal.size() );
                assert( n == common_size() );
                assert( n == dfdy.rows );
                assert( n == dfdy.cols );
                assert( n == a.rows );
                assert( n == a.cols );
                
                //==============================================================
                // Saving data
                //==============================================================
                real_t xsav = x;
                for(size_t i=1;i<=n;i++) 
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
                    const real_t __diag = 1.0/(GAM*h);
                    for( size_t i=1;i<=n;++i)
                    {
                        for( size_t j=1; j <= n; ++j )
                            a[i][j] = -dfdy[i][j];
                        a[i][i] += __diag;
                    }
                    
                    if( !lss.LU(a) )
                    {
                        throw exception("singular jacobian");
                    }
                    
                    for( size_t i=1; i<=n ; ++i )
                        g1[i]=dysav[i]+h*C1X*dfdx[i];
                    lss(a,g1);
                    for (size_t i=1;i<=n;i++)
                        y[i]=ysav[i]+A21*g1[i];
                    
                    x=xsav+A2X*h;
                    derivs(dydx,x,y);
                    for(size_t i=1; i<=n; ++i )
                        g2[i]=dydx[i]+h*C2X*dfdx[i]+C21*g1[i]/h;
                    lss(a,g2);
                    for(size_t i=1; i<=n; ++i )
                        y[i]=ysav[i]+A31*g1[i]+A32*g2[i];
                    
                    x=xsav+A3X*h;
                    derivs(dydx,x,y);
                    for(size_t i=1; i<=n; ++i )
                        g3[i]=dydx[i]+h*C3X*dfdx[i]+(C31*g1[i]+C32*g2[i])/h;
                    lss(a,g3);
                    
                    for(size_t i=1; i<=n; ++i )
                        g4[i]=dydx[i]+h*C4X*dfdx[i]+(C41*g1[i]+C42*g2[i]+C43*g3[i])/h;
                    lss(a,g4);
                    
                    for( size_t i=1; i <= n; ++i )
                    {
                        y[i]=ysav[i]+B1*g1[i]+B2*g2[i]+B3*g3[i]+B4*g4[i];
                        err[i]=E1*g1[i]+E2*g2[i]+E3*g3[i]+E4*g4[i];
                    }
                    
                    x=xsav+h;
                    if ( Fabs(x - xsav) <= 0 )
                        throw exception("stepsize not significant in stiff");
                    
                    real_t errmax=0;
                    for(size_t i=1;i<=n;i++) 
                        errmax=max_of<real_t>(errmax,Fabs(err[i]/yscal[i]));
                    //std::cerr << "errmax=" << errmax << std::endl;
                    errmax /= eps;
                    //std::cerr << "errmax=" << errmax << std::endl;
                    if (errmax <= 1.0)
                    {
                        hdid  = h;
                        hnext = (errmax > ERRCON ? SAFETY*h*Pow(errmax,PGROW) : GROW*h);
                        return;
                    }
                    else 
                    {
                        hnext=SAFETY*h*Pow(errmax,PSHRNK);
                        h=(h >= 0.0 ? max_of<real_t>(hnext,SHRNK*h) : min_of<real_t>(hnext,SHRNK*h));
                    }
                    //if( jtry >= 10 ) exit(1);
                }
                
            }
            
        }
    }
}
