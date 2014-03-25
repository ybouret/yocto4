#include "yocto/aqueous/initializer.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/rand.hpp"

namespace yocto
{
    
    using namespace math;
    
    namespace aqueous
    {
        
        namespace
        {
            typedef algebra<double> mkl;
            
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
                const size_t   M;  // #components
                const size_t   N;  // #reactions
                const array<constraint::ptr> &constraints;
                const size_t   Nc;    // #constraints
                vector<double> Xstar; // constant part
                matrix<double> P;     // constraint matrix
                vector<double> L;     // constraints values
                matrix<double> Q;     // orthogonal to constraint
                vector<double> U;     // helper for linear improvement
                matrix<double> J;     // helper for linear improvement
                array<double> &dX;    // cs.dC, for computations
                
                inline
                void improveX()
                {
                    double old_norm = get_max_of(X);
                    
                    //----------------------------------------------------------
                    // project as best as we can
                    //----------------------------------------------------------
                    while(true)
                    {
                        algebra<double>::mul(U,P,X);
                        algebra<double>::sub(U,L);
                        algebra<double>::mul(dX, J, U);
                        algebra<double>::sub(X,dX);
                        const double new_norm = get_max_of(dX);
                        //std::cerr << "lindX=" << dX << std::endl;
                        if(new_norm>=old_norm)
                            break;
                        old_norm = new_norm;
                    }
                    
                }
                
                inline
                void buildX( const array<double> &v ) throw()
                {
                    assert( v.size() == N );
                    mkl::mul_trn(X, Q, v);
                    mkl::add(X,Xstar);
                    improveX();
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
                P(Nc,M),
                L(Nc,0),
                Q(N,M),
                U(Nc,0),
                J(M,Nc),
                dX( cs.dC )
                {
                    static const char fn[] = "aqueous::init";
                    const library  &lib   = cs.lib;
                    lu<double> LU(Nc);
                    
                    //==========================================================
                    //
                    // constraints to algebraic relations
                    //
                    //==========================================================
                    //std::cerr << "# Decoding Linear Constraints" << std::endl;
                    for( size_t i=1; i <= Nc; ++i )
                    {
                        const constraint &c = *(constraints[i]);
                        L[i] = c.value(t);
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
                    // and J = P' * inv(P*P')
                    //
                    //==========================================================
                    //std::cerr << "#Create constant part" << std::endl;
                    {
                        matrix<double> P2(Nc,Nc);
                        mkl::mul_rtrn(P2, P, P);
                        if( !LU.build(P2) )
                        {
                            throw exception("%s(singular constraints/PseudoInverse)", fn);
                        }
                        vector<double> tmp(Nc,0);
                        for(size_t i=Nc;i>0;--i)
                            tmp[i] = L[i];
                        LU.solve(P2,tmp);
                        mkl::mul_trn(Xstar, P, tmp);
                        matrix<double> iP2(Nc,Nc);
                        iP2.ld1();
                        LU.solve(P2,iP2);
                        mkl::mul_ltrn(J, P, iP2);
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
                            throw exception("%s(singular constraints/SVD)", fn);
                        
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
                        //std::cerr << "Q=" << Q << std::endl;
                    }
                    
                    
                    //==========================================================
                    //
                    // Prepare the Newton Algorithm,
                    //
                    //==========================================================
                    const double    amplitude = max_of(1e-7,get_max_of(Xstar));
                    vector<double>  V(N,0);
                    matrix<double> &W         = cs.W;
                    array<double>  &dV        = cs.xi;
                    matrix<double> &Phi       = cs.Phi;
                    array<double>  &Gamma     = cs.Gamma;
                    bool            converged = false;
                    const double    ftol      = cs.ftol;
                    
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
                    //std::cerr << "X0=" << X << std::endl;
                    
                NEWTON_STEP:
                    //----------------------------------------------------------
                    // compute jacobian
                    //----------------------------------------------------------
                    cs.computeGammaAndPhi(t,false);
                    mkl::mul_rtrn(W, Phi, Q);
                    if( !cs.LU.build(W) )
                    {
                        //std::cerr << "Singular Jacobian" << std::endl;
                        goto NEWTON_INIT;
                    }
                    
                    //----------------------------------------------------------
                    // compute newton's step
                    //----------------------------------------------------------
                    for( size_t i=N;i>0;--i) dV[i] = -Gamma[i];
                    cs.LU.solve(W,dV);
                    mkl::add(V,dV);
                    buildX(V);
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
                    
                    //----------------------------------------------------------
                    // check that we have an acceptable solution
                    //----------------------------------------------------------
                    if( !is_acceptable(X) )
                        goto NEWTON_INIT;
                    
                    //std::cerr << "X1=" << X << std::endl;
                    
                    if(!is_acceptable(X))
                        goto NEWTON_INIT;
                    
                    //==========================================================
                    //
                    // Error evaluation : dX = Q'*(Phi*Q')^-1*Gamma
                    //
                    //==========================================================
                    //std::cerr << "Craw=" << X << std::endl;
                    //----------------------------------------------------------
                    // compute the exact term
                    //----------------------------------------------------------
                    cs.computeGammaAndPhi(t, false);
                    algebra<double>::mul_rtrn(W, Phi, Q);
                    if( ! cs.LU.build(W) )
                    {
                        //std::cerr << "Singular Jacobian/Error" << std::endl;
                        goto NEWTON_INIT;
                    }
                    cs.LU.solve(W,Gamma);
                    algebra<double>::mul_trn(dX, Q, Gamma);
                    
                    //----------------------------------------------------------
                    // round up
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        const double dX_i = Fabs(dX[i]) + numeric<double>::tiny;
                        const double dX10 = pow(10.0,ceil(log10(dX_i)));
                        dX[i] = dX10;
                    }
                    //std::cerr << "Cerr=" << dX << std::endl;
                    
                    //----------------------------------------------------------
                    // cut-off
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        if( Fabs(X[i]) <= dX[i] ) X[i] = 0;
                        if( X[i] < 0 )
                            throw exception("Invalid Constraints!");
                    }
                    //std::cerr << "C   =" << X << std::endl;
                }
                
                
                inline ~Init() throw()
                {
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Init);
            };
            
            
        }
        
        void initializer:: operator()( chemsys &cs, double t )
        {
            //==================================================================
            //
            // initialize constants and sanity check
            //
            //==================================================================
            array<double> &C  = cs.C;
			const size_t   M  = C.size();
			const size_t   N  = cs.size();
			const size_t   Nc = constraints.size();
			if( Nc+N != M )
				throw exception("#constraints mismatch: #constraints=%u + #equilibria=%u != #components=%u", unsigned(Nc), unsigned(N), unsigned(M) );
            
			if( Nc > 0 )
			{
                Init __init(cs,t,constraints);
            }
            
        }
        
        
        
    }
}
