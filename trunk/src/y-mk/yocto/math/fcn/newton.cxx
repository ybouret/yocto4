#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/ios/ocstream.hpp"

namespace yocto
{
    namespace math
    {
        
        
        namespace
        {
#define Y_NEWTON_CB() do { if( (0!=cb) && ! (*cb)(F,X) ) return false; } while(false)
            
            static inline real_t energy_of( const array<real_t> &U ) throw()
            {
                real_t ans = 0;
                for(size_t i=U.size();i>0;--i)
                {
                    const real_t u = U[i];
                    ans += u*u;
                }
                return ans/2;
            }
            
            class newton
            {
            public:
                typedef algebra<real_t>   mkl;
                Newton<real_t>::Function &func;
                Newton<real_t>::Jacobian &fjac;
                array<real_t>            &X;    //!< user's X
                const size_t              n;    //!< #variables
                vector<real_t>            F;    //!< function values
                vector<real_t>            h;    //!< Newton's step
                vector<real_t>            g;    //!< auxiliary
                vector<real_t>            xi;   //!< for CJ
                vector<real_t>            XX;   //!< trial position
                vector<real_t>            FF;   //!< trial function
                vector<real_t>            W;    //!< for SVD
                vector<real_t>            A;    //!< |W|
                matrix<real_t>            J;    //!< Jacobian, used in SVD
                matrix<real_t>            V;    //!< for SVD
                numeric<real_t>::function H;
                const real_t              ftol;  //!< fractional tolerance
                const real_t              alpha; //!< control parameter
                Newton<real_t>::Callback *cb;
                
                enum status_type
                {
                    using_nr,
                    using_cj
                };
                
                inline ~newton() throw() {}
                
                //==============================================================
                //
                // create resources
                //
                //==============================================================
                inline  newton(Newton<real_t>::Function &func_ref,
                               Newton<real_t>::Jacobian &fjac_ref,
                               array<real_t>            &X_ref,
                               const real_t              frc_tol,
                               Newton<real_t>::Callback *cb_addr) :
                func( func_ref ),
                fjac( fjac_ref ),
                X( X_ref    ),
                n( X.size() ),
                F(n,0),
                h(n,0),
                g(n,0),
                xi(n,0),
                XX(n,0),
                FF(n,0),
                W(n,0),
                A(n,0),
                J(n,n),
                V(n,n),
                H( this, & newton::getH ),
                ftol( Fabs(frc_tol) ),
                alpha( REAL(1.0e-3) ),
                cb(cb_addr)
                {
                }
                
                //==============================================================
                //
                // compute jacobian condition
                //
                //==============================================================
                inline bool compute_icond_and_xi( real_t &icond )
                {
                    //-- compute the Jacobian @X
                    fjac(J,X);
                    
                    //-- Gradient in xi
                    for( size_t j=n;j>0;--j)
                    {
                        real_t sum = 0;
                        for( size_t i=n;i>0;--i)
                            sum += F[i] * J[i][j];
                        xi[j] = sum;
                    }
                    
                    //-- svd
                    if( !svd<real_t>::build(J, W, V) )
                    {
                        std::cerr << "[Newton] Invalid Jacobian" << std::endl;
                        return false;
                    }
                    
                    //-- condition
                    for(size_t i=n;i>0;--i) A[i] = Fabs(W[i]);
                    quicksort(A);
                    const real_t amax = A[n];
                    if(amax<=0)
                    {
                        std::cerr << "[Newton] Singular Jacobian" << std::endl;
                        return false;
                    }
                    const real_t amin = A[1];
                    icond = amin/amax;
                    
                    std::cerr << "[Newton] icond=" << icond << std::endl;
                    
                   
                    
                    return true;
                }
                
                inline bool has_converged()
                {
                    bool cvg = true;
                    for(size_t i=n;i>0;--i)
                    {
                        const real_t dx = Fabs(XX[i]-X[i]);
                        const real_t sx = Fabs(XX[i]) + Fabs(X[i]);
                        if( dx > ftol * sx )
                            cvg = false;
                        X[i] = XX[i];
                        F[i] = FF[i];
                    }
                    
                    return cvg;
                }
                //==============================================================
                //
                // the algorithm
                //
                //==============================================================
                bool solve()
                {
                    
                    //----------------------------------------------------------
                    //
                    // initialize
                    //
                    //----------------------------------------------------------
                    func(F,X);
                    real_t      icond  = -1;
                    status_type status = using_nr;
                    real_t      H0     = energy_of(F);
                    
                    
                    Y_NEWTON_CB();
                    
                    
                    //----------------------------------------------------------
                    //
                    // loop
                    //
                    //----------------------------------------------------------
                    for(;;)
                    {
                        std::cerr << std::endl;
                        std::cerr << "[Newton] H0=" << H0 << std::endl;
                        std::cerr << "          X=" << X << std::endl;
                        std::cerr << "          F=" << F << std::endl;
                        
                        //------------------------------------------------------
                        // compute Jacobian, its inverse condition
                        // and the Energy gradient in xi
                        //------------------------------------------------------
                        if(! compute_icond_and_xi(icond) )
                            return false;
                        
                        if( icond >= 1e-1 )
                        {
                            //__________________________________________________
                            //
                            // Using Newton's Step
                            //__________________________________________________
                            
                            status = using_nr;
                            //--------------------------------------------------
                            // compute the full Newton's step in h
                            //--------------------------------------------------
                            mkl::neg(g,F);
                            svd<real_t>::solve(J, W, V, g, h);
                            
                            //--------------------------------------------------
                            // try it...will initialize XX and FF
                            //--------------------------------------------------
                            const real_t rho   = -mkl::dot(xi,h);
                            const real_t slope = alpha*rho;
                            const real_t Hr    = H0-slope;
                            const real_t H1    = H(1);
                            std::cerr << "[Newton] rho = " << rho << std::endl;
                            std::cerr << "[Newton] H1  = " << H1  << " / Hr=" << Hr << std::endl;
                            
                            if(H1<=Hr)
                            {
                                //----------------------------------------------
                                // We descend fast enough
                                //----------------------------------------------
                                std::cerr << "[Newton] Accepting" << std::endl;
                                H0=H1;
                                
                                Y_NEWTON_CB();
                                
                                if(has_converged())
                                {
                                    std::cerr << "[Newton] Convergence" << std::endl;
                                    return true;
                                }

                            }
                            else
                            {
                                //----------------------------------------------
                                // Bad descent: find a backtracking point
                                //----------------------------------------------
                                std::cerr << "[Newton] Backtracking" << std::endl;
                                real_t lam = REAL(0.5);
                                real_t Hbk = H(lam);
                                while(Hbk>H0-lam*slope)
                                {
                                    lam /= 2;
                                    if( lam <= numeric<real_t>::tiny )
                                    {
                                        std::cerr << "[Newton] Spurious Point" << std::endl;
                                        return false;
                                    }
                                    Hbk = H(lam);
                                }
                                std::cerr << "H(" << lam << ")=" << Hbk << std::endl;
                                
                                H0  = Hbk;
                                std::cerr << "H(" << lam <<  ")=" << H0 << std::endl;
                                for(size_t i=n;i>0;--i)
                                {
                                    X[i] = XX[i];
                                    F[i] = FF[i];
                                }
                                Y_NEWTON_CB();
                            }
                        }
                        else
                        {
                            //__________________________________________________
                            //
                            // Using Conjugated Gradient
                            //__________________________________________________
                            
                            //--------------------------------------------------
                            // Algorithm for CJ
                            //--------------------------------------------------
                            bool may_return = true;
                            if( using_nr == status )
                            {
                                std::cerr << "[Newton] Initialize CJ" << std::endl;
                                status     = using_cj;
                                may_return = false;
                                for(size_t j=n;j>0;--j)
                                {
                                    g[j] = -xi[j];
                                    xi[j]=h[j]=g[j];
                                }
                            }
                            else
                            {
                                std::cerr << "[Newton] update CJ" << std::endl;
                                real_t dgg=0, gg=0;
                                for( size_t j=n;j>0;--j)
                                {
                                    gg  += g[j] * g[j];
                                    dgg += (xi[j]+g[j])*xi[j];
                                }
                                if( gg <= 0 )
                                {
                                    std::cerr << "[Newton] Local Extremum" << std::endl;
                                    return true;
                                }
                                const real_t gam=dgg/gg;
                                for(size_t j=n;j>0;--j)
                                {
                                    g[j]  = -xi[j];
                                    xi[j] = h[j]=g[j]+gam*h[j];
                                }
                            }
                            
                            //--------------------------------------------------
                            // Line Search along h = conjugated direction
                            //--------------------------------------------------
                            std::cerr << "[Newton] Line Minimization" << std::endl;
                            const real_t rate = mkl::dot(h,xi);
                            real_t       beta = alpha*H0/(rate);
                            std::cerr << "rate = "  << rate << std::endl;
                            std::cerr << "beta = "  << beta << std::endl;
                            triplet<real_t> x = { 0,  beta,   0 };
                            triplet<real_t> f = { H0, H(x.b), 0 };
                            bracket<real_t>::expand(H, x,f);
                            minimize<real_t>(H, x, f, 0);
                            beta = x.b;
                            std::cerr << "beta = " << beta << std::endl;
                            H0 = H(beta);
                            std::cerr << "X="  << X   << std::endl;
                            std::cerr << "XX=" << XX  << std::endl;
                            std::cerr << "h="  << h   << std::endl;
                            
                            if( has_converged() && may_return)
                            {
                                std::cerr << "[Newton] SubConvergence" << std::endl;
                                return true;
                            }
                        }
                        
                    }
                    
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(newton);
                
                //! evaluate H using X and Newton's step
                inline real_t getH( real_t lam )
                {
                    for(size_t i=n;i>0;--i) XX[i] = X[i] + lam * h[i];
                    func(FF,XX);
                    return energy_of(FF);
                }
                
                
            };
            
            
        }
        
        template <>
        bool Newton<real_t>:: solve(Function      &func,
                                    Jacobian      &fjac,
                                    array<real_t> &X,
                                    real_t         ftol,
                                    Callback      *cb)
        {
            const size_t n = X.size(); assert(n>0);
            
            //==================================================================
            // acquire resources
            //==================================================================
            newton       newt(func,fjac,X,ftol,cb);
            
            return newt.solve();
            
        }
        
    }
}

#if 0
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
#endif
