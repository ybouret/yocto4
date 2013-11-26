#include "yocto/chemical/initializer.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/svd.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/ios/ocstream.hpp"

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
                urand32     &ran;
                lu_t        &L;
                matrix_t    &W;
                matrix_t    &Phi;
                vector_t    &Gamma;
                const size_t N;
                const size_t M;
                const size_t Nc;
                const double t;
                matrix_t     P;
                vector_t     Lam;
                matrix_t     Q;
                matrix_t     P2;
                lu_t         L2;
                vector_t     Xstar;
                vector_t     &C;
                vector_t      C1;
                vector_t     &dC;
                vector_t     Mu;
                vector_t     V;
                vector_t     dV;
                vector_t     Y;
                
                Initializer(equilibria        &user_cs,
                            collection        &lib,
                            initializer       &user_cr,
                            const double       user_t,
                            urand32           &user_ran) :
                cs(user_cs),
                cr(user_cr),
                ran(user_ran),
                L(cs.LU),
                W(cs.W),
                Phi(cs.Phi),
                Gamma(cs.Gamma),
                N(cs.size()),
                M(lib.size()),
                Nc(cr.size()),
                t(user_t),
                P(),
                Lam(),
                Q(),
                P2(),
                L2(),
                Xstar(),
                C(cs.C),
                C1(),
                dC(cs.dC),
                Mu(),
                V(),
                dV(),
                Y()
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
                    // scale the concentrations
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
                    Xstar. make(M,  0);
                    C1.    make(M,  0);
                    Mu.    make(Nc, 0);
                    V.     make(N,  0);
                    dV.    make(N,  0);
                    Y.     make(M,  0);
                    
                    
                    //==========================================================
                    //
                    // Initialize the algorithm
                    //
                    //==========================================================
                    
                    
                    //----------------------------------------------------------
                    // compute Xstar such that X = Xstar + Q'*V
                    //----------------------------------------------------------
                    mkl::set(Mu,Lam);
                    L2.solve(P2, Mu);
                    mkl::mul_trn(Xstar,P,Mu);
                    project(Xstar);
                    
                    std::cerr << "Xstar=" << Xstar << std::endl;
                    
                    
                INIT_STEP:
                    //==========================================================
                    //
                    // Step initialization
                    //
                    //==========================================================
                    
                    //----------------------------------------------------------
                    // build a guess composition from equilibria
                    //----------------------------------------------------------
                    mkl::set(C, 0);
                    for( equilibria::iterator i=cs.begin();i!=cs.end();++i)
                    {
                        const equilibrium &eq = **i;
                        eq.append(C,ran);
                    }
                    
					std::cerr << "Xguess=" << C << std::endl;
                    cs.normalize_C(t);
                    std::cerr << "Xchem=" << C << std::endl;
                    
                    //----------------------------------------------------------
                    // deduce initial V
                    //----------------------------------------------------------
                    mkl::mul(V, Q, C);
					std::cerr << "V=" << V << std::endl;
                    
                    //----------------------------------------------------------
                    // recompute initial X0
                    //----------------------------------------------------------
                    build_composition(C);
					std::cerr << "X0=" << C << std::endl;
                    
                    //==========================================================
                    //
                    // Looping
                    //
                    //==========================================================
                   
                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    ios::ocstream fp("dx.dat",false);
                    size_t ITER=1;
                    if( !build_next_composition()) goto INIT_STEP;
                    double old_rms = getRMS();
                    fp("%u %g\n", unsigned(ITER), old_rms);
                    
                    const size_t ITER_MIN_PER_COMPONENT = 8;
                    const size_t ITER_MIN = ITER_MIN_PER_COMPONENT * M;
                    for(;;++ITER)
                    {
                        if( !build_next_composition()) goto INIT_STEP;
                        const double new_rms = getRMS();
                        fp("%u %g\n", unsigned(ITER), new_rms);
                        if(ITER>=ITER_MIN)
                        {
                            if(new_rms>=old_rms) break;
                        }
                        
                        old_rms = new_rms;
                    }
                    
                    
                    std::cerr << "End of Newton..." << std::endl;
                    std::cerr << "dC=" << dC << std::endl;
                    std::cerr << " C=" << C  << std::endl;
                    
                    
                    //==========================================================
                    //
                    // Error evaluation
                    //
                    //==========================================================
                    cs.compute_Gamma_and_Phi(t,false);
                    mkl::mul_rtrn(W, Phi, Q);
                    if( !L.build(W) )
                    {
                        std::cerr << "singular final composition" << std::endl;
                        goto INIT_STEP;
                    }
                    L.solve(W,Gamma);
                    mkl::mul_trn(dC, Q, Gamma);
                    
                    for(size_t i=M;i>0;--i)
                    {
                        double    err = Fabs(dC[i]);
                        if(err>0) err = Pow(10.0,Ceil(Log10(err)));
                        dC[i] = err;
                    }
                    
                    //==========================================================
                    //
                    // Truncation/Accept
                    //
                    //==========================================================
                    for(size_t i=M;i>0;--i)
                    {
                        if( Fabs(C[i]) <= dC[i] )
                            C[i] = 0;
                    }
                    
                    for(size_t i=M;i>0;--i)
                    {
                        if(C[i]<0)
                            goto INIT_STEP;
                    }
                    
                    
                    //==========================================================
                    //
                    // Final answer
                    //
                    //==========================================================
                    cs.normalize_C(t);
                }
                
                
                //--------------------------------------------------------------
                // X = Xstar + Q'*V, then X1 is projected
                //--------------------------------------------------------------
                inline void build_composition(vector_t &X) throw()
                {
                    mkl::mul_trn(X, Q, V);
                    mkl::add(X, Xstar);
                    project(X);
                }
                
                //--------------------------------------------------------------
                // compute X1 and dX from X0
                //--------------------------------------------------------------
                inline bool build_next_composition() throw()
                {
                    //----------------------------------------------------------
                    // compute Newton's step dV from C
                    //----------------------------------------------------------
                    cs.compute_Gamma_and_Phi(t, false);
                    mkl::mul_rtrn(W, Phi, Q);
                    if( ! L.build(W) )
                    {
                        std::cerr << "singular newton step" << std::endl;
                        return false;
                    }
                    mkl::neg(dV,Gamma);
                    L.solve(W,dV);
                    
                    //----------------------------------------------------------
                    // update V
                    //----------------------------------------------------------
                    mkl::add(V, dV);
                    
                    //----------------------------------------------------------
                    // compute the next composition
                    //----------------------------------------------------------
                    build_composition(C1);
                    
                    //----------------------------------------------------------
                    // compute the difference in composition
                    //----------------------------------------------------------
                    mkl::vec(dC,C,C1);
                    mkl::set(C,C1);
                    
                    //----------------------------------------------------------
                    // compute the effective V
                    //----------------------------------------------------------
                    mkl::mul(V, Q, C);
                    return true;
                }
                
                
                inline void project( vector_t &X ) throw()
                {
                    //==========================================================
                    // initialize first projection
                    //==========================================================
                    mkl::set(Mu, Lam);
                    mkl::mulsub(Mu, P, X);
                    L2.solve(P2, Mu);
                    mkl::mul_trn(Y, P, Mu);
                    mkl::add(X, Y);
                    double Ymin = mkl::norm_infty(Y);
                    
                    //==========================================================
                    // go on while norm decreases
                    //==========================================================
                    while(true)
                    {
                        mkl::set(Mu, Lam);
                        mkl::mulsub(Mu, P, X);
                        L2.solve(P2, Mu);
                        mkl::mul_trn(Y, P, Mu);
                        mkl::add(X, Y);
                        const double Ytmp = mkl::norm_infty(Y);
                        if(Ytmp>=Ymin)
                            break;
                        Ymin = Ytmp;
                    }
                    
                }
                
                inline double getRMS(void) const throw() { return sqrt( mkl::norm2(dC)/M ); }
                
                               
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
