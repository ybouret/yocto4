#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/network.hpp"

//#include <iostream>

namespace yocto {
    
    namespace math {
        
#if 0
        static inline
        real_t compute_curv(real_t x0, real_t x1, real_t x2, real_t x3,
                            real_t f0, real_t f1, real_t f2, real_t f3)
        {
            const real_t xm = (x3+x0)/2;
            const real_t dx0 = x0-xm;
            const real_t dx1 = x1-xm;
            const real_t dx2 = x2-xm;
            const real_t dx3 = x3-xm;
            
            const real_t dx02 = Square(dx0);
            const real_t dx12 = Square(dx1);
            const real_t dx22 = Square(dx2);
            const real_t dx32 = Square(dx3);
            
            
            const real_t s2   = dx02 + dx12 + dx22 + dx32;
            const real_t s4   = Square(dx02) + Square(dx12) + Square(dx22) + Square(dx32);
            const real_t sf   = f0+f1+f2+f3;
            const real_t sfs2 = f0 * dx02 + f1 * dx12 + f2 * dx22 + f3 * dx32;
            
            const real_t mu = (4*sfs2 - s2 * sf)/(3*s4);
            
            return mu;
            
        }
#endif
        
        
        static inline
        real_t __getErr( real_t f0, real_t f1, real_t f2, real_t f3 )
        {
            netsort<real_t>::level4(f0, f1, f2, f3);
            return Fabs(f2-f0);
        }
        
        static inline
        real_t __getErr( real_t f0, real_t f1, real_t f2 )
        {
            netsort<real_t>::level3(f0, f1, f2);
            return Fabs(f2-f0);
        }
        
        template <>
        void minimize<real_t>(numeric<real_t>::function &func,
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
            
            static const real_t R    = REAL(0.618033988749895);
            static const real_t C    = REAL(1.0)-R;
            
            xtol = max_of(Fabs(xtol),numeric<real_t>::ftol);
            
            
            //__________________________________________________________________
            //
            // Prepare control points
            //__________________________________________________________________
            
            real_t       x0   = x.a, x1, x2, x3 = x.c;
            real_t       f0   = f.a, f1, f2, f3 = f.c;
            {
                const real_t bc = max_of<real_t>(x.c - x.b,0);
                const real_t ab = max_of<real_t>(x.b - x.a,0);
                if(bc>ab)
                {
                    x1 = x.b;
                    f1 = f.b;
                    x2 = min_of<real_t>(x.b + C * bc,x3);
                    f2 = func( x2 );
                }
                else
                {
                    x2 = x.b;
                    f2 = f.b;
                    x1 = max_of<real_t>(x.b - C * ab,x0);
                    f1 = func( x1 );
                }
                assert(x0<=x1); assert(x1<=x2); assert(x2<=x3);
                assert(Fabs(x0-x1)<=Fabs(x2-x3));
            }
            assert( (f1<=f0&&f1<=f3) || (f2<=f0&&f2<=f3) );
            
            //__________________________________________________________________
            //
            // Main Loop
            //__________________________________________________________________
            real_t w = 0;
			real_t E = __getErr(f0,f1,f2,f3);
            while( (w=max_of<real_t>(x3-x0,0)) > xtol )
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
                }
                const real_t new_err = __getErr(f0,f1,f2,f3);
                
                std::cerr << "Error=" << new_err << std::endl;
                
                
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
        
        template <>
        void minimize2<real_t>(numeric<real_t>::function &func,
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
            xtol = max_of(Fabs(xtol),numeric<real_t>::ftol);
            
            while(max_of<real_t>(x.c-x.a,0)>xtol)
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
                std::cerr << "Error2=" << __getErr(f.a, f.b, f.c) << std::endl;
                //std::cerr << "x=" << x << std::endl;
                //std::cerr << "f=" << f << std::endl;
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
            real_t ac = 0;
            xtol = max_of(Fabs(xtol),numeric<real_t>::ftol);
            
            size_t count = 0;
            while((ac=max_of<real_t>(x.c-x.a,0))>xtol)
            {
                const real_t ab    = max_of<real_t>(x.b - x.a,0);
                const real_t beta  = ab/ac;
                const real_t f1mf0 = f.c - f.a;
                const real_t den   = f1mf0*beta + (f.a-f.b);
                std::cerr << "den=" << den << std::endl;
                if(den<=0)
                {
                    break;
                }
                const real_t num = -f1mf0 * beta * (REAL(1.0)-beta);
                std::cerr << "num=" << num << std::endl;
                real_t u = 0;
                if(num<=-den)
                {
                    u = 0;
                }
                else
                {
                    if(num>=den)
                    {
                        u = 1;
                    }
                    else
                    {
                        u = clamp<real_t>(0,REAL(0.5)*(REAL(1.0) + num/den),1);
                    }
                }
                std::cerr << "u=" << u << std::endl;
                const real_t xu = clamp(x.a,x.a + ac * u,x.c);
                const real_t fu = func(xu);
                std::cerr << "xu=" << xu << ", fu=" << fu << std::endl;
                if(fu>=f.b)
                {
                    // b is still the winner
                    if(xu<=x.b)
                    {
                        std::cerr << "\tmove a->u" << std::endl;
                        x.a = xu;
                        f.a = fu;
                    }
                    else
                    {
                        std::cerr << "\tmove c->u" << std::endl;
                        x.c = xu;
                        f.c = fu;
                    }
                    assert(x.a<=x.b);
                    assert(x.b<=x.c);
                    assert(f.b<=f.a);
                    assert(f.b<=f.c);
                }
                else
                {
                    // u is the new winner
                    std::cerr << "NEW WINNER" << std::endl;
                    if(xu<=x.b)
                    {
                        std::cerr << "\tmove c->b" << std::endl;
                        x.c = x.b;
                        f.c = f.b;
                    }
                    else
                    {
                        std::cerr << "\tmove a->b" << std::endl;
                        x.a = x.b;
                        f.a = f.b;
                    }
                    std::cerr << "\tmove b->u" << std::endl;
                    x.b = xu;
                    f.b = fu;
                    assert(x.a<=x.b);
                    assert(x.b<=x.c);
                    assert(f.b<=f.a);
                    assert(f.b<=f.c);
                }
                std::cerr << "x=" << x << std::endl;
                std::cerr << "f=" << f << std::endl;
                if(++count>10)
                    break;
            }
            
            
            
            
        }
#endif
        
        
    } // math
    
} //yocto
