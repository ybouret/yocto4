#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/math/kernel/linsys.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

namespace yocto
{
    namespace math
    {
        namespace
        {
            
            static inline real_t energy_of( const array<real_t> &F ) throw()
            {
                real_t ans = 0;
                for( size_t i=F.size();i>0;--i)
                    ans += F[i] * F[i];
                return ans/2;
            }
            
            class Energy
            {
            public:
                inline Energy(Newton<real_t>::Function &func,
                              const array<real_t>      &pos,
                              const array<real_t>      &vec
                              ) :
                fn(func),
                n( pos.size() ),
                position(pos),
                direction(vec),
                F(n,0),
                X(n,0)
                {
                }
                
                inline ~Energy() throw() {}
                
                inline real_t Compute( real_t lam )
                {
                    for(size_t i=n;i>0;--i)
                    {
                        X[i] = position[i] + lam * direction[i];
                    }
                    fn(F,X);
                    return energy_of(F);
                }
                
                Newton<real_t>::Function &fn;
                const size_t              n;
                const array<real_t>      &position;
                const array<real_t>      &direction;
                vector<real_t>            F; // trial function value
                vector<real_t>            X; // trial position value
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Energy);
            };
            
            static inline bool has_converged( const array<real_t> &X, const array<real_t> &h, const real_t ftol) throw()
            {
                assert( h.size() == X.size() );
                for( size_t i=h.size();i>0;--i)
                {
                    if( Fabs(h[i]) > Fabs( ftol * X[i] ) )
                    {
                        return false;
                    }
                }
                return true;
            }
        }
        
        
#define IS_NR 0
#define IS_CJ 1
        
        template <>
        bool Newton<real_t>:: solve( Function &func, Jacobian &jac, array<real_t> &X, real_t ftol )
        {
            static const real_t alpha      = real_t(1e-4);
            static const real_t rate       = (real_t(1)-(alpha+alpha));
            
            
            const size_t n = X.size(); assert(n>0);
            
            matrix<real_t> J0(n,n);
            matrix<real_t> J(n,n);
            vector<real_t> F(n,0);
            vector<real_t> h(n,0); // common step
            vector<real_t> g(n,0); // for Conjugated Gradient
            linsys<real_t> solve(n);
            vector<real_t> Xb(n,0);
            
            Energy                    nrj(func,X,h);
            array<real_t>            &Xtmp = nrj.X;
            array<real_t>            &Ftmp = nrj.F;
            numeric<real_t>::function E( &nrj, & Energy::Compute );
            //int status = IS_NR; // default: newton raphson
            
            
            //==================================================================
            // initialize F and its Energy
            //==================================================================
            func(F,X);
            real_t g0 = energy_of(F);
            
            for(;;)
            {
                //==============================================================
                //
                // evaluate jacobian @X
                //
                //==============================================================
                jac(J0,X);
                J.assign(J0);
                std::cerr << "Xi=" << X << std::endl;
                std::cerr << "Fi=" << F << std::endl;
                std::cerr << "J="  << J << std::endl;
                
                //==============================================================
                //
                // examine jacobian
                //
                //==============================================================
                if( !solve.LU(J) )
                {
                    //----------------------------------------------------------
                    //
                    // switch to CJ
                    //
                    //----------------------------------------------------------
                    std::cerr << "Singular Jacobian" << std::endl;
                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // compute the full newton step
                    //
                    //----------------------------------------------------------
                    for(size_t i=n;i>0;--i) h[i] = -F[i];
                    solve(J,h);
                    std::cerr << "h=" << h << std::endl;
                    
                    //----------------------------------------------------------
                    //
                    // evaluate the new position/energy
                    //
                    //----------------------------------------------------------
                    for(size_t i=n;i>0;--i) Xtmp[i] = X[i] + h[i];
                    func(Ftmp,Xtmp);
                    real_t g1 = energy_of(Ftmp);
                    std::cerr << "g0=" << g0 << "-> g1=" << g1 << std::endl;
                   
                    //----------------------------------------------------------
                    //
                    // do we accept the full newton step ?
                    //
                    //----------------------------------------------------------
                    const real_t glim = rate * g0;
                    if( g1 <= glim )
                    {
                        //------------------------------------------------------
                        // yes: good enough decrease
                        //------------------------------------------------------
                        for(size_t i=n;i>0;--i)
                        {
                            X[i] = Xtmp[i];
                            F[i] = Ftmp[i];
                        }
                        std::cerr << "good step" << std::endl;
                        std::cerr << "X=" << X << std::endl;
                        std::cerr << "F=" << F << std::endl;
                        if( has_converged(X,h,ftol) )
                        {
                            std::cerr << "Success after a good step" << std::endl;
                            return true;
                        }
                        g0 = g1;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // no: find a better decrease
                        //------------------------------------------------------
                        std::cerr << "need to backtrack!" << std::endl;
                        size_t iter=1;
                        real_t lam =1;
                        //-- decrease to fall under g0
                        do
                        {
                            lam = real_t(1)/(++iter);
                            if( lam <= numeric<real_t>::tiny )
                            {
                                std::cerr << "backtrack failure" << std::endl;
                                return false;
                            }
                            g1 = E(lam);
                            if( g1 < g0 )
                            {
                                std::cerr << "backtrack@" << lam << " => " << g1 << std::endl;
                                break;
                            }
                        }
                        while(g1>=g0);
                        assert(g1<g0);
                        std::cerr << "backtrack@" << lam << " => " << g1 << std::endl;
                        
                        g0 = g1;
                        for( size_t i=n;i>0;--i)
                        {
                            X[i] = Xtmp[i];
                            F[i] = Ftmp[i];
                        }
                    }
                    
                }
                
            }
            
            
            
            return false;
        }
        
    }
}
