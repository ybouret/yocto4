#include "yocto/aqueous/initializer.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/exception.hpp"
//#include "yocto/math/fcn/newton.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/rand.hpp"

namespace yocto
{
    
    using namespace math;
    
    namespace aqueous
    {
        
        namespace
        {
            static inline bool is_acceptable( const array<double> &C )
            {
                size_t       num_pos = 0;
                size_t       num_neg = 0;
                const size_t count   = C.size();
                
                for( size_t i=count; i > 0 ; --i )
                {
                    const double C_i = C[i];
                    if( C_i > 0 ) ++num_pos; else ++num_neg;
                }
                
                return num_pos > num_neg;
            }
            
            static inline double get_max_of( const array<double> &U )
            {
                const size_t count = U.size();
                assert( count > 0 );
                double ans = fabs( U[1] );
                for( size_t i=2; i <= count; ++i )
                {
                    const double tmp = fabs(U[i]);
                    if( tmp > ans ) ans = tmp;
                }
                return ans;
            }
            
            
            class Init
            {
            public:
                chemsys       &cs;
                const double   t;
                array<double> &X;
                const size_t   M;  // components
                const size_t   N;  // reactions
                const array<constraint::ptr> &constraints;
                const size_t   Nc;      // constraints
                vector<double> Xstar;
                matrix<double> Q;
                //Newton<double>::Function zF;
                // Newton<double>::Jacobian zJ;
                
                
                void buildX( const array<double> &v )
                {
                    assert( v.size() == N );
                    algebra<double>::mul_trn(X, Q, v);
                    algebra<double>::add(X,Xstar);
                }
                
                void newtF( array<double> &f, const array<double> &v)
                {
                    assert( f.size() == N );
                    assert( v.size() == N );
                    buildX(v);
                    cs.computeGammaAndPhi(t,false);
                    algebra<double>::set(f,cs.Gamma);
                }
                
                void newtJ( matrix<double> &J, const array<double> &v )
                {
                    assert( v.size() == N );
                    buildX(v);
                    cs.computeGammaAndPhi(t,false);
                    algebra<double>::mul_rtrn(J, cs.Phi, Q);
                }
                
                inline Init( chemsys &user_cs,
                            double    user_t,
                            const array< constraint::ptr > &user_constraints
                            ) :
                cs( user_cs ),
                t( user_t ),
                X( cs.C ),
                M( X.size() ),
                N( cs.size() ),
                constraints( user_constraints ),
                Nc( constraints.size() ),
                Xstar(M,0),
                Q(N,M)
                //,zF( this, & Init::newtF ),
                //zJ( this, & Init::newtJ )
                {
                    const library  &lib   = cs.lib;
                    linsys<double> &solve = cs.solver;
                    solve.ensure(max_of(Nc,N));
                    
                    //==========================================================
                    //
                    // constraints to algebraic relations
                    //
                    //==========================================================
                    std::cerr << "# Decoding Linear Constraints" << std::endl;
                    matrix<double> P(Nc,M);
                    vector<double> Lambda(Nc,0);
                    for( size_t i=1; i <= Nc; ++i )
                    {
                        const constraint &c = *(constraints[i]);
                        Lambda[i] = c.value(t);
                        for( size_t j=1; j <= M; ++j )
                        {
                            const species &sp = *lib(j);
                            
                            const double  *pW = c.weight_address_of( sp.name );
                            if( pW )
                                P[i][j] = *pW;
                        }
                    }
                    
                    
                    //==========================================================
                    //
                    // create Xstar = Pseudo-Inverse P times Lambda
                    //
                    //==========================================================
                    std::cerr << "#Create constant part" << std::endl;
                    {
                        matrix<double> P2(Nc,Nc);
                        algebra<double>::mul_rtrn(P2, P, P);
                        std::cerr << "P2=" << P2 << std::endl;
                        if( !solve.LU(P2) )
                        {
                            throw exception("Singular Constraints/PseudoInverse");
                        }
                        vector<double> tmp(Nc,0);
                        for(size_t i=Nc;i>0;--i) tmp[i] = Lambda[i];
                        solve(P2,tmp);
                        algebra<double>::mul_trn(Xstar, P, tmp);
                        std::cerr << "Xstar=" << Xstar << std::endl;
                    }
                    
                    //==========================================================
                    //
                    // create the orthogonal matrix part
                    //
                    //==========================================================
                    {
                        matrix<double> F(M,M);
                        //------------------------------------------------------
                        // Nc first rows are the P rows
                        //------------------------------------------------------
                        for( size_t j=1; j <= Nc; ++j )
                        {
                            for( size_t i=1; i <= M; ++i )
                            {
                                F[i][j] = P[j][i];
                            }
                        }
                        
                        //------------------------------------------------------
                        // use SVD to orthonormalize F
                        //------------------------------------------------------
                        matrix<double> svd_v(M,M);
                        vector<double> svd_w(M,0);
                        
                        if( !svd<double>::build(F, svd_w, svd_v) )
                            throw exception("Singular Constraints/SingularValues");
                        
                        //------------------------------------------------------
                        // extract Q rows as the M-Nc columns
                        //------------------------------------------------------
                        for( size_t j=Nc+1,k=1; j<=M; ++j,++k )
                        {
                            for(size_t i=1; i<=M; ++i)
                            {
                                Q[k][i] = F[i][j];
                            }
                        }
                        std::cerr << "Q=" << Q << std::endl;
                    }
                    
                    
                    //==========================================================
                    //
                    // Prepare the Newton Algorithm
                    //
                    //==========================================================
                    const double   amplitude = max_of(1e-7,get_max_of(Xstar));
                    vector<double>  V(N,0);
                    matrix<double> &W      = cs.W;
                    array<double>  &dV     = cs.xi;
                    matrix<double> &Phi    = cs.Phi;
                    array<double>  &Gamma  = cs.Gamma;
                    bool            converged = false;
                    const double    ftol   = 1e-7;
                    
                NEWTON_INIT:
                    //----------------------------------------------------------
                    // initialize step
                    //----------------------------------------------------------
                    do
                    {
                        for( size_t i=N;i>0;--i)
                            V[i] = amplitude * (0.5 - alea<double>());
                        buildX(V);
                    }
                    while( !is_acceptable(X) );
                    std::cerr << "X0=" << X << std::endl;
                    
                NEWTON_STEP:
                    //----------------------------------------------------------
                    // compute jacobian
                    //----------------------------------------------------------
                    cs.computeGammaAndPhi(t,false);
                    algebra<double>::mul_rtrn(W, Phi, Q);
                    if( ! solve.LU(W) )
                    {
                        std::cerr << "Singular Jacobian" << std::endl;
                        goto NEWTON_INIT;
                    }
                    
                    //----------------------------------------------------------
                    // compute newton's step
                    //----------------------------------------------------------
                    for( size_t i=N;i>0;--i) dV[i] = -Gamma[i];
                    solve(W,dV);
                    algebra<double>::add(V,dV);
                    buildX(V);
                    std::cerr << "dV=" << dV << std::endl;
                    std::cerr << "V=" << V << std::endl;
                    std::cerr << "X=" << X << std::endl;
                    converged = true;
                    for( size_t i=N;i>0;--i)
                    {
                        if( fabs(dV[i]) > fabs( ftol * V[i] ) )
                        {
                            converged = false;
                            break;
                        }
                    }
                    if(!converged)
                        goto NEWTON_STEP;
                    
                }
                
                
                inline ~Init() throw()
                {
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Init);
            };
            
            
        }
        
        void initializer:: run( chemsys &cs, double t )
        {
            //==================================================================
            //
            // initialize constants and sanity check
            //
            //==================================================================
			std::cerr << "# Initializing system" << std::endl;
			array<double> &C  = cs.C;
			const size_t   M  = C.size();
			const size_t   N  = cs.size();
			const size_t   Nc = constraints.size();
			if( Nc+N != M )
				throw exception("#constraints mismatch: Nc=%u + N=%u != M=%u", unsigned(Nc), unsigned(N), unsigned(M) );
            
			if( Nc > 0 )
			{
                
                Init __init(cs,t,constraints);
            }
            
        }
        
        
        
    }
}
