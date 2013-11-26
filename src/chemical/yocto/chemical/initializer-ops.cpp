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
                vector_t     X0;
                
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
                    L2.ensure(Nc);
                    P2.make(Nc,Nc);
                    mkl::mul_rtrn(P2,P,P);
                    if( !L2.build(P2) )
                        throw exception("singular initializer for pseudo-inverse");
                    
                    //==========================================================
                    // Allocating resources
                    //==========================================================
                    X0.make(M,0);
                    
                    
                    
                    build_guess_composition();
                    std::cerr << "X0=" << X0 << std::endl;
                    normalize_composition(X0);
                    std::cerr << "X0=" << X0 << std::endl;
                }
                
                
                inline void build_guess_composition() throw()
                {
                    mkl::set(X0, 0);
                    for( equilibria::iterator i=cs.begin();i!=cs.end();++i)
                    {
                        const equilibrium &eq = **i;
                        eq.append(X0,ran);
                    }
                }
                
                inline void normalize_composition( vector_t &X )
                {
                    mkl::set(cs.C,X);
                    cs.normalize_C(t);
                    mkl::set(X,cs.C);
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



