#include "yocto/aqueous/initializer.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/rand.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        constraint::~ constraint() throw() {}
        
        constraint:: constraint( const library &L, double v ) throw() :
        value(v), W(), lib(L) {}
        
        void constraint:: add( const string &id, double w )
        {
            if( ! lib.search(id) )
                throw exception("constraint( no '%s' )", id.c_str() );
            
            if( !W.insert(id, w) )
                throw exception("constraint( multiple '%s' )", id.c_str() );
        }
        
        
        initializer:: ~initializer() throw()
        {
        }
        
        initializer:: initializer( const library &L ) throw() :
        constraints(),
        lib(L)
        {
        }
        
        
        constraint & initializer:: create( double v )
        {
            constraint::ptr p( new constraint(lib,v) );
            constraints.push_back(p);
            return *p;
        }
        
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
        
        void initializer:: operator()( chemsys &cs, double t )
        {
            std::cerr << "# Initializing system" << std::endl;
            array<double> &C  = cs.C;
            const size_t   M  = C.size();
            const size_t   N  = cs.size();
            const size_t   Nc = constraints.size();
            if( Nc+N != M )
                throw exception("#constraints mismatch!");
            
            if( Nc > 0 )
            {
                //-- local variables
                matrix<double> P(Nc,M);
                vector<double> V(Nc,0.0);
                vector<double> C0(M,0.0);
                matrix<double> Q(N,M);
                matrix<double> J(M,Nc);
                matrix<double> F(M,M);
                matrix<double> F2(M,M);
                matrix<double> G(M,M);
                vector<double> Y(N,0.0);
                
                //-- aliased variables
                array<double> &dY     = cs.xi;
                matrix<double> &W     = cs.W;
                matrix<double> &Phi   = cs.Phi;
                const double    ftol  = cs.ftol;
                array<double>  &Gamma = cs.Gamma;
                linsys<double> &solver = cs.solver;
                
                //==============================================================
                //
                // decode constraints
                //
                //==============================================================
                std::cerr << "# Decoding Linear Constraints" << std::endl;
                for( size_t i=1; i <= Nc; ++i )
                {               
                    const constraint &c = *(constraints[i]);
                    V[i] = c.value;
                    for( size_t j=1; j <= M; ++j )
                    {
                        const species &sp = *lib(j);
                        const double  *pW = c.W.search( sp.name );
                        if( pW )
                            P[i][j] = *pW;
                    }
                }
                std::cerr << "P=" << P << std::endl;
                std::cerr << "V=" << V << std::endl;
                
                //==============================================================
                //
                // compute constant part with J = P' * inv(P*P')
                //
                //==============================================================
                {
                    matrix<double> P2(Nc,Nc);
                    solver.ensure(Nc);
                    algebra<double>::mul_rtrn(P2, P, P);
                    if( !solver.LU(P2) )
                        throw exception("Singular intializer");
                    matrix<double> iP2(Nc,Nc);
                    iP2.ld1();
                    solver(P2,iP2);
                    algebra<double>::mul_ltrn(J, P, iP2);
                }
                algebra<double>::mul(C0, J, V);
                std::cerr << "C0=" << C0 << std::endl;
                
            BUILD_Q:
                //--------------------------------------------------------------
                // complete P into F = invertible matrix
                //--------------------------------------------------------------
                cs.solver.ensure(M);
                do 
                {
                    for( size_t i=1; i <= Nc; ++i )
                        for( size_t j=1; j <= M; ++j )
                            F2[i][j] = F[i][j] = P[i][j];
                    for( size_t i=Nc+1; i <= M; ++i )
                        for( size_t j=1; j <= M; ++j )
                            F2[i][j] = F[i][j] = alea<double>();
                }
                while( !solver.LU(F2) );
                
                //--------------------------------------------------------------
                // make an orthonormal sub-matrix => Q
                //--------------------------------------------------------------
                algebra<double>::gram_schmidt(G, F, true);
                for( size_t i=Nc+1, k=1; i <=M; ++i, ++k )
                {
                    for( size_t j=1; j <= M; ++j )
                    {
                        Q[k][j] = G[i][j];
                    }
                }
                std::cerr << "Q=" << Q << std::endl;
                
                
                //==============================================================
                //
                // modified newton
                //
                //==============================================================
                std::cerr << "# Modified Newton Step" << std::endl;
                
                
                solver.ensure(N);
                //--------------------------------------------------------------
                // find a valid starting point
                //--------------------------------------------------------------
            NEWTON_INIT:
                std::cerr << "# Looking for a starting point" << std::endl;
                do {
                    for( size_t j=N; j>0; --j )
                        Y[j] = 1e-5 * ( 0.5 - alea<double>());
                    algebra<double>::mul_trn(C, Q, Y);
                    algebra<double>::add(C,C0);
                }
                while( !is_acceptable(C) );
                
                std::cerr << "# Newton iterations" << std::endl;
                //--------------------------------------------------------------
                // compute newton step
                //--------------------------------------------------------------
            NEWTON_STEP:
                cs.computeGammaAndPhi(t,false);
                algebra<double>::mul_rtrn(W, Phi, Q);
                if( ! solver.LU(W) )
                    goto NEWTON_INIT;
                for( size_t i=N;i>0;--i)
                    dY[i] = -Gamma[i];
                solver(W,dY);
                
                //--------------------------------------------------------------
                // update Y
                //--------------------------------------------------------------
                bool converged = true;
                
                for( size_t i=N;i>0;--i)
                {
                    const double dY_i = dY[i];
                    const double Y_i  = (Y[i] += dY[i]);
                    if( Fabs(dY_i) > Fabs( ftol * Y_i ) )
                        converged = false;
                }
                
                //--------------------------------------------------------------
                // corresponding C
                //--------------------------------------------------------------
                algebra<double>::mul_trn(C, Q, Y);
                algebra<double>::add(C,C0);
                if( !converged ) 
                    goto NEWTON_STEP;
                
                //==============================================================
                //
                // Check position and signess of solution
                //
                //==============================================================
                std::cerr << "# Newton Result" << std::endl;
                std::cerr << "Y=" << Y << std::endl;
                std::cerr << "C=" << C << std::endl;
                
                {
                    double maxAbsC = 0;
                    for( size_t i=M; i >0; --i ) 
                    {
                        const double tmp = C[i];
                        if( fabs(tmp) > fabs(maxAbsC) )
                            maxAbsC = tmp;
                    }
                    if( maxAbsC < 0 )
                    {
                        std::cerr << "# SIGN RESTART!" << std::endl;
                        goto BUILD_Q; //!< numeric problem
                    }
                }
             
                
                //==============================================================
                //
                // numeric corrections : project onto P
                //
                //==============================================================
                vector<double> U(Nc,0.0);
                array<double>  &dC    = cs.dC;
                
                //--------------------------------------------------------------
                // how far are we, initially
                //--------------------------------------------------------------
                algebra<double>::mul(U,P,C);
                algebra<double>::sub(U,V);
                double old_norm = get_max_of(U);
                
                //--------------------------------------------------------------
                // project as best as we can
                //--------------------------------------------------------------
                while(true)
                {
                    algebra<double>::mul(dC, J, U); // dC = J * U
                    algebra<double>::sub(C, dC);
                    algebra<double>::mul(U,P,C);    // U = P * C
                    algebra<double>::sub(U,V);
                    const double new_norm = get_max_of(U);
                    if( new_norm >= old_norm )
                        break;
                    old_norm = new_norm;
                }
                std::cerr << "C=" << C << std::endl;
                
                //==============================================================
                //
                // Find the cutoff
                //
                //==============================================================

                // -- recompute Y
                algebra<double>::mul(Y,Q,C);
                
                //-- compute the fractional Y
                for( size_t i=N; i>0; --i ) Y[i] *= ftol;
                
                //- deduce the error on C
                algebra<double>::mul_trn(dC,Q,Y);
                std::cerr << "dC=" << dC << std::endl;
                
                //-- cutoff
                for( size_t j=M; j>0; --j )
                {
                    if( C[j] <= fabs(dC[j]) ) C[j] = 0;
                }
                std::cerr << "C=" << C << std::endl;

                //==============================================================
                //
                // normalize the solution
                //
                //==============================================================
                cs.normalize(t);
                std::cerr << "C=" << C << std::endl;

            }
            
        }
        
        
    }
    
}
