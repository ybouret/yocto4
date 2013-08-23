#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        typedef algebra<double> mkl;
        
        
        
        namespace  {
            
            class Initializer
            {
            public:
                equilibria  &cs;
                initializer &cr;
                const size_t N;
                const size_t M;
                const size_t Nc;
                const double t;
                matrix_t     P;
                vector_t     Lam;
                matrix_t     Q;
                matrix_t     P2;
                lu_t         L2;
                vector_t     X0;
                vector_t     X1;
                vector_t     Y;
                vector_t     Mu;
                vector_t     V;
                vector_t     dV;
                vector_t     V0;
                
                Initializer(equilibria        &user_cs,
                            collection        &lib,
                            initializer       &user_cr,
                            const double       user_t,
                            urand32           &ran) :
                cs(user_cs),
                cr(user_cr),
                N(cs.size()),
                M(lib.size()),
                Nc(cr.size()),
                t(user_t),
                P(),
                Lam(),
                Q(),
                P2(),
                L2(),
                X0(),
                X1(),
                Y(),
                Mu(),
                V(),
                dV(),
                V0()
                {
                    //==========================================================
                    // check rank
                    //==========================================================
                    if( N+Nc != M )
                        throw exception("initialier: #species=%u != (#equilibria=%u+#constraints=%u)",
                                        unsigned(M),
                                        unsigned(N),
                                        unsigned(Nc) );
                    
                    //==========================================================
                    // initialize the equilibria
                    //==========================================================
                    cs.build_from(lib);
                    
                    //==========================================================
                    // initialize the concentrations
                    //==========================================================
                    cs.scale_all(t);
                    
                    //==========================================================
                    // build agebraic constraints: matrix P and rhs Lambda
                    //==========================================================
                    P.make(Nc,M);
                    Lam.make(Nc,M);
                    for(size_t i=1; i <= Nc; ++i )
                    {
                        const constraint &Q = * cr[i];
                        Lam[i] = Q.value;
                        for( constraint_weights::const_iterator weight=Q.begin(); weight != Q.end(); ++weight )
                        {
                            const species::ptr &sp = lib[weight->key];
                            P[i][sp->indx]         = *weight;
                        }
                    }
                    std::cerr << "P=" << P << std::endl;
                    std::cerr << "Lam=" << Lam << std::endl;
                    
                    //==========================================================
                    //
                    // build the orthogonal matrix Q by SVD
                    //
                    //==========================================================
                    Q.make(N,M);
                    {
                        //------------------------------------------------------
                        // make a MxM zero matrix, then fill its first colums
                        // with P rows...
                        //------------------------------------------------------
                        matrix_t F(M,M);
                        for(size_t i=1;i<=Nc;++i)
                        {
                            for(size_t j=1;j<=M;++j)
                                F[j][i] = P[i][j];
                        }
                        
                        //------------------------------------------------------
                        // use SVD
                        //------------------------------------------------------
                        matrix_t __V(M,M);
                        vector_t __W(M,0);
                        if( !math::svd<double>::build(F, __W, __V) )
                            throw exception("singular initializer for SVD");
                        
                        //------------------------------------------------------
                        // Fetch the orthogonal space into Q
                        //------------------------------------------------------
                        for(size_t i=1;i<=N;++i)
                        {
                            for(size_t j=1;j<=M;++j)
                                Q[i][j] = F[j][i+Nc];
                        }
                    }
                    std::cerr << "Q=" << Q << std::endl;
                    
                    //==========================================================
                    //
                    // build the partial Moore-Penrose Pseudo Inverse: inv(P*P')
                    //
                    //==========================================================
                    L2.ensure(Nc);
                    P2.make(Nc,Nc);
                    mkl::mul_rtrn(P2,P,P);
                    if( !L2.build(P2) )
                        throw exception("singular initializer for pseudo-inverse");
                    
                    //==========================================================
                    //
                    // Final Memory Allocation
                    //
                    //==========================================================
                    X0.make(M,0);
                    X1.make(M,0);
                    Y.make(M,0);
                    Mu.make(Nc,0);
                    V.make(N,0);
                    dV.make(N,0);
                    V0.make(N,0);
                    
                    //==========================================================
                    //
                    // Initialize the algorithm
                    //
                    //==========================================================
                    
                    
                    //----------------------------------------------------------
                    // compute X0 such that X = X0 + Q'*V
                    //----------------------------------------------------------
                    mkl::set(Mu,Lam);
                    L2.solve(P2, Mu);
                    mkl::mul_trn(X0,P,Mu);
                    project(X0);
                    //std::cerr << "X0=" << X0 << std::endl;
                    
                    //==========================================================
                    //
                    // Algorithm
                    //
                    //==========================================================
                    bool   converged = false;
                    double dV_norm   = 0;
                INIT_STEP:
                    //----------------------------------------------------------
                    // initialize step in X1:
                    // add a random fraction of the scaled composition
                    //----------------------------------------------------------
                    mkl::set(X1,0);
                    for( equilibria::iterator eq = cs.begin(); eq != cs.end(); ++eq )
                    {
                        (**eq).append(X1, ran);
                    }
                    
                    //----------------------------------------------------------
                    // "Legalize" the concentration: compute the initial V value
                    //----------------------------------------------------------
                    mkl::mul(V, Q, X1);
                    dV_norm = -1;
                    
                NEWTON_STEP:
                    //----------------------------------------------------------
                    // build composition: X1 = X0 + Q'*V;
                    //----------------------------------------------------------
                    build_composition();
                    //std::cerr << "X1=" << X1 << std::endl;
                    
                    //----------------------------------------------------------
                    // compute numeric newton step
                    //----------------------------------------------------------
                    mkl::set(cs.C,X1);
                    cs.compute_Gamma_and_Phi(t,false);
                    mkl::mul_rtrn(cs.W, cs.Phi, Q);
                    if( ! cs.LU.build(cs.W) )
                    {
                        std::cerr << "singular newton step" << std::endl;
                        goto INIT_STEP;
                    }
                    mkl::neg(dV,cs.Gamma);
                    cs.LU.solve(cs.W,dV);
                    
                    //----------------------------------------------------------
                    // ok: dV is the Newton step
                    // now, we must take care of numeric noise on all
                    // the matrices we computed.
                    //----------------------------------------------------------
                    //std::cerr << "V="  << V << std::endl;
                    //std::cerr << "dV=" << dV << std::endl;
                    
                    mkl::set(V0,V);         // save V into V0
                    mkl::add(V,dV);         // use Newton's step
                    build_composition();    // deduce new composition, in X1
                    mkl::mul(V, Q, X1);     // compute the corrected V, in dV
                    mkl::set(dV,V);         // compute the "real" dV
                    mkl::sub(dV,V0);        // using all the numerical incertainty
                    if( dV_norm < 0 )
                    {
                        dV_norm = mkl::norm1(dV);
                    }
                    std::cerr << "V1="  << V  << std::endl;
                    std::cerr << "dV1=" << dV << std::endl;
                    converged = true;
                    for(size_t i=N;i>0;--i)
                    {
                        if( Fabs(dV[i]) > Fabs( cs.ftol * V[i] ) )
                            converged = false;
                    }
                    if(!converged) goto NEWTON_STEP;
                    
                    //----------------------------------------------------------
                    // compute error
                    //----------------------------------------------------------
                    build_composition();
                    mkl::set(cs.C,X1);
                    //std::cerr << "X1=" << X1 << std::endl;
                    cs.compute_Gamma_and_Phi(t,false);
                    mkl::mul_rtrn(cs.W, cs.Phi, Q);
                    if( ! cs.LU.build(cs.W) )
                    {
                        std::cerr << "singular final composition" << std::endl;
                        goto INIT_STEP;
                    }
                    cs.LU.solve(cs.W,cs.Gamma);
                    mkl::mul_trn(Y, Q, cs.Gamma);
                    for(size_t i=M;i>0;--i)
                    {
                        double err = Fabs(Y[i]);
                        if(err>0) err = pow(10.0,ceil(Log10(err)));
                        Y[i] = err;
                    }
                    //std::cerr << "dX=" << Y  << std::endl;
                    
                    //----------------------------------------------------------
                    // update X1
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        if( Fabs(X1[i]) <= Y[i] )
                            X1[i] = 0;
                    }
                    //std::cerr << "X2=" << X1 << std::endl;
                    
                    for(size_t i=M;i>0;--i)
                    {
                        if(X1[i]<0)
                            goto INIT_STEP;
                    }
                    
                    //----------------------------------------------------------
                    // make a final normalization
                    //----------------------------------------------------------
                    mkl::set(cs.C, X1);
                    cs.normalize_C(t);
                }
                
                
                //--------------------------------------------------------------
                // X1 = X0 + Q'*V, then X1 is projected
                //--------------------------------------------------------------
                void build_composition()
                {
                    mkl::mul_trn(X1, Q, V);
                    mkl::add(X1, X0);
                    project(X1);
                }
                
                
                void project( vector_t &X ) throw()
                {
                    //==================================================================
                    // initialize first projection
                    //==================================================================
                    mkl::set(Mu, Lam);
                    mkl::mulsub(Mu, P, X);
                    L2.solve(P2, Mu);
                    mkl::mul_trn(Y, P, Mu);
                    mkl::add(X, Y);
                    double Ymin = mkl::norm1(Y);
                    
                    //==================================================================
                    // go on while norm decreases
                    //==================================================================
                    while(true)
                    {
                        mkl::set(Mu, Lam);
                        mkl::mulsub(Mu, P, X);
                        L2.solve(P2, Mu);
                        mkl::mul_trn(Y, P, Mu);
                        mkl::add(X, Y);
                        const double Ytmp = mkl::norm1(Y);
                        if(Ytmp>=Ymin)
                            break;
                        Ymin = Ytmp;
                    }
                    
                }
                
                
                ~Initializer() throw()
                {
                    
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Initializer);
            };
            
        }
        
        
        
        void initializer:: operator()( equilibria &cs, collection &lib, const double t )
        {
            
            Initializer ini(cs,lib,*this,t,ran);
            
        }
        
        
    }
}
