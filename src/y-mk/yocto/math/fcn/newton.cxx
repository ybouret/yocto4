#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

//#define YOCTO_NEWTON_SHOW 1

#if defined(YOCTO_NEWTON_SHOW)
#define Y_NEWTON_OUT(EXPR) do { EXPR; } while(false)
#else
#define Y_NEWTON_OUT(EXPR) 
#endif

namespace yocto
{
    namespace math
    {
        namespace
        {
            // Energy Function for Conjugated Gradient
            static inline real_t energy_of( const array<real_t> &F ) throw()
            {
                real_t ans = 0;
                for( size_t i=F.size();i>0;--i)
                    ans += F[i] * F[i];
                return ans/2;
            }
            
            // class to probe energy of a function along a line
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
                
                inline real_t Get( real_t lam )
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
            
            static inline bool has_converged(const array<real_t> &X,
                                             const array<real_t> &h,
                                             const real_t         ftol) throw()
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
        
#define Y_NEWTON_CB() do { if( (0!=cb) && ! (*cb)(F,X) ) return false; } while(false)
        
        template <>
        bool Newton<real_t>:: solve(Function      &func,
                                    Jacobian      &jac,
                                    array<real_t> &X,
                                    real_t         ftol,
                                    Callback      *cb)
        {
            static const real_t alpha      = real_t(1e-4);
            static const real_t rate       = (1-alpha);
            static const real_t icond_min  = max_of<real_t>(numeric<real_t>::ftol,REAL(1e-6));
            
            const size_t n = X.size(); assert(n>0);
            
            matrix<real_t>             J0(n,n);       // initial jacobian
            matrix<real_t>             J(n,n);        // working copy of jacobian
            vector<real_t>             F(n,0);        // initial function values
            vector<real_t>             h(n,0);        // common step
            vector<real_t>             g(n,0);        // for Conjugated Gradient
            vector<real_t>             w(n,0);        // for singular values
            vector<real_t>             wa(n,0);       // absolute values
            matrix<real_t>             v(n,n);        // for SVD
            vector<real_t>             xi(n,0);       // for Conjugated Gradient
            Energy                     nrj(func,X,h); // for line minimization
            array<real_t>             &XX = nrj.X;    // helper memory
            array<real_t>             &FF = nrj.F;    // helper memory
            numeric<real_t>::function  E( &nrj, & Energy::Get );
            int                        status = IS_NR; // default: Newton Raphson
            
            //==================================================================
            //
            // Initialize F and its Energy
            //
            //==================================================================
            func(F,X);
            Y_NEWTON_CB();
            real_t G0 = energy_of(F);
            
            for(;;)
            {
                //==============================================================
                //
                // Evaluate Jacobian
                //
                //==============================================================
                Y_NEWTON_OUT(std::cerr << "[newton] step" << std::endl);
                jac(J0,X);
                J.assign(J0);
                //std::cerr << "X=" << X << std::endl;
                //std::cerr << "F=" << F << std::endl;
                //std::cerr << "J=" << J << std::endl;
                Y_NEWTON_OUT(std::cerr << "[newton] G0 = " << G0 << std::endl);
                
                //==============================================================
                //
                // compute SVD
                //
                //==============================================================
                if( !svd<real_t>::build(J,w,v) )
                {
                    Y_NEWTON_OUT(std::cerr << "[newton] \tvery bad jacobian!" << std::endl);
                    return false;
                }
                
                //==============================================================
                //
                // Check condition
                //
                //==============================================================
                for(size_t j=n;j>0;--j) wa[j] = Fabs(w[j]);
                quicksort(wa);
                const real_t wmax  = wa[ n ];
                if( wmax <= 0 )
                {
                    Y_NEWTON_OUT(std::cerr << "[newton] \tnull jacobian" << std::endl);
                    return false;
                }
                const real_t wmin  = wa[ 1 ];
                const real_t icond = Fabs( wmin / wmax );
                
                //std::cerr << "w="       << w     << std::endl;
                Y_NEWTON_OUT(std::cerr << "[newton] icond = " << icond << std::endl);
                
                if( icond >= icond_min )
                {
                    //==========================================================
                    //
                    // ENTER: Newton's Step
                    //
                    //==========================================================
                    Y_NEWTON_OUT(std::cerr << "[newton] \tusing Newton-Raphson" << std::endl);
                    status = IS_NR;
                    
                    //----------------------------------------------------------
                    //
                    // compute it thru SVD
                    //
                    //----------------------------------------------------------
                    for( size_t j=n;j>0;--j)
                        g[j] = -F[j];
                    svd<real_t>::solve(J, w, v, g, h);
                    Y_NEWTON_OUT(std::cerr << "[newton] \th = " << h << std::endl);
                    
                    
                    //----------------------------------------------------------
                    //
                    // Take the full Newton's step
                    //
                    //----------------------------------------------------------
                    real_t G1 = E(1);
                    Y_NEWTON_OUT(std::cerr  << "[newton] \tG1 = " << G1 << std::endl);
                    if( G1 <= G0 * rate )
                    {
                        //------------------------------------------------------
                        // accept it
                        //------------------------------------------------------
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tgood step" << std::endl);
                        for( size_t j=n;j>0;--j)
                        {
                            X[j] = XX[j];
                            F[j] = FF[j];
                        }
                        Y_NEWTON_CB();
                        G0 = G1;
                        if( has_converged(X, h, ftol) )
                        {
                            Y_NEWTON_OUT(std::cerr << "[newton] \t\t\t[[success]]" << std::endl);
                            return true;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // ENTER: backtrack
                        //
                        //------------------------------------------------------
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tbacktrack" << std::endl);
                        
                        //------------------------------------------------------
                        // find first point,
                        // based on decrease rate approximation
                        //------------------------------------------------------
                        real_t lam = alpha;
                        for(;;)
                        {
                            G1 = E(lam);
                            if( G1 < G0 ) break;
                            lam /= 2;
                            if( lam <= numeric<real_t>::tiny )
                            {
                                Y_NEWTON_OUT(std::cerr << "[newton] \t\t\tbacktrack failure" << std::endl);
                                return false;
                            }
                        }
                        //std::cerr << "[newton]: backtrack from " << lam << ", G=" << G1 << std::endl;
                        
                        triplet<real_t> x = { 0,  lam, 0 };
                        triplet<real_t> f = { G0, G1,  0 };
                        bracket<real_t>::expand( E, x, f );
                        minimize<real_t>(E,x,f,ftol);
                        lam = x.b;
                        G1  = E(lam);
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tbacktrack@" << lam << ", G = " << G1 << std::endl);
                        
                        //------------------------------------------------------
                        // record new position, no success on backtrack
                        //------------------------------------------------------
                        for( size_t j=n;j>0;--j)
                        {
                            X[j] = XX[j];
                            F[j] = FF[j];
                        }
                        G0 = G1;
                        
                        
                        //------------------------------------------------------
                        //
                        // LEAVE: backtrack
                        //
                        //------------------------------------------------------
                    }
                    
                    
                    //==========================================================
                    //
                    // LEAVE: Newton's Step
                    //
                    //==========================================================
                    
                }
                else
                {
                    //==========================================================
                    //
                    // ENTER: Conjugated Gradient
                    //
                    //==========================================================
                    Y_NEWTON_OUT(std::cerr << "[newton] \tUsing Conjugated Gradient" << std::endl);
                    //----------------------------------------------------------
                    //
                    // compute gradient
                    //
                    //----------------------------------------------------------
                    for( size_t j=n;j>0;--j)
                    {
                        real_t sum = 0;
                        for( size_t i=n;i>0;--i)
                            sum += F[i] * J0[i][j];
                        xi[j] = sum;
                    }
                    Y_NEWTON_OUT(std::cerr << "[newton] \tgrad=" << xi << std::endl);
                    
                    //----------------------------------------------------------
                    //
                    // compute conjugated gradient
                    //
                    //----------------------------------------------------------
                    bool may_return = true;
                    if( IS_NR == status )
                    {
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tinitialize CJ" << std::endl);
                        status     = IS_CJ;
                        may_return = false;
                        for(size_t j=n;j>0;--j)
                        {
                            g[j] = -xi[j];
                            xi[j]=h[j]=g[j];
                        }
                    }
                    else
                    {
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tupdate CJ" << std::endl);
                        real_t dgg=0, gg=0;
                        for( size_t j=n;j>0;--j)
                        {
                            gg  += g[j] * g[j];
                            dgg += (xi[j]+g[j])*xi[j];
                        }
                        if( gg <= 0 )
                        {
                            Y_NEWTON_OUT(std::cerr << "[newton] \t\t\tlocal extremum failure" << std::endl);
                            return true;
                        }
                        const real_t gam=dgg/gg;
                        for(size_t j=n;j>0;--j)
                        {
                            g[j]  = -xi[j];
                            xi[j] = h[j]=g[j]+gam*h[j];
                        }
                    }
                    
                    //----------------------------------------------------------
                    //
                    // find the line minimum
                    //
                    //----------------------------------------------------------
                    Y_NEWTON_OUT(std::cerr << "[newton] \tCJ line minimization" << std::endl);
                    triplet<real_t> x = { 0,  1,      0 };
                    triplet<real_t> f = { G0, E(x.b), 0 };
                    //std::cerr << "cj_x=" << x << std::endl;
                    //std::cerr << "cj_f=" << f << std::endl;
                    bracket<real_t>::expand(E, x,f);
                    //std::cerr << "cj_x=" << x << std::endl;
                    //std::cerr << "cj_f=" << f << std::endl;
                    minimize<real_t>(E, x, f, ftol);
                    
                    //----------------------------------------------------------
                    //
                    // update the coordinates
                    //
                    //----------------------------------------------------------
                    G0 = E(x.b);
                    for(size_t j=n;j>0;--j)
                    {
                        h[j] = XX[j] - X[j]; // effective step
                        X[j] = XX[j];
                        F[j] = FF[j];
                    }
                    Y_NEWTON_CB();

                    Y_NEWTON_OUT(std::cerr << "[newton] \tCJ@ G = " << G0 << " (opt=" << x.b << ")" << std::endl);
                    Y_NEWTON_OUT(std::cerr << "[newton] \t    h = " << h << std::endl);
                    if( may_return && has_converged(X, h, ftol) )
                    {
                        Y_NEWTON_OUT(std::cerr << "[newton] \t\tCJ success" << std::endl);
                        return true;
                    }
                    //==========================================================
                    //
                    // LEAVE: Conjugated Gradient
                    //
                    //==========================================================
                }
                
            }
            
            
            return false;
        }
    }
}
