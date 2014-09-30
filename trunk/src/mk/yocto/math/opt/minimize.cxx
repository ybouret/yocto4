#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"

//#include <iostream>

namespace yocto {
    
    namespace math {
        
        
#define SHIFT3(a,b,c,d) do { (a)=(b);(b)=(c);(c)=(d); } while(false)
        
        
        template <>
        void minimize<real_t>(numeric<real_t>::function &func,
                              triplet<real_t>           &x,
                              triplet<real_t>           &f,
                              real_t                     ftol )
        {
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            assert(x.is_ordered());
            
            
            static const real_t fmax = Sqrt(numeric<real_t>::ftol); //!< numeric constraint
            //static const real_t dmax = numeric<real_t>::tiny;       //!< numeric constraint
            static const real_t R    = REAL(0.618033988749895);
            static const real_t C    = REAL(1.0)-R;
            
            
            //const real_t xtol = max_of<real_t>( fmax, Fabs(ftol) );
            
            real_t       x0   = x.a, x1, x2, x3 = x.c;
            real_t       f0   = f.a, f1, f2, f3 = f.c;
            
            {
                const real_t bc = x.c - x.b;
                const real_t ab = x.b - x.a;
                if( Fabs(bc) > Fabs(ab) )
                {
                    x1 = x.b;
                    f1 = f.b;
                    x2 = x.b + C * bc;
                    f2 = func( x2 );
                }
                else
                {
                    x2 = x.b;
                    f2 = f.b;
                    x1 = x.b - C * ab;
                    f1 = func( x1 );
                }
                assert(Fabs(x0-x1)<=Fabs(x2-x3));
            }
            
            std::cerr << "x0=" << x0 << ", x1=" << x1 << ", x2=" << x2 << ", x3=" << x3 << std::endl;
            std::cerr << "f0=" << f0 << ", f1=" << f1 << ", f2=" << f2 << ", f3=" << f3 << std::endl;
            
            
            real_t old_w = Fabs(x3-x0);
            real_t old_f = f.b;
            
            //while(Fabs( x3 - x0 ) > max_of<real_t>(xtol * ( Fabs(x1) + Fabs(x2) ), dmax) )
            while(true)
            {
                assert( (f1<=f0&&f1<=f3) || (f2<=f0&&f2<=f3) );
                if( f2 < f1 )
                {
                    SHIFT3(x0,x1,x2,R*x1+C*x3);
                    SHIFT3(f0,f1,f2,func(x2));
                }
                else
                {
                    SHIFT3(x3,x2,x1,R*x2+C*x0);
                    SHIFT3(f3,f2,f1,func(x1));
                }
                std::cerr << "x0=" << x0 << ", x1=" << x1 << ", x2=" << x2 << ", x3=" << x3 << std::endl;
                std::cerr << "f0=" << f0 << ", f1=" << f1 << ", f2=" << f2 << ", f3=" << f3 << std::endl;
                
                const real_t new_w  = Fabs(x3-x0);
                const real_t new_f  = min_of(f1,f2);
                std::cerr << "f: " << old_f  << " ->  " << new_f  << std::endl;
                std::cerr << "w: " << old_w  << " ->  " << new_w  << std::endl;
                if(new_w>=old_w)
                {
                    break;
                }
                const real_t df = Fabs(old_f-new_f);
                std::cerr << "df=" << df << std::endl;
                if(df<=0)
                {
                    //break;
                }
                old_f = new_f;
                old_w = new_w;
            }
            
            // best numeric presentation
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
                               real_t                     ftol )
        {
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            assert(x.is_ordered());
            
            while(true)
            {
                const real_t ac    = x.c - x.a;
                const real_t ab    = x.b - x.a;
                //TODO: check
                const real_t beta  = ab/ac;
                const real_t f1mf0 = f.c - f.a;
                const real_t fbmf0 = f.b - f.a;
                const real_t minus_half_den   =  Fabs(beta * f1mf0 - fbmf0);
                real_t u = REAL(0.5);
                if(minus_half_den>0)
                {
                    const real_t num = (f1mf0) * beta * (REAL(1.0)-beta);
                    if(num>=minus_half_den)
                    {
                        u = REAL(1.0);
                    }
                    else
                    {
                        if(num<=-minus_half_den)
                        {
                            u = REAL(-1.0);
                        }
                        else
                        {
                            u = clamp<real_t>(0,REAL(0.5)+num/(minus_half_den+minus_half_den),1);
                        }
                    }
                }
                std::cerr << "u=" << u << std::endl;
                const real_t d  = clamp<real_t>(x.a,x.a + u * ac,x.c);
                std::cerr << "d=" << d << std::endl;
                const real_t fd = func(d);
                
                
                
                return;
            }
            
            
            
            
        }
        
        
    } // math
    
} //yocto
