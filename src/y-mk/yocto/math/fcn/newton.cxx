#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"
//#include "yocto/math/kernel/linsys.hpp"
#include "yocto/math/kernel/svd.hpp"
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
            vector<real_t> h(n,0);  // common step
            vector<real_t> g(n,0);  // for Conjugated Gradient
            vector<real_t> W(n,0);  // singular values
            matrix<real_t> V(n,n);  // for SVD
            vector<real_t> xi(n,0); // for Conjugated Gradient
            Energy                    nrj(func,X,h);
            array<real_t>            &Xtmp = nrj.X;
            array<real_t>            &Ftmp = nrj.F;
            numeric<real_t>::function E( &nrj, & Energy::Compute );
            int status = IS_NR; // default: newton raphson
            
            
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
                if( ! svd<real_t>::build(J,W,V) )
                {
                    std::cerr << "SVD failure: very bad jacobian" << std::endl;
                    
                                        
                    return false;
                }
                
                std::cerr << "W=" << W << std::endl;
                if( svd<real_t>::truncate(W, ftol) )
                {
                    //----------------------------------------------------------
                    //
                    // switch to CJ
                    //
                    //----------------------------------------------------------
                    std::cerr << "(Almost) Singular Jacobian" << std::endl;
                   
                    //----------------------------------------------------------
                    //
                    // build energy gradient
                    //
                    //----------------------------------------------------------
                    for( size_t i=n;i>0;--i)
                    {
                        real_t sum = 0;
                        for( size_t j=n;j>0;--j)
                            sum += F[j] * J0[j][i];
                        xi[i] = sum;
                    }
                    
                    if( IS_NR )
                    {
                        //-- initialize CJ
                        std::cerr << "start CJ" << std::endl;
                        status = IS_CJ;
                    }
                    else
                    {
                        //-- updated CJ
                        std::cerr << "update CJ" << std::endl;
                    }

                    return false;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // compute the full newton step
                    //
                    //----------------------------------------------------------
                    status = IS_NR;
                    for(size_t i=n;i>0;--i)
                        F[i] = -F[i];
                    svd<real_t>::solve(J, W, V, F, h);
                    std::cerr << "h=" << h << std::endl;
                    
                    //----------------------------------------------------------
                    //
                    // evaluate the new position/energy
                    //
                    //----------------------------------------------------------
                    for(size_t i=n;i>0;--i)
                        Xtmp[i] = X[i] + h[i];
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
                        //
                        // yes: good enough decrease
                        //
                        //------------------------------------------------------
                        for(size_t i=n;i>0;--i)
                        {
                            X[i] = Xtmp[i];
                            F[i] = Ftmp[i];
                        }
                        std::cerr << "good step" << std::endl;
                        std::cerr << "X=" << X   << std::endl;
                        std::cerr << "F=" << F   << std::endl;
                        std::cerr << "g=" << g1  << std::endl;
                        //------------------------------------------------------
                        //
                        // test convergenge
                        //
                        //------------------------------------------------------
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
                        //
                        // no: find a better decrease
                        //
                        //------------------------------------------------------
                        std::cerr << "need to backtrack!" << std::endl;
                        
                        //------------------------------------------------------
                        //-- approximating g(lambda) = glim
                        //------------------------------------------------------
                        real_t lam = alpha;
                        for(;;)
                        {
                            g1=E(lam);
                            if( g1 < g0 )
                                break;
                            lam /= 2;
                            if( lam <= numeric<real_t>::tiny )
                            {
                                return false;
                            }
                        }
                        assert(g1<g0);
                        std::cerr << "backtrack@" << lam << " => " << g1 << std::endl;
                        
                        //------------------------------------------------------
                        // limited bracketing
                        //------------------------------------------------------
                        triplet<real_t>     x    = { 0, lam, 0 };
                        triplet<real_t>     f    = { g0, g1, 0 };
                        static const real_t GOLD = REAL(1.61803398874989);
                        static const real_t xmax = 0.5;
                        bool                stop = false;
                        for(;;)
                        {
                            x.c = x.b + GOLD * (x.b-x.a);
                            if( x.c >= xmax )
                            {
                                x.c  = xmax;
                                stop = true;
                            }
                            f.c = E(x.c);
                            if( f.c >= f.b )
                            {
                                break;
                            }
                            else
                            {
                                assert( f.c < f.b );
                                if( stop )
                                {
                                    x.b = x.c;
                                    f.b = f.c;
                                    break;
                                }
                                x.a = x.b;
                                f.a = f.b;
                                x.b = x.c;
                                f.b = f.c;
                            }
                        }
                        std::cerr << "bracket_x=" << x << std::endl;
                        std::cerr << "bracket_f=" << f << std::endl;
                        if( !stop )
                            minimize(E, x, f, ftol);
                        g0 = E(x.b);
                        std::cerr << "backtrack g(" << x.b << ")=" << g0 << std::endl;
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
