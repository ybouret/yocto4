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



        namespace kernel
        {
            template <>
            void minimize(numeric<real_t>::function &func,
                          triplet<real_t>           &x,
                          triplet<real_t>           &f)
            {
                static const real_t C = REAL(0.381966011250105);
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
                    const real_t xu = clamp(x.b,x.b+C*bc,x.c);
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
                    const real_t xu = clamp(x.a,x.b-C*ab,x.b);
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
                x.co_sort(f);
            }
        }

        template <>
        void minimize<real_t>(numeric<real_t>::function &func,
                              triplet<real_t>           &x,
                              triplet<real_t>           &f,
                              real_t                     ftol )
        {
            assert(x.is_ordered());
            x.co_sort(f);
            assert(x.a<=x.b);
            assert(x.b<=x.c);
            assert(f.b<=f.a);
            assert(f.b<=f.c);
            ftol =  __compute_ftol(ftol);

            while( max_of<real_t>(x.c-x.a,0) > max_of(ftol*Fabs(x.b),XTOL) )
            {
                kernel::minimize<real_t>(func,x,f);
            }

        }

#if 0
        namespace {

            static inline
            void minstep_para(numeric<real_t>::function &func,
                              triplet<real_t>           &x,
                              triplet<real_t>           &f)
            {
                static const real_t C = REAL(0.381966011250105);
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                //std::cerr << "min3: " << f << " @ " << x << std::endl;
                const real_t ab    = max_of<real_t>(x.b - x.a,0);
                const real_t bc    = max_of<real_t>(x.c - x.b,0);
                const real_t famfb = f.a - f.b;
                const real_t fcmfb = f.c - f.b;
                const real_t p     = bc*famfb; assert(p>=0);
                const real_t q     = ab*fcmfb; assert(q>=0);
                const real_t hd    = p+q; assert(hd>=0);
                if(ab<=0||bc<=0||hd<=0)
                {
                    //std::cerr << "Failsafe..." << std::endl;
                    goto FAILSAFE;
                }
                else
                {
                    //std::cerr << "try parabolic step" << std::endl;
                    const real_t den = hd+hd;
                    const real_t num = bc*p - ab*q;
                    if(num<= -ab*den)
                    {
                        //std::cerr << "\t!at left!" << std::endl;
                        goto FAILSAFE;
                    }
                    else
                    {
                        if(num>=bc*den)
                        {
                            //std::cerr << "\t!at right!" << std::endl;
                            goto FAILSAFE;
                        }
                        else
                        {
                            const real_t xu = clamp(x.a,x.b+num/den,x.c);
                            //std::cerr << "\tat " << xu << std::endl;
                            const real_t xm = (x.a+x.c) * REAL(0.5);
                            const real_t moved = Fabs(xm-xu);
                            const real_t width = Fabs(x.c - x.a);
                            const real_t ratio = moved/width;
                            std::cerr << "\t\tratio=" << ratio << std::endl;
                            if(ratio>REAL(0.25))
                            {
                                goto FAILSAFE;
                            }

                            const real_t fu = func(xu);
                            if(fu>f.b)
                            {
                                //______________________________________________
                                //
                                // b is still the winner: move one side
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
                            else
                            {
                                //______________________________________________
                                //
                                // u is the new winner
                                //______________________________________________
                                if(xu <= x.b )
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

                        }
                    }
                    return;
                }

            FAILSAFE:
                if(bc>ab)
                {
                    //__________________________________________________________
                    //
                    // probe at right
                    //__________________________________________________________
                    const real_t xu = clamp(x.b,x.b+C*bc,x.c);
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
                    const real_t xu = clamp(x.a,x.b-C*ab,x.b);
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
        void minimize_para<real_t>(numeric<real_t>::function &func,
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
                minstep_para(func,x,f);
            }
            
        }
        
#endif
        
        
    } // math
    
} //yocto
