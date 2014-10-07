#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/network.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto {
    
    namespace math {
        
#if 0
        static inline
        real_t __getErr( real_t f0, real_t f1, real_t f2, real_t f3 ) throw()
        {
            netsort<real_t>::level4(f0, f1, f2, f3);
            return Fabs(f2-f0);
        }
#endif
        
#define XTOL numeric<real_t>::sqrt_ftol
        
        static inline
        real_t __compute_ftol(real_t ftol) throw()
        {
            return max_of(Fabs(ftol),XTOL);
        }
        
        
        template <>
        void minimize<real_t>(numeric<real_t>::function &func,
                              triplet<real_t>           &x,
                              triplet<real_t>           &f,
                              real_t                     ftol )
        {
            assert(x.is_ordered());
            //__________________________________________________________________
            //
            // ensure left to right
            //__________________________________________________________________
            netsort<real_t>::co_level3<real_t>( &x.a, &f.a);
            assert(x.a<=x.b);
            assert(x.b<=x.c);
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            
            static const real_t R    = REAL(0.618033988749895);
            static const real_t C    = REAL(1.0)-R;
            
            ftol = __compute_ftol(ftol)/2;
            
            //__________________________________________________________________
            //
            // Prepare control points
            //__________________________________________________________________
            real_t       x0   = x.a, x1, x2, x3 = x.c;
            real_t       f0   = f.a, f1, f2, f3 = f.c;
            real_t       dx   = 0;
            {
                const real_t bc = max_of<real_t>(x.c - x.b,0);
                const real_t ab = max_of<real_t>(x.b - x.a,0);
                if(bc>ab)
                {
                    x1 = x.b;
                    f1 = f.b;
                    x2 = min_of<real_t>(x.b + C * bc,x3);
                    f2 = func( x2 );
                    dx = max_of<real_t>(x2-x0,0);
                }
                else
                {
                    x2 = x.b;
                    f2 = f.b;
                    x1 = max_of<real_t>(x.b - C * ab,x0);
                    f1 = func( x1 );
                    dx = max_of<real_t>(x3-x1,0);
                }
                assert(x0<=x1); assert(x1<=x2); assert(x2<=x3);
            }
            assert( (f1<=f0&&f1<=f3) || (f2<=f0&&f2<=f3) );
            
            //__________________________________________________________________
            //
            // Main Loop
            //__________________________________________________________________
            
            while( dx > max_of(ftol*(Fabs(x1)+Fabs(x2)),XTOL) )
            {
                assert( (f1<=f0&&f1<=f3) || (f2<=f0&&f2<=f3) );
                if( f2 < f1 )
                {
                    
                    
                    x0 = x1;
                    f0 = f1;
                    
                    x1 = x2;
                    f1 = f2;
                    
                    
                    x2 = min_of<real_t>(R*x1+C*x3,x3);
                    f2 = func(x2);
                    assert(x0<=x1); assert(x1<=x2); assert(x2<=x3);
                    dx = max_of<real_t>(0,x3-x1);
                    
                }
                else
                {
                    assert(f1<=f2);
                    
                    
                    x3 = x2;
                    f3 = f2;
                    
                    x2 = x1;
                    f2 = f1;
                    
                    x1 = max_of<real_t>(C*x0+R*x2,x0);
                    f1 = func(x1);
                    assert(x0<=x1); assert(x1<=x2); assert(x2<=x3);
                    dx=max_of<real_t>(0,x2-x0);
                }
                
                
                //const real_t new_err = __getErr(f0,f1,f2,f3);
                //std::cerr << x0 << ' ' << x1 << ' ' << x2 << ' ' << x3 << std::endl;
                //std::cerr << f0 << ' ' << f1 << ' ' << f2 << ' ' << f3 << std::endl;
                //std::cerr << "Error=" << new_err << std::endl;
                //dx = max_of<real_t>(x3-x0,0);
            }
            
            //__________________________________________________________________
            //
            // best numeric presentation
            //__________________________________________________________________
            if( f1 <= f2 )
            {
                x.a = x0; f.a = f0;
                x.b = x1; f.b = f1;
                x.c = x2; f.c = f2;
            }
            else
            {
                x.a = x1; f.a = f1;
                x.b = x2; f.b = f2;
                x.c = x3; f.c = f3;
            }
            
        }
        
        
        namespace
        {
            
            
            static inline
            void minstep2(numeric<real_t>::function &func,
                          triplet<real_t>           &x,
                          triplet<real_t>           &f)
            {
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                const real_t ab = max_of<real_t>(x.b - x.a,0);
                const real_t bc = max_of<real_t>(x.c - x.b,0);
                if(bc>ab)
                {
                    //__________________________________________________________
                    //
                    // probe at right
                    //__________________________________________________________
                    const real_t xu = clamp(x.b,REAL(0.5)*(x.b+x.c),x.c);
                    const real_t fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move c->u
                        x.c = xu;
                        f.c = fu;
                    }
                    else
                    {
                        // move a->b, b->u
                        x.a = x.b;
                        f.a = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // probe at left
                    //__________________________________________________________
                    const real_t xu = clamp(x.a,REAL(0.5)*(x.a+x.b),x.b);
                    const real_t fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move a->u
                        x.a = xu;
                        f.a = fu;
                    }
                    else
                    {
                        // move c->b, b->u
                        x.c = x.b;
                        f.c = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
            }
        }
        
        template <>
        void minimize2<real_t>(numeric<real_t>::function &func,
                               triplet<real_t>           &x,
                               triplet<real_t>           &f,
                               real_t                     ftol )
        {
            assert(x.is_ordered());
            netsort<real_t>::co_level3<real_t>( &x.a, &f.a);
            assert(x.a<=x.b);
            assert(x.b<=x.c);
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            ftol =  __compute_ftol(ftol);
            
            while( max_of<real_t>(x.c-x.a,0) > max_of(ftol*Fabs(x.b),XTOL) )
            {
                minstep2(func,x,f);
            }
            
        }
        
#if 0
        template <>
        void minimize3<real_t>(numeric<real_t>::function &func,
                               triplet<real_t>           &x,
                               triplet<real_t>           &f,
                               real_t                     xtol )
        {
            
            assert(x.is_ordered());
            netsort<real_t>::co_level3<real_t>( &x.a, &f.a);
            assert(x.a<=x.b);
            assert(x.b<=x.c);
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            
            xtol = __compute_xtol(xtol);
            
            real_t ac=0;
            while((ac=max_of<real_t>(x.c-x.a,0))>xtol)
            {
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                const real_t ab  = max_of<real_t>(x.b-x.a,0);
                const real_t bc  = max_of<real_t>(x.c-x.b,0);
                const real_t lam = (f.a-f.b)*ac + (f.c-f.a) * ab;
                std::cerr << "lam=" << lam << std::endl;
                if(lam<=0)
                {
                    // not really a max
                    break;
                }
                const real_t mu   = (f.a-f.c)*ab*bc;
                real_t       fu   = 0;
                const real_t xu   = clamp(x.a, REAL(0.5)*((x.a+x.c) +  mu/lam), x.c);
                fu = func(xu);
                
                std::cerr << "xu=" << xu << ", fu=" << fu << std::endl;
                if(fu>=f.b)
                {
                    // b stays the winner
                    if(xu<=x.b)
                    {
                        x.a = xu;
                        f.a = fu;
                    }
                    else
                    {
                        x.c = xu;
                        f.c = fu;
                    }
                }
                else
                {
                    // u is the new winner
                    if(xu<=x.b)
                    {
                        x.c = x.b;
                        f.c = f.b;
                    }
                    else
                    {
                        x.a = x.b;
                        f.a = f.b;
                    }
                    x.b = xu;
                    f.b = fu;
                }
                std::cerr << "x=" << x << ", f=" << f << ", w=" << x.c-x.a << std::endl;
            }
            
        }
#endif
        
    } // math
    
} //yocto
