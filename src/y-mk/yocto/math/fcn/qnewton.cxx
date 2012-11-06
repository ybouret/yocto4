#include "yocto/math/fcn/newton.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/code/hsort.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    
    namespace math
    {
        
        namespace
        {
            
            static inline
            real_t energy_of( const array<real_t> &F ) throw()
            {
                real_t sum = 0;
                for(size_t j=F.size();j>0;--j)
                    sum += F[j] * F[j];
                return REAL(0.5) * sum;
            }
            
            class energy
            {
            public:
                inline energy(Newton<real_t>::Function &f,
                              const array<real_t>      &p,
                              const array<real_t>      &d) :
                fcn_ref( f ),
                pos_ref( p ),
                dir_ref( d ),
                n( p.size() ),
                F(n,0),
                X(n,0)
                {
                    
                }
                
                Newton<real_t>::Function &fcn_ref;
                const array<real_t>      &pos_ref;
                const array<real_t>      &dir_ref;
                const size_t              n; //!< dimension
                vector<real_t>            F; //!< trial function values
                vector<real_t>            X; //!< trial position values
                
                inline ~energy() throw()
                {
                    
                }
                
                inline real_t compute( real_t lambda )
                {
                    for(size_t j=n;j>0;--j)
                        X[j] = pos_ref[j] + lambda * dir_ref[j];
                    fcn_ref( F, X );
                    return energy_of(F);
                }
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(energy);
            };
            
        }
        
        
        template <>
        bool Newton<real_t>:: qsolve( Function &func, Jacobian &jac, array<real_t> &X, real_t ftol )
        {
            const real_t alpha = 1e-4;
            
            const size_t              n = X.size(); assert(n>0);
            matrix<real_t>            J(n,n);
            const matrix<real_t>     &U = J;   //!< once SVD is done
            vector<real_t>            F(n,0);  //!< currrent function values
            vector<real_t>            g(n,0);
            vector<real_t>            h(n,0);  //!< current full step, for energy as well
            matrix<real_t>            V(n,n);  //!< for SVD
            vector<real_t>            W(n,0);  //!< for SVD
            vector<real_t>            Wa(n,0); //!< |Wa|, for condition
            vector<real_t>            z(n,0);  //!< W * transpose(U) * F
            energy                    nrj( func, X, h );
            numeric<real_t>::function E( &nrj, & energy::compute );
            const array<real_t>       &EF = nrj.F;
            const array<real_t>       &EX = nrj.X;
            
            //==================================================================
            //
            // initialize functions and values
            //
            //==================================================================
            func(F,X);
            real_t G0 = energy_of(F);
            
            for(;;)
            {
                //==============================================================
                //
                // compute Jacobian for X,F and G0 computed
                //
                //==============================================================
                std::cerr << "[QuasiNewton]: Initialize Step" << std::endl;
                jac(J,X);
                std::cerr << "X=" << X  << std::endl;
                std::cerr << "F=" << F  << std::endl;
                std::cerr << "G=" << G0 << std::endl;
                
                for( size_t j=n;j>0;--j)
                {
                    real_t s = 0;
                    for( size_t k=n;k>0;--k) s += F[k] * J[k][j];
                    g[j] = s;
                }
                std::cerr << "grad=" << g << std::endl;
                
                //==============================================================
                //
                // compute svd
                //
                //==============================================================
                if( !svd<real_t>::build(J, W, V) )
                {
                    std::cerr << "[QuasiNewton]: very bad Jacobian" << std::endl;
                    return false;
                }
                std::cerr << "W=" << W << std::endl;
                
                //==============================================================
                //
                // analyze condition
                //
                //==============================================================
                for(size_t j=n;j>0;--j) Wa[j] = Fabs(W[j]);
                hsort(Wa);
                std::cerr << "Wa=" << Wa << std::endl;
                const real_t wmax = Wa[n];
                if( wmax <= 0 )
                {
                    std::cerr << "[QuasiNewton]: Null Jacobian" << std::endl;
                    return false;
                }
                const real_t wmin = Wa[1];
                const real_t icnd = wmin / wmax;
                std::cerr << "icnd=" << icnd << std::endl;
                
                //-- the condition is between 0 (bad) and 1 (homothetic matrix)
                //-- somewere around numeric::ftol
                real_t cfac = 0; // --> Newton's step
                static const real_t c_hi = numeric<real_t>::sqrteps;
                static const real_t c_lo = numeric<real_t>::epsilon;
                if( icnd <= c_hi )
                {
                    if( icnd <= c_lo )
                        cfac = 1;
                    else
                    {
                        cfac = 1.0 - log(icnd/c_lo) / log(c_hi/c_lo);
                    }
                }
                // else cfac = 0
                cfac = clamp<real_t>(0,cfac,1);
                std::cerr << "condition_factor=" << cfac << std::endl;
                
                //==============================================================
                //
                // find the regularization factor
                //
                //==============================================================
                
                //--------------------------------------------------------------
                //-- find the median of the positive |W|
                //--------------------------------------------------------------
                size_t m=1;
                while( (m < n) && Wa[m] <= 0 ) ++m;
                const size_t np  = (n-m)+1;
                const size_t hn  = np>>1;
                const size_t hn1 = 1 + hn;
                const real_t beta = (np&1) ? Wa[hn1] : REAL(0.5)*(Wa[hn1] + Wa[hn]);
                std::cerr << "beta=" << beta << std::endl;
                
                //--------------------------------------------------------------
                //-- build the regularized inverse singular values
                //--------------------------------------------------------------
                const real_t tmp   = beta * cfac;
                const real_t beta2 = tmp*tmp;
                for( size_t j=n;j>0;--j)
                {
                    const real_t w = W[j];
                    W[j] /= (w*w+beta2);
                }
                std::cerr << "iW=" << W << std::endl;
                
                
                //==============================================================
                //
                // compute the regularized step from W
                //
                //==============================================================
                for( size_t j=n;j>0;--j)
                {
                    real_t s = 0;
                    for( size_t k=n;k>0;--k)
                        s += U[k][j] * F[k];
                    z[j] = W[j] * s;
                }
                for( size_t j=n;j>0;--j)
                {
                    real_t s = 0;
                    for( size_t k=n;k>0;--k)
                        s += V[j][k] * z[k];
                    h[j] = -s;
                }
                std::cerr << "h=" << h << std::endl;
                
                //==============================================================
                //
                // compute the enrgy slope
                //
                //==============================================================
                real_t slope = 0;
                for( size_t j=n;j>0;--j) slope += g[j] * h[j];
                std::cerr << "slope=" << slope << std::endl;
                if( slope >= 0 )
                {
                    std::cerr << "[QuasiNewton]: invalid slope!" << std::endl;
                    return false;
                }
                
                //==============================================================
                //
                // Evaluate G1
                //
                //==============================================================
                real_t G1 = E(1);
                std::cerr << "G1=" << G1 << std::endl;
                if( G1 <= G0 + alpha * slope )
                {
                    std::cerr << "[QuasiNewton]: Good Step" << std::endl;
                    bool converged = true;
                    for(size_t j=n;j>0;--j)
                    {
                        X[j] = EX[j];
                        F[j] = EF[j];
                        if( Fabs(h[j]) > Fabs( ftol * X[j] ) )
                            converged = false;
                    }
                    if( converged )
                    {
                        std::cerr << "[QuasiNewton]: Converged" << std::endl;
                        return true;
                    }
                    G0 = G1;
                }
                else
                {
                    std::cerr << "[QuasiNewton]: BackTrack" << std::endl;
                    //----------------------------------------------------------
                    //
                    // find a backtracking starting point
                    //
                    //----------------------------------------------------------
                    real_t lam = alpha;
                    for(;;)
                    {
                        G1 = E(lam);
                        if( G1 < G0 )
                            break;
                        lam *= REAL(0.5);
                        if( lam <= numeric<real_t>::tiny )
                        {
                            std::cerr << "[QuasiNewton]: Can't BackTrack" << std::endl;
                            return false;
                        }
                    }
                    std::cerr << "[QuasiNewton]: BackTrack From G(" << lam << ")=" << G1 << std::endl;
                    
                    //----------------------------------------------------------
                    //
                    // bracket minimum
                    //
                    //----------------------------------------------------------
                    triplet<real_t> x = { 0,  lam, 0 };
                    triplet<real_t> f = { G0, G1,  0 };
                    bracket<real_t>::expand(E,x,f);
                    minimize<real_t>(E, x, f, ftol);
                    lam = x.b;
                    G0  = E(lam);
                    std::cerr << "[QuasiNewton]: Backtrack G(" << lam << ")=" << G0 << std::endl;
                    for(size_t j=n;j>0;--j)
                    {
                        X[j] = EX[j];
                        F[j] = EF[j];
                    }
                }
                
                
            }
            
            return false;
        }
        
        
    }
    
}

