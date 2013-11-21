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
                vector_t     X0;
                vector_t     X1;
                vector_t     dX;
                vector_t     Mu;
                vector_t     V;
                vector_t     dV;
                vector_t     Y;
                
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
                Xstar(),
                X0(),
                X1(),
                dX(),
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
                    X0.    make(M,  0);
                    X1.    make(M,  0);
                    dX.    make(M,  0);
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
                    mkl::set(X0, 0);
                    for( equilibria::iterator i=cs.begin();i!=cs.end();++i)
                    {
                        const equilibrium &eq = **i;
                        eq.append(X0,ran);
                    }
                    
					std::cerr << "Xguess=" << X0 << std::endl;
                    //----------------------------------------------------------
                    // deduce initial V
                    //----------------------------------------------------------
                    mkl::mul(V, Q, X0);
					std::cerr << "V=" << V << std::endl;
                    
                    //----------------------------------------------------------
                    // recompute initial X0
                    //----------------------------------------------------------
                    build_composition(X0);
					std::cerr << "X0=" << X0 << std::endl;
                    
                    //==========================================================
                    //
                    // first norm init
                    //
                    //==========================================================
#if 0
                    if( !build_next_composition() )
						goto INIT_STEP;
                    old_norm = mkl::norm2(dX);
					std::cerr << "Init=" << old_norm << std::endl;
                    //==========================================================
                    //
                    // forward while increasing norm
                    //
                    //==========================================================
                    while(true)
                    {
                        mkl::set(X0,X1);
                        if( !build_next_composition() ) goto INIT_STEP;
						const double new_norm = mkl::norm2(dX);
						std::cerr << old_norm << " ==> " << new_norm << std::endl;
                        if(new_norm<=old_norm)
                        {
                            // accept an leave
                            mkl::set(X0,X1);
                            std::cerr << "Accept X0=" << X0 << std::endl;
                            old_norm = new_norm;
                            break;
                        }
                        old_norm = new_norm;
                    }
                    
                    
                    //==========================================================
                    //
                    // forward while decreasing norm
                    // starting from X1=X0
                    //
                    //==========================================================
                    while(true)
                    {
                        if( !build_next_composition() ) goto INIT_STEP;
                        const double new_norm = mkl::norm2(dX);
						std::cerr << old_norm << " => " << new_norm << std::endl;
                        if(new_norm>=old_norm)
                        {
                            break; // X0 is the best guest
                        }
                        mkl::set(X0,X1);
                        old_norm = new_norm;
                    }
#endif
                    
                    double old_norm = -1;
                    
                    {
                        ios::ocstream fp("dx.dat",false);
                        for(size_t ITER=1;ITER<=30;++ITER)
                        {
                            if( !build_next_composition() ) goto INIT_STEP;
                            mkl::set(X0,X1);
                            std::cerr << "dX=" << dX << std::endl;
                            std::cerr << "X0=" << X0  << std::endl;
                            std::cerr << "=>" << mkl::norm2(dX) << std::endl;
                            const double new_norm = sqrt(mkl::norm2(dX));
                            if(old_norm>0)
                                fp("%u %g %g\n", unsigned(ITER), new_norm, old_norm );
                            old_norm = new_norm;
                            if(new_norm<=0) break;
                        }
                    }
                    std::cerr << "End of Newton..." << std::endl;
                    std::cerr << "dX=" << dX << std::endl;
                    std::cerr << "X0=" << X0 << std::endl;
                    
                    
                    //==========================================================
                    //
                    // Error evaluation
                    //
                    //==========================================================
                    mkl::set(cs.C,X0);
                    cs.compute_Gamma_and_Phi(t,false);
                    mkl::mul_rtrn(cs.W, cs.Phi, Q);
                    if( ! cs.LU.build(cs.W) )
                    {
                        std::cerr << "singular final composition" << std::endl;
                        goto INIT_STEP;
                    }
                    cs.LU.solve(cs.W,cs.Gamma);
                    mkl::mul_trn(dX, Q, cs.Gamma);
                    
                    for(size_t i=M;i>0;--i)
                    {
                        double    err = Fabs(dX[i]);
                        if(err>0) err = Pow(10.0,Ceil(Log10(err)));
                        dX[i] = err;
                    }
                    
                    //==========================================================
                    //
                    // Truncation/Accept
                    //
                    //==========================================================
                    for(size_t i=M;i>0;--i)
                    {
                        if( Fabs(X0[i]) <= dX[i] )
                            X0[i] = 0;
                    }
                    
                    for(size_t i=M;i>0;--i)
                    {
                        if(X0[i]<0)
                            goto INIT_STEP;
                    }
                    
                    
                    //==========================================================
                    //
                    // Final answer
                    //
                    //==========================================================
                    mkl::set(cs.C, X0);
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
                    // compute Newton's step dV from X0
                    //----------------------------------------------------------
                    mkl::set(cs.C,X0);
                    cs.compute_Gamma_and_Phi(t, false);
                    mkl::mul_rtrn(cs.W, cs.Phi, Q);
                    if( ! cs.LU.build(cs.W) )
                    {
                        std::cerr << "singular newton step" << std::endl;
                        return false;
                    }
                    mkl::neg(dV,cs.Gamma);
                    cs.LU.solve(cs.W,dV);
                    
                    //----------------------------------------------------------
                    // update V
                    //----------------------------------------------------------
                    mkl::add(V, dV);
                    
                    //----------------------------------------------------------
                    // compute the next composition
                    //----------------------------------------------------------
                    build_composition(X1);
                    
                    //----------------------------------------------------------
                    // compute the difference in composition
                    //----------------------------------------------------------
                    mkl::vec(dX,X0,X1);
                    
                    //----------------------------------------------------------
                    // compute the effective V
                    //----------------------------------------------------------
                    mkl::mul(V, Q, X1);
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
