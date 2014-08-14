#include "yocto/chemical/boot.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/det.hpp"

namespace yocto
{
    
    using namespace math;
    
    namespace chemical
    {
        
        typedef algebra<double> mkl;
        
        namespace
        {
            //----------------------------------------------------------------------
            //
            // check at least one argument is not zero
            //
            //----------------------------------------------------------------------
            static inline
            bool __check_valid( const array<double> &P ) throw()
            {
                for(size_t j=P.size();j>0;--j)
                {
                    if( 0 != RInt(P[j]) )
                        return true;
                }
                return false;
            }
            
            
            //----------------------------------------------------------------------
            //
            // count not zero items, storing last index position
            //
            //----------------------------------------------------------------------
            static inline
            size_t __count_not_zero_items( size_t &s, const array<double> &P ) throw()
            {
                size_t count = 0;
                s = 0;
                for(size_t j=P.size();j>0;--j)
                {
                    if(int(P[j])!=0)
                    {
                        s = j;
                        ++count;
                    }
                }
                return count;
            }
            
            
            //----------------------------------------------------------------------
            //
            // refactor the constraint matrix
            //
            //----------------------------------------------------------------------
            static inline
            void __refactor(matrix_t              &P,
                            vector_t              &Lambda,
                            matrix_t              &Nu,
                            vector_t              &Cfixed,
                            vector<size_t>        &ifixed )
            {
                const size_t Nc      = P.rows;
                const size_t N       = Nu.rows;
                
                size_t i   = 1; // active line    index
                size_t top = 0; // last singleton index
                Cfixed.free();
                ifixed.free();
                while(i<=Nc)
                {
                    //______________________________________________________________
                    //
                    // count how many valid items
                    //______________________________________________________________
                    size_t       s     = 0;
                    const size_t count = __count_not_zero_items(s,P[i]);
                    
                    //______________________________________________________________
                    //
                    // None: bad
                    //______________________________________________________________
                    if(count<=0)
                        throw exception("unexpected empty constraint[%u]!", unsigned(i));
                    
                    //______________________________________________________________
                    //
                    // general case
                    //______________________________________________________________
                    if(count>1)
                    {
                        ++i; // next line
                        continue;
                    }
                    
                    //______________________________________________________________
                    //
                    // ok, this is a singleton => fixed
                    // find its new position
                    //______________________________________________________________
                    ++top; assert(top<=Nc); assert(top<=i);
                    
                    //______________________________________________________________
                    //
                    // rearrange the rows and Lambda
                    //______________________________________________________________
                    while(i>top)
                    {
                        const size_t j = i-1;
                        P.swap_rows(i, j);
                        cswap(Lambda[i],Lambda[j]);
                        i=j;
                    }
                    
                    //______________________________________________________________
                    //
                    // simplify the singleton row
                    //______________________________________________________________
                    assert(0!=P[i][s]);
                    const double lhs = (Lambda[i] /= P[i][s]);
                    P[i][s] = 1;
                    Cfixed.push_back(lhs);
                    ifixed.push_back(s);
                    
                    //______________________________________________________________
                    //
                    // propagate the information to the rows below
                    //______________________________________________________________
                    for(size_t k=i+1;k<=Nc;++k)
                    {
                        array<double>    &Pk = P[k];
                        double           &p  = Pk[s];
                        if(p>0)
                        {
                            Lambda[k] -= p*lhs;
                            p          = 0.0;
                            if( !__check_valid(Pk) )
                                throw exception("degenerated constraints");
                        }
                    }
                    
                    //______________________________________________________
                    //
                    // simplify Nu from fixed species
                    //______________________________________________________
                    for(size_t k=N;k>0;--k)
                    {
                        Nu[k][s] = 0;
                    }
                    ++i;
                }
                co_qsort(ifixed, Cfixed);
                
            }
            
            static inline void rint_matrix( matrix_t &A ) throw()
            {
                for(size_t i=A.rows;i>0;--i)
                {
                    for(size_t j=A.cols;j>0;--j)
                    {
                        A[i][j] = RInt(A[i][j]);
                    }
                }
            }
            
            class BootManager
            {
            public:
                
                const collection &lib;
                equilibria       &eqs;
                const boot       &loader;
                const size_t      M;
                const size_t      N;
                const size_t      Nc;
                matrix_t          P;
                vector_t          Lambda;
                vector_t          Cfixed;
                vector<size_t>    ifixed;
                vector_t          X;
                const size_t      nfix;
                const size_t      ndof;
                vector<size_t>    idof;
                matrix_t          J;
                const integer_t   D;
                matrix_t          A;
                matrix_t          Q;
                
                static const char fn[];
                
                inline BootManager(const collection &user_lib,
                                   equilibria       &user_eqs,
                                   const boot       &user_loader,
                                   const double t ) :
                lib( user_lib ),
                eqs( user_eqs ),
                loader( user_loader ),
                M( lib.size() ),
                N( eqs.size()  ),
                Nc( loader.size() ),
                P(),
                Lambda(),
                Cfixed(),
                ifixed(),
                X(),
                nfix(0),
                ndof(0),
                idof(),
                J(),
                D(0),
                A(),
                Q()
                {
                    //__________________________________________________________
                    //
                    // Sanity Check....
                    //__________________________________________________________
                    eqs.startup(lib);
                    
                    //__________________________________________________________
                    //
                    // Find what to do
                    //__________________________________________________________
                    if(N>M)
                    {
                        throw exception("%stoo many reactions",fn);
                    }
                    
                    const size_t Nc = loader.size();
                    if(Nc+N!=M)
                    {
                        throw exception("%s#species=%u != #reactions=%u + #constraints=%u ", fn,unsigned(M), unsigned(N), unsigned(Nc));
                    }
                    
                    if(M<=0)
                        return;
                    
                    // if N==0 -> special case, handled later
                    // if Nc==0 -> special case, handled later
                    
                    //__________________________________________________________
                    //
                    // Fill the P matrix and Lambda vector
                    //__________________________________________________________
                    P.make(Nc,M);
                    Lambda.make(Nc,0.0);
                    for(size_t i=1;i<=Nc;++i)
                    {
                        const boot::constraint &cc = loader[i];
                        Lambda[i] = cc.sum;
                        cc.fill(P[i]);
                    }
                    
                    std::cerr << "P0     =" << P       << std::endl;
                    std::cerr << "Lambda0=" << Lambda  << std::endl;
                    std::cerr << "Nu0    =" << eqs.Nu0 << std::endl;
                    
                    Cfixed.ensure(Nc);
                    ifixed.ensure(Nc);
                    
                    //__________________________________________________________
                    //
                    // Optimize It
                    //__________________________________________________________
                    __refactor(P, Lambda, eqs.Nu, Cfixed, ifixed);
                    
                    eqs.find_active_species();
                    
#if 1
                    std::cerr << "ifixed=" << ifixed << std::endl;
                    std::cerr << "Cfixed=" << Cfixed << std::endl;
                    
                    std::cerr << "Nu0="    << eqs.Nu0  << std::endl;
                    std::cerr << "Nu="     << eqs.Nu   << std::endl;
                    std::cerr << "P="      << P        << std::endl;
                    std::cerr << "Lambda=" << Lambda   << std::endl;
#endif
                    
                    //__________________________________________________________
                    //
                    // Solution in any case...
                    //__________________________________________________________
                    X.make(M,0.0);
                    
                    //__________________________________________________________
                    //
                    // special case: no reaction
                    //__________________________________________________________
                    if( N <= 0 )
                    {
                        assert(P.is_square());
                        Q.make(M,M);
                        Q.assign(P);
                        if(M>0)
                        {
                            if( !crout<double>::build(P) )
                            {
                                throw exception("%ssingular full constraints",fn);
                            }
                            mkl::set(X,Lambda);
                            crout<double>::solve(P,X);
                            crout<double>::improve(X, Q, P, Lambda);
                        }
                        return;
                    }
                    
                    //__________________________________________________________
                    //
                    // special case: no constraints
                    //__________________________________________________________
                    eqs.computeK(t);
                    eqs.compute_scaled_concentrations();
                    std::cerr << "K     =" << eqs.K      << std::endl;
                    std::cerr << "scaled=" << eqs.scaled << std::endl;
                    std::cerr << "gammaC=" << eqs.gammaC << std::endl;
                    
                    //______________________________________________________________
                    //
                    // Fixed species and D.O.F
                    //______________________________________________________________
                    findDOF();
                    std::cerr << "#fix=" << nfix << std::endl;
                    std::cerr << "#dof=" << ndof << std::endl;
                    std::cerr << "idof=" << idof << std::endl;
                    
                    //__________________________________________________________
                    //
                    // Gramian Matrix to check P rank as well
                    //__________________________________________________________
                    buildJ();
                    
                    //__________________________________________________________
                    //
                    // Orthogonal matrix Q to P
                    //__________________________________________________________
                    buildQ();
                    std::cerr << "Q=" << Q << std::endl;
                    
                    
                    
                    
                }
                
                inline ~BootManager() throw()
                {
                    
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootManager);
                void findDOF()
                {
                    (size_t&)nfix = ifixed.size();
                    (size_t&)ndof = M-nfix;
                    idof.ensure(ndof);
                    for(size_t j=1;j<=M;++j)
                    {
                        bool is_dof = true;
                        for(size_t i=ifixed.size();i>0;--i)
                        {
                            if( j == ifixed[i] )
                            {
                                is_dof = false;
                                break;
                            }
                        }
                        if(is_dof)
                            idof.push_back(j);
                    }
                }
                
                void buildJ()
                {
                    J.make(Nc,Nc);
                    mkl::mul_rtrn(J,P,P);
                    rint_matrix(J);
                    (integer_t &)D = RInt(determinant_of(J)); //!< J is preserved
                    if(!D)
                        throw exception("%ssingular constraints",fn);
                    std::cerr << "J=" << J << std::endl;
                    std::cerr << "D=" << D << std::endl;
                    A.make(Nc,Nc);
                    adjoint(A,J);
                    rint_matrix(A);
                    std::cerr << "A=" << A << std::endl;
                }
                
                inline void buildQ()
                {
                    Q.make(N,M);
                    matrix_t F(M,M);
                    for(size_t i=Nc;i>0;--i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            F[i][j] = P[i][j];
                        }
                    }
                    
                    for(size_t i=N;i>0;--i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            F[Nc+i][j] = eqs.Nu[i][j];
                        }
                    }
                    
                    if( RInt(determinant_of(F)) == 0)
                    {
                        throw exception("%sconstraints are colinear to reactions",fn);
                    }
                    
                    boot::igs(F);
                    
                    for(size_t i=N;i>0;--i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            Q[i][j] = F[Nc+i][j];
                        }
                    }
                }
                
                inline void prepareX()
                {
                    for(size_t j=nfix;j>0;--j)
                    {
                        X[ ifixed[j] ] = Cfixed[j];
                    }
                    eqs.generate(X,loader.ran);
                }
                
            };
            
            const char BootManager::fn[] = "chemical::boot: ";

            
        }
        
        void boot:: operator()(array<double>    &C0,
                               const collection &lib,
                               equilibria       &cs,
                               double            t )
        {
            assert(C0.size()>=lib.size());
            BootManager mgr(lib,cs,*this,t);
            for(size_t i=lib.size();i>0;--i)
            {
                C0[i] = mgr.X[i];
            }
            
        }
        
        
    }
    
    
}
