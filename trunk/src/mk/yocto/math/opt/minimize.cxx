#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/sort/network.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto {
    
    namespace math {
        
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
                    dx = max_of<real_t>(0,x2-x0);
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
        
        namespace {
            
            static inline
            void minstep3(numeric<real_t>::function &func,
                          triplet<real_t>           &x,
                          triplet<real_t>           &f)
            {
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                std::cerr << "min3: " << f << " @ " << x << std::endl;
                const real_t ab    = max_of<real_t>(x.b - x.a,0);
                const real_t bc    = max_of<real_t>(x.c - x.b,0);
                const real_t famfb = max_of<real_t>(f.a - f.b,0);
                const real_t fcmfb = max_of<real_t>(f.c - f.b,0);
                const real_t mu    = bc*famfb+ab*fcmfb;
                const real_t den   = mu+mu; assert(den>=0);
                std::cerr << "curv=" << den << std::endl;
                if(ab<=0||bc<=0||den<=0)
                {
                    std::cerr << "failsafe step" << std::endl;
                    //__________________________________________________________
                    //
                    // take a naive step
                    //__________________________________________________________
                    if(bc>ab)
                    {
                        //______________________________________________________
                        //
                        // probe at right
                        //______________________________________________________
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
                        //______________________________________________________
                        //
                        // probe at left
                        //______________________________________________________
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
                else
                {
                    //__________________________________________________________
                    //
                    // take a parabolic step
                    //__________________________________________________________
                    std::cerr << "parabolic step" << std::endl;
                    const real_t num = bc*bc*famfb - ab*ab*fcmfb;
                    
                    // xu = xb + num/den
                    if(num <= -ab*den)
                    {
                        std::cerr << "\tgo to x.a" << std::endl;
                        x.b=x.a;
                        f.b=f.a;
                    }
                    else
                    {
                        if(num>=bc*den)
                        {
                            std::cerr << "\tgo to x.c" << std::endl;
                            x.b=x.c;
                            f.b=f.c;
                        }
                        else
                        {
                            const real_t xu = clamp(x.a,x.b + num/den,x.c);
                            const real_t fu = func(xu);
                            std::cerr << "xu=" << xu << ", fu=" << fu << std::endl;
                            if(fu<f.b)
                            {
                                //______________________________________________
                                //
                                // u is the new winner
                                //______________________________________________
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
                            else
                            {
                                //______________________________________________
                                //
                                // b is still the winner
                                //______________________________________________
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
                        }
                    }
                    
                }
                
            }
            
            
        }
        
        template <>
        void minimize3<real_t>(numeric<real_t>::function &func,
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
                minstep3(func,x,f);
            }
            
        }
        
        
        
    } // math
    
} //yocto
