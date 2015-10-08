#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/core/tao.hpp"
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
//#define Y_NEWTON_SHOW 1
            
#if defined(Y_NEWTON_SHOW)
#define Y_NEWT_OUT(CODE) do { CODE; } while(false)
#else
#define Y_NEWT_OUT(CODE)
#endif
            
#define Y_NEWTON_CB() do { if( (0!=cb) && ! (*cb)(F,X) ) return false; } while(false)
            
            
            
            class newton
            {
            public:
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
                
#if defined(_MSC_VER)
				// this in ctor
#pragma warning ( disable : 4355 )
#endif
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
                    //----------------------------------------------------------
                    // compute the Jacobian @X
                    //----------------------------------------------------------
                    fjac(J,X);
                    
                    //----------------------------------------------------------
                    // Gradient in xi
                    //----------------------------------------------------------
                    for( size_t j=n;j>0;--j)
                    {
                        real_t sum = 0;
                        for( size_t i=n;i>0;--i)
                            sum += F[i] * J[i][j];
                        xi[j] = sum;
                    }
                    
                    //----------------------------------------------------------
                    // SVD of Jacobian
                    //----------------------------------------------------------
                    if( !svd<real_t>::build(J, W, V) )
                    {
                        Y_NEWT_OUT(std::cerr << "[Newton] Invalid Jacobian" << std::endl);
                        return false;
                    }
                    
                    //----------------------------------------------------------
                    // condition value
                    //----------------------------------------------------------
                    for(size_t i=n;i>0;--i) A[i] = Fabs(W[i]);
                    quicksort(A);
                    const real_t amax = A[n];
                    if(amax<=0)
                    {
                        Y_NEWT_OUT(std::cerr << "[Newton] Singular Jacobian" << std::endl);
                        return false;
                    }
                    const real_t amin = A[1];
                    icond = amin/amax;
                    
                    Y_NEWT_OUT(std::cerr << "[Newton] icond=" << icond << std::endl);
                    return true;
                }
                
                //==============================================================
                //
                // Convergence On Variables
                //
                //==============================================================
                inline bool has_converged()
                {
                    bool cvg = true;
                    for(size_t i=n;i>0;--i)
                    {
                        const real_t dx = Fabs(XX[i]-X[i]);
                        const real_t sx = Fabs(XX[i]) + Fabs(X[i]);
                        if( dx+dx > ftol * sx )
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
                inline bool solve()
                {
                    
                    //----------------------------------------------------------
                    //
                    // initialize @X: F and f0
                    //
                    //----------------------------------------------------------
                    func(F,X);
                    status_type status = using_nr;
                    real_t      f0     = energy_of(F);
                    
                    
                    Y_NEWTON_CB(); // show the starting point
                    
                    
                    //----------------------------------------------------------
                    //
                    // loop
                    //
                    //----------------------------------------------------------
                    for(;;)
                    {
                        real_t icond = -1;
                        
                        std::cerr << std::endl;
                        Y_NEWT_OUT(std::cerr << "[Newton]  f=" << f0 << std::endl);
                        Y_NEWT_OUT(std::cerr << "          X=" << X  << std::endl);
                        Y_NEWT_OUT(std::cerr << "          F=" << F  << std::endl);
                        
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
                            tao::neg(g,F);
                            svd<real_t>::solve(J, W, V, g, h);
                            
                            //--------------------------------------------------
                            // try it...will initialize XX and FF
                            //--------------------------------------------------
                            const real_t rho   = -tao::dot(xi,h);
                            const real_t slope = alpha*rho;
                            const real_t fc    = f0-slope;
                            const real_t f1    = H(1);
                            Y_NEWT_OUT(std::cerr << "[Newton] rho = " << rho << std::endl);
                            Y_NEWT_OUT(std::cerr << "[Newton] f1  = " << f1  << " / fr=" << fc << std::endl);
                            
                            if(f1<=fc)
                            {
                                //----------------------------------------------
                                // We descend fast enough
                                //----------------------------------------------
                                Y_NEWT_OUT(std::cerr << "[Newton] Accepting" << std::endl);
                                f0=f1;
                                
                                Y_NEWTON_CB();
                                if(has_converged())
                                {
                                    Y_NEWT_OUT(std::cerr << "[Newton] Convergence" << std::endl);
                                    return true;
                                }
                                
                            }
                            else
                            {
                                //----------------------------------------------
                                // Bad descent: find a backtracking point
                                //----------------------------------------------
                                Y_NEWT_OUT(std::cerr << "[Newton] Backtracking" << std::endl);
                                real_t lam = REAL(0.5);
                                real_t fbk = H(lam);
                                while(fbk>f0-lam*slope)
                                {
                                    lam /= 2;
                                    if( lam <= numeric<real_t>::tiny )
                                    {
                                        Y_NEWT_OUT(std::cerr << "[Newton] Spurious Point" << std::endl);
                                        return false;
                                    }
                                    fbk = H(lam);
                                }
                                Y_NEWT_OUT(std::cerr << "f(" << lam << ")=" << fbk << std::endl);
                                
                                //----------------------------------------------
                                // update and continue
                                //----------------------------------------------
                                f0  = fbk;
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
                            // We enter the loop with a computed gradient=xi
                            //__________________________________________________
                            
                            //--------------------------------------------------
                            // Algorithm for CJ
                            //--------------------------------------------------
                            bool may_return = true;
                            if( using_nr == status )
                            {
                                //----------------------------------------------
                                // first initialization
                                //----------------------------------------------
                                Y_NEWT_OUT(std::cerr << "[Newton] Initialize CJ" << std::endl);
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
                                //----------------------------------------------
                                // next initialization
                                //----------------------------------------------
                                Y_NEWT_OUT(std::cerr << "[Newton] Update CJ" << std::endl);
                                real_t dgg=0, gg=0;
                                for( size_t j=n;j>0;--j)
                                {
                                    gg  += g[j] * g[j];
                                    dgg += (xi[j]+g[j])*xi[j];
                                }
                                if( gg <= 0 )
                                {
                                    Y_NEWT_OUT(std::cerr << "[Newton] Local Extremum" << std::endl);
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
                            Y_NEWT_OUT(std::cerr << "[Newton] Line Minimization" << std::endl);
                            
                            triplet<real_t> x = { 0,      1,  0 };
                            triplet<real_t> f = { f0, H(x.b), 0 };
                            bracket<real_t>::expand(H, x,f);
                            minimize<real_t>(H, x, f, 0);
                            Y_NEWT_OUT(std::cerr << "[Newton] opt = " << x.b << std::endl);
                            f0 = H(x.b);
                            
                            Y_NEWTON_CB();
                            if( has_converged() && may_return)
                            {
                                Y_NEWT_OUT(std::cerr << "[Newton] SubConvergence" << std::endl);
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
            };
            
            
        }
        
        template <>
        bool Newton<real_t>:: solve(Function      &func,
                                    Jacobian      &fjac,
                                    array<real_t> &X,
                                    real_t         ftol,
                                    Callback      *cb)
        {
            assert(X.size()>0);
            newton newt(func,fjac,X,ftol,cb);
            return newt.solve();
        }
        
    }
}
