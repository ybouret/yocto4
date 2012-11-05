#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/code/hsort.hpp"

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
            
#if 0
            static inline int compareW( const real_t a, const real_t b )
            {
                const real_t lhs = Fabs(a);
                const real_t rhs = Fabs(b);
                return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0 );
            }
#endif
        }
        
        
#define IS_NR 0
#define IS_CJ 1
        
        template <>
        bool Newton<real_t>:: solve( Function &func, Jacobian &jac, array<real_t> &X, real_t ftol )
        {
            static const real_t alpha      = real_t(1e-4);
            static const real_t rate       = (1-alpha);
            
            const size_t n = X.size(); assert(n>0);
            
            matrix<real_t>             J0(n,n);   // initial jacobian
            matrix<real_t>             J(n,n);    // working copy of jacobian
            vector<real_t>             F(n,0);    // initial function values
            vector<real_t>             h(n,0);    // common step
            vector<real_t>             g(n,0);    // for Conjugated Gradient
            vector<real_t>             w(n,0);    // for singular values
            vector<real_t>             wa(n,0);   // absolute values
            matrix<real_t>             v(n,n);    // for SVD
            vector<real_t>             xi(n,0);   // for Conjugated Gradient
            Energy                     nrj(func,X,h);
            array<real_t>             &XX = nrj.X;
            array<real_t>             &FF = nrj.F;
            numeric<real_t>::function  E( &nrj, & Energy::Compute );
            int status = IS_NR; // default: newton raphson
            
            //==================================================================
            //
            // Initialize F and its Energy
            //
            //==================================================================
            func(F,X);
            real_t G0 = energy_of(F);
            
            for(;;)
            {
                //==============================================================
                //
                // Evaluate Jacobian
                //
                //==============================================================
                jac(J0,X);
                J.assign(J0);
                std::cerr << "X=" << X << std::endl;
                std::cerr << "F=" << F << std::endl;
                std::cerr << "J=" << J << std::endl;
                std::cerr << "G=" << G0 << std::endl;
                
                //==============================================================
                //
                // compute SVD
                //
                //==============================================================
                if( !svd<real_t>::build(J,w,v) )
                {
                    std::cerr << "[newton: very bad jacobian]" << std::endl;
                    return false;
                }
                
                //==============================================================
                //
                // Check condition
                //
                //==============================================================
                for(size_t j=n;j>0;--j) wa[j] = Fabs(w[j]);
                hsort(wa);
                const real_t wmax  = wa[ n ];
                if( wmax <= 0 )
                {
                    std::cerr << "[newton]: null jacobian" << std::endl;
                    return false;
                }
                const real_t wmin  = wa[ 1 ];
                const real_t icond = Fabs( wmin / wmax );
                
                std::cerr << "w="       << w     << std::endl;
                std::cerr << "icond = " << icond << std::endl;
                
                if( icond >= 1e-6 )
                {
                    //==========================================================
                    //
                    // ENTER: Newton's Step
                    //
                    //==========================================================
                    status = IS_NR;
                    
                    //----------------------------------------------------------
                    //
                    // compute it thru SVD
                    //
                    //----------------------------------------------------------
                    for( size_t j=n;j>0;--j)
                        g[j] = -F[j];
                    svd<real_t>::solve(J, w, v, g, h);
                    std::cerr << "h=" << h << std::endl;
                    
                    
                    //----------------------------------------------------------
                    //
                    // Take the full Newton's step
                    //
                    //----------------------------------------------------------
                    real_t G1 = E(1);
                    std::cerr  << "G1=" << G1 << std::endl;
                    if( G1 <= G0 * rate )
                    {
                        //------------------------------------------------------
                        // accept it
                        //------------------------------------------------------
                        std::cerr << "[newton]: good step" << std::endl;
                        for( size_t j=n;j>0;--j)
                        {
                            X[j] = XX[j];
                            F[j] = FF[j];
                        }
                        G0 = G1;
                        if( has_converged(X, h, ftol) )
                        {
                            std::cerr << "[newton]: success" << std::endl;
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
                        std::cerr << "[newton]: backtrack" << std::endl;
                        
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
                                std::cerr << "[newton]: backtrack failure" << std::endl;
                                return false;
                            }
                        }
                        //std::cerr << "[newton]: backtrack from " << lam << ", G=" << G1 << std::endl;
                        
                        triplet<real_t> x = { 0,  lam, 0 };
                        triplet<real_t> f = { G0, G1,  0 };
#if 0
                        //------------------------------------------------------
                        // cheap bracketing with simple golden extension
                        //------------------------------------------------------
                        static const real_t GOLD = REAL(1.61803398874989);
                        static const real_t xmax = 0.5;
                        bool                stop = false;
                        
                        
                        x.c = x.b + GOLD * ( x.b - x.a );
                        f.c = E(x.c);
                        while( f.b > f.c )
                        {
                            if( x.c >= xmax )
                            {
                                // don't go further
                                x.b = x.c;
                                f.b = f.c;
                                stop = true;
                                break;
                            }
                            //-- move forward
                            x.a = x.b; x.b = x.c; x.c = x.b + GOLD * (x.b - x.a);
                            f.a = f.b; f.b = f.c; f.c = E(x.c);
                        }
                        if( !stop )
                        {
                            minimize(E, x, f, ftol);
                        }
#endif
                        bracket<real_t>::expand( E, x, f );
                        minimize<real_t>(E,x,f,ftol);
                        lam = x.b;
                        G1  = E(lam);
                        std::cerr << "[newton]: backtrack@" << lam << ", G=" << G1 << std::endl;
                        
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
                    std::cerr << "grad=" << xi << std::endl;
                    
                    //----------------------------------------------------------
                    //
                    // compute conjugated gradient
                    //
                    //----------------------------------------------------------
                    bool may_return = true;
                    if( IS_NR == status )
                    {
                        std::cerr << "[newton]: initialize cj" << std::endl;
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
                        std::cerr << "[newton]: update cj" << std::endl;
                        real_t dgg=0, gg=0;
                        for( size_t j=n;j>0;--j)
                        {
                            gg  += g[j] * g[j];
                            dgg += (xi[j]+g[j])*xi[j];
                        }
                        if( gg <= 0 )
                        {
                            std::cerr << "[newton]: local extremum" << std::endl;
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
                    std::cerr << "[newton]: cj line minimization" << std::endl;
                    triplet<real_t> x = { 0,  1,      0 };
                    triplet<real_t> f = { G0, E(x.b), 0 };
                    std::cerr << "cj_x=" << x << std::endl;
                    std::cerr << "cj_f=" << f << std::endl;
                    bracket<real_t>::expand(E, x,f);
                    std::cerr << "cj_x=" << x << std::endl;
                    std::cerr << "cj_f=" << f << std::endl;
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
                    
                    std::cerr << "[newton]: cj@G=" << G0 << " (opt=" << x.b << ")" << std::endl;
                    std::cerr << "h=" << h << std::endl;
                    if( may_return && has_converged(X, h, ftol) )
                    {
                        std::cerr << "[newton]: cj success" << std::endl;
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
