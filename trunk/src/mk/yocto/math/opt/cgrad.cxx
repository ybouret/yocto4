#include "yocto/math/ztype.hpp"
#include "yocto/math/opt/cgrad.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/sequence/some-arrays.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

#include "yocto/math/kernel/tao.hpp"

namespace yocto
{
    namespace math
    {
        
        namespace
        {
            // conjugated gradient wrapper
            /**
             evaluate a scalar field along a given direction
             */
            class cgw
            {
            public:
                inline cgw(numeric<real_t>::scalar_field &func,
                           const array<real_t>           &pos,
                           const array<real_t>           &vec,
                           array<real_t>                 &tmp
                           ) :
                _func( func ),
                _pos( pos ),
                _vec( vec ),
                nvar( _pos.size() ),
                _tmp( tmp )
                {
                }
                
                inline ~cgw() throw() {}
                
                
                numeric<real_t>::scalar_field & _func;
                const array<real_t>           & _pos;
                const array<real_t>           & _vec;
                const size_t                    nvar;
                array<real_t>                 & _tmp;
                
                inline real_t compute( real_t x )
                {
                    for( size_t i=nvar;i>0;--i)
                    {
                        _tmp[i] = _pos[i] + x * _vec[i];
                    }
                    return _func(_tmp);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(cgw);
            };
        }
        
        template <>
        bool cgrad<real_t>::optimize(numeric<real_t>::scalar_field          &func,
                                     numeric<real_t>::vector_field          &grad,
                                     array<real_t>                          &p,
                                     const real_t                            ftol,
                                     callback                               *cb
                                     )
        {
            const size_t nvar = p.size(); assert(nvar>0);
            some_arrays<4,real_t,memory::global> arrays;
            arrays.allocate(nvar);
            array<real_t> &g  = arrays.next_array(); assert(g.size()==nvar);
            array<real_t> &h  = arrays.next_array(); assert(h.size()==nvar);
            array<real_t> &xi = arrays.next_array(); assert(xi.size()==nvar);
            array<real_t> &xx = arrays.next_array(); assert(xx.size()==nvar);
            
            cgw                       wrapper( func, p, xi, xx);
            numeric<real_t>::function F( &wrapper, & cgw::compute );
            
            //------------------------------------------------------------------
            //
            // initialize: value and gradient
            //
            //------------------------------------------------------------------
            if( cb && !(*cb)(p) )
                return false;
            
            real_t fp = func(p);
            grad(xi,p);
            for( size_t j=nvar;j>0;--j)
            {
                g[j] = -xi[j];
                xi[j]=h[j]=g[j];
            }
            
            
            //------------------------------------------------------------------
            //
            // conjugated gradient loop
            //
            //------------------------------------------------------------------
            for(;;)
            {
                //--------------------------------------------------------------
                // initialize line minimzation
                //--------------------------------------------------------------
                triplet<real_t> x = { 0,  1, 0 };
                triplet<real_t> f = { fp, F(x.b), 0};
                bracket<real_t>::expand( F, x, f);
                
                //--------------------------------------------------------------
                // perform line minimization
                //--------------------------------------------------------------
                minimize<real_t>(F, x, f, ftol);
                
                bool converged = true;
                for( size_t i=nvar;i>0;--i)
                {
                    const real_t dp     = x.b * xi[i];
                    const real_t p_old  = p[i];
                    p[i] += dp;
                    const real_t p_new  = p[i];
                    if( Fabs(p_new-p_old) > Fabs(ftol*(Fabs(p_old)+Fabs(p_new))) )
                    {
                        converged = false;
                    }
                }
                const real_t fp_new = f.b;
                //--------------------------------------------------------------
                // apply callback
                //--------------------------------------------------------------
                if( cb && !(*cb)(p) )
                    return false;
                
                //--------------------------------------------------------------
                // test convegence on variables
                //--------------------------------------------------------------
                if( converged )
                    return true;
                
                //--------------------------------------------------------------
                // prepare next step
                //--------------------------------------------------------------
                fp = fp_new;
                grad(xi,p);
                //std::cerr << "pos=" << p  << std::endl;
                //std::cerr << "f  =" << fp << std::endl;
                
                //--------------------------------------------------------------
                // conjugated gradient
                //--------------------------------------------------------------
                real_t dgg = 0, gg= 0;
                for( size_t i=nvar;i>0;--i)
                {
                    gg  += g[i] * g[i];
                    dgg += (xi[i]+g[i])*xi[i];
                }
                
                if( Fabs(gg) <= 0 )
                    return true; // early return
                
                const real_t gam = dgg / gg;
                for( size_t i=nvar;i>0;--i)
                {
                    g[i] = -xi[i];
                    xi[i]=h[i]=g[i]+gam*h[i];
                }
                
            }
        }
        
        template <>
        void cgrad<real_t>:: compute_gradient(array<real_t> &dFdx, const array<real_t> &x)
        {
            assert(pF);
            assert(pdp);
            return __compute( (array<real_t> &)x, dFdx, *pdp);
        }
        
        template <>
        cgrad<real_t>:: cgrad() :
        gradient<real_t>(),
        G( this, & cgrad<real_t>::compute_gradient)
        {
            
        }
        
        template <>
        cgrad<real_t>:: ~cgrad() throw()
        {
            
        }
        
       
        
        template <>
        bool cgrad<real_t>:: run(numeric<real_t>::scalar_field  &F,
                                 array<real_t>                  &p,
                                 const array<real_t>            &dp,
                                 const real_t                    ftol,
                                 callback                       *cb)
        {
            //-- prepare data
            pF  = &F;
            pdp = &dp;
            return optimize(F, G, p, ftol,cb);
        }
        
        
        
        
    } // math
} // yocto

