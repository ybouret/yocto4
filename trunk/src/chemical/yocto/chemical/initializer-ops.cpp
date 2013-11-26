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
                const size_t N;
                const size_t M;
                const size_t Nc;
                const double t;
                matrix_t     P;
                vector_t     Lam;
                matrix_t     P2;
                lu_t         L2;
                matrix_t     Q;
                vector_t     X0;
                vector_t     dX;
                vector_t     Y;
                vector_t     dU;
                vector_t     dV;
                vector_t     dXU;
                vector_t     dXV;
                vector_t     Gam;
                
                Initializer(equilibria        &user_cs,
                            collection        &lib,
                            initializer       &user_cr,
                            const double       user_t,
                            urand32           &user_ran) :
                cs(user_cs),
                cr(user_cr),
                ran(user_ran),
                N(cs.size()),
                M(lib.size()),
                Nc(cr.size()),
                t(user_t),
                P(),
                Lam(),
                P2(),
                L2(),
                X0()
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
                    // build the partial Moore-Penrose Pseudo Inverse: inv(P*P')
                    //
                    //==========================================================
                    P2.make(Nc,Nc);
                    L2.ensure(Nc);
                    mkl::mul_rtrn(P2,P,P);
                    if( !L2.build(P2) )
                        throw exception("singular initializer for pseudo-inverse");
                    
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
                    // Allocating resources
                    //==========================================================
                    X0.  make(M, 0);
                    dX.  make(M, 0);
                    Y.   make(Nc,0);
                    dU.  make(Nc,0);
                    dV.  make(N, 0);
                    dXU. make(M, 0);
                    dXV. make(M, 0);
                    Gam. make(N, 0);
                    
                    build_guess_composition();
                    std::cerr << "X0=" << X0 << std::endl;
                    
                    ios::ocstream fp("dx.dat",false);
                    for(size_t k=1;k<=100;++k)
                    {
                        compute_dU();
                        compute_dV();
                        mkl:: set(dX,dXU);
                        mkl:: add(dX,dXV);
                        std::cerr << "dX=" << dX << std::endl;
                        const double nrm = sqrt( mkl::norm2(dX));
                        fp("%u %g\n", unsigned(k),  nrm );
                        update_composition();
                        normalize_composition();
                        if(nrm<=0)
                            break;
                    }
                    
                    mkl::set(cs.C,X0);
                }
                
                
                inline void build_guess_composition() throw()
                {
                    mkl::set(X0, 0);
                    for( equilibria::iterator i=cs.begin();i!=cs.end();++i)
                    {
                        const equilibrium &eq = **i;
                        eq.append(X0,ran);
                    }
                    positive_composition();
                    normalize_composition();
                }
                
                inline void positive_composition() throw()
                {
                    for(size_t i=M;i>0;--i)
                    {
                        if(X0[i]<=0) X0[i] = 0;
                    }
                }
                
                inline void normalize_composition()
                {
                    mkl::set(cs.C,X0);
                    cs.normalize_C(t);
                    mkl::set(X0,cs.C);
                }
                
                inline void compute_dU()
                {
                    mkl:: mul(Y, P, X0);
                    mkl:: sub(Y, Lam);
                    L2.solve(P2,Y);
                    mkl:: neg(dU, Y);
                    mkl:: mul_trn(dXU, P, dU);
                    //std::cerr << "dU="  << dU  << std::endl;
                    //std::cerr << "dXU=" << dXU << std::endl;
                }
                
                //! only from a freshly normalized composition
                inline bool compute_dV()
                {
                    mkl::set(cs.C,X0);
                    cs.compute_Gamma_and_Phi(t, false);
                    
                    mkl::mul_rtrn(cs.W, cs.Phi, Q);
                    if( ! cs.LU.build(cs.W) )
                    {
                        std::cerr << "singular newton step" << std::endl;
                        return false;
                    }
                    
                    //-- build the modified Gamma
                    mkl::mul(Gam,cs.Phi,dXU);
                    mkl::add(Gam,cs.Gamma);
                    
                    mkl::neg(dV,Gam);
                    cs.LU.solve(cs.W,dV);
                    
                    mkl:: mul_trn(dXV, Q, dV);
                    
                    //std::cerr << "dV ="  << dV  << std::endl;
                    //std::cerr << "dXV=" << dXV << std::endl;
                    return true;
                }
                
                
                inline void update_composition()
                {
                    double shrink = -1;
                    
                    for(size_t i=1;i<=M;++i)
                    {
                        const double d = dX[i];
                        const double x = X0[i];
                        if(d<0&&(-d>x))
                        {
                            const double alpha = x/(-d);
                            if(shrink>0)
                            {
                                shrink = min_of(alpha,shrink);
                            }
                            else
                                shrink = alpha;
                        }
                    }
                    std::cerr << "shrink=" << shrink << std::endl;
                    
                    //-- use this trick
                    shrink = Fabs(shrink);
                    
                    //-- gone
                    for(size_t i=M;i>0;--i)
                    {
                        X0[i] = max_of<double>(X0[i]+shrink*dX[i], 0.0);
                    }
                }
                
                inline ~Initializer() throw() {}
                
            };
            
        }
        
        
        void initializer:: find( equilibria &cs, collection &lib, const double t )
        {
            Initializer ini(cs,lib,*this,t,ran);
            
        }
        
    }
    
}



