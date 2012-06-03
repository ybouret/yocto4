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
        
        static inline
        void __hypershere( double radius, array<double> &Y )
        {
            double sum = 0;
            for( size_t i=Y.size(); i >0; --i )
            {
                const double u1 = alea<double>();
                const double u2 = alea<double>();
                const double l1 = -log(u1);
                const double a2 = numeric<double>::two_pi * u2;
                const double z  = sqrt( l1+l1 ) * cos( a2 );
                Y[i] = z;
                sum += z*z;
            }
            const double fac = radius / sqrt(sum);
            for( size_t i=Y.size(); i >0; --i )
            {
                Y[i] *= fac;
            }
        }
        
        static inline bool __valid( const array<double> &Y )
        {
            bool has_positive = false;
            for( size_t i=Y.size();i>0;--i)
            {
                if( Y[i] < 0.0 )
                {
                    return false;
                }
                if( Y[i] > 0 )
                {
                    has_positive = true;
                }
            }
            return has_positive;
        }
        
        double __norm_sq( const array<double> &Y )
        {
            double sum = 0;
            for( size_t i=Y.size();i>0;--i)
            {
                const double temp=Y[i];
                sum += temp * temp;
            }
            return sum;
        }
        
        void initializer:: operator()( chemsys &cs, double t )
        {
            std::cerr << "** Initializing system" << std::endl;
            array<double> &C  = cs.C;
            const size_t   M  = C.size();
            const size_t   N  = cs.size();
            const size_t   Nc = constraints.size();
            if( Nc+N != M )
                throw exception("#constraints mismatch!");
            
            if( Nc > 0 )
            {
                //--------------------------------------------------------------
                //
                // decode constraints
                //
                //--------------------------------------------------------------
                std::cerr << "** Decoding Linear Constraints" << std::endl;
                matrix<double> P(Nc,M);
                vector<double> V(Nc,0.0);
                vector<double> U(Nc,0.0);
                vector<double> C0(M,0.0);
                matrix<double> Q(N,M);
                
                for( size_t i=1; i <= Nc; ++i )
                {               
                    const constraint &c = *(constraints[i]);
                    U[i] = V[i] = c.value;
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
                
                //--------------------------------------------------------------
                //
                // compute constant part
                //
                //--------------------------------------------------------------
                {
                    matrix<double> P2(Nc,Nc);
                    cs.solver.ensure(Nc);
                    algebra<double>::mul_rtrn(P2, P, P);
                    if( !cs.solver.LU(P2) )
                        throw exception("Singular intializer");
                    cs.solver(P2,U);
                    algebra<double>::mul_trn(C0, P, U);
                }
                std::cerr << "C0=" << C0 << std::endl;
                
                {
                    //----------------------------------------------------------
                    // complete P into F = invertible matrix
                    //----------------------------------------------------------
                    cs.solver.ensure(M);
                    matrix<double> F(M,M);
                    matrix<double> F2(M,M);
                    do 
                    {
                        for( size_t i=1; i <= Nc; ++i )
                            for( size_t j=1; j <= M; ++j )
                                F2[i][j] = F[i][j] = P[i][j];
                        for( size_t i=Nc+1; i <= M; ++i )
                            for( size_t j=1; j <= M; ++j )
                                F2[i][j] = F[i][j] = alea<double>();
                    }
                    while( !cs.solver.LU(F2) );
                    
                    //--------------------------------------------------------------
                    // make an orthonormal sub-matrix
                    //--------------------------------------------------------------
                    matrix<double> G(M,M);
                    algebra<double>::gram_schmidt(G, F, true);
                    for( size_t i=Nc+1, k=1; i <=M; ++i, ++k )
                    {
                        for( size_t j=1; j <= M; ++j )
                        {
                            Q[k][j] = G[i][j];
                        }
                    }
                }
                std::cerr << "Q=" << Q << std::endl;
                
                
                //--------------------------------------------------------------
                //
                // modified newton
                //
                //--------------------------------------------------------------
                std::cerr << "** Modified Newton Step" << std::endl;
                const double radius = sqrt( __norm_sq(C0) ) + 1e-7;
                vector<double> Y(N,0.0);
                array<double> &dY = cs.xi;
                array<double> &dC = cs.dC;
                
                cs.solver.ensure(N);
                
                //--------------------------------------------------------------
                // find a valid starting point
                //--------------------------------------------------------------
            NEWTON_INIT:
                std::cerr << "** Looking for a starting point" << std::endl;
                do 
                {
                    __hypershere(radius, Y);
                    algebra<double>::mul_trn(C, Q, Y);
                    algebra<double>::add(C,C0);
                    std::cerr << "Q=" << Q << std::endl;
                    std::cerr << "Y=" << Y << std::endl;
                    std::cerr << "C0=" << C0 << std::endl;
                    std::cerr << "C=" << C << std::endl;
                }
                while( !__valid(C) );
                //std::cerr << "Y=" << Y << std::endl;
                //std::cerr << "C=" << C << std::endl;
                std::cerr << "** Newton iterations" << std::endl;
                //--------------------------------------------------------------
                // compute newton step
                //--------------------------------------------------------------
            NEWTON_STEP:
                cs.computeGammaAndPhi(t);
                algebra<double>::mul_rtrn(cs.W, cs.Phi, Q);
                if( ! cs.solver.LU(cs.W) )
                    goto NEWTON_INIT;
                for( size_t i=N;i>0;--i) dY[i] = - cs.Gamma[i];
                cs.solver(cs.W,dY);
                
                //--------------------------------------------------------------
                // update Y
                //--------------------------------------------------------------
                for( size_t i=N;i>0;--i)
                    Y[i] += dY[i];
                
                //--------------------------------------------------------------
                // corresponding C and dC
                //--------------------------------------------------------------
                algebra<double>::mul_trn(dC, Q, dY);
                algebra<double>::mul_trn(C, Q, Y);
                algebra<double>::add(C,C0);
                std::cerr << "C=" << C << std::endl;
                bool converged = true;
                for( size_t j=M;j>0;--j)
                {
                    if( Fabs(dC[j]) > Fabs( cs.ftol * C[j] ) )
                        converged = false;
                }
                if( !converged ) goto NEWTON_STEP;
                std::cerr << "C=" << C << std::endl;
                cs.computeGammaAndPhi(t);
                std::cerr << "Gamma=" << cs.Gamma << std::endl;
            }
            
        }
        
        
    }
    
}
