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
                vector_t          C;
                const size_t      nfix;
                const size_t      ndof;
                vector<size_t>    idof;
                matrix_t          J;
                const integer_t   D;
                matrix_t          A;
                matrix_t          Q;
                matrix_t          PA;
                vector_t          dL;
                vector_t          dX;
                matrix_t          PhiQ;
                matrix_t          iPhiQ;
                vector_t          rhs;
                vector_t          dV;
                vector_t          dY;
                vector_t          C0;
                vector_t          C1;
                
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
                C(),
                nfix(0),
                ndof(0),
                idof(),
                J(),
                D(0),
                A(),
                Q(),
                PA(),
                dL(),
                dX(),
                PhiQ(),
                iPhiQ(),
                rhs(),
                dV(),
                dY(),
                C0(),
                C1()
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
                    
                    try
                    {
                        //______________________________________________________
                        //
                        // Optimize It
                        //______________________________________________________
                        __refactor(P, Lambda, eqs.Nu, Cfixed, ifixed);
                        
                        
                        
#if 1
                        std::cerr << "ifixed=" << ifixed << std::endl;
                        std::cerr << "Cfixed=" << Cfixed << std::endl;
                        
                        std::cerr << "Nu0="    << eqs.Nu0  << std::endl;
                        std::cerr << "Nu="     << eqs.Nu   << std::endl;
                        std::cerr << "P="      << P        << std::endl;
                        std::cerr << "Lambda=" << Lambda   << std::endl;
#endif
                        
                        //______________________________________________________
                        //
                        // Reduce Nu
                        //______________________________________________________
                        eqs.find_active_species();
                        
                        //______________________________________________________
                        //
                        // Solution in any case...
                        //______________________________________________________
                        C.make(M,0.0);
                        
                        //______________________________________________________
                        //
                        // special case: no reaction
                        //______________________________________________________
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
                                mkl::set(C,Lambda);
                                crout<double>::solve(P,C);
                                crout<double>::improve(C, Q, P, Lambda);
                            }
                            eqs.restore_topology();
                            return;
                        }
                        
                        //______________________________________________________
                        //
                        // special case: no constraints
                        //______________________________________________________
                        eqs.computeK(t);
                        eqs.compute_scaled_concentrations();
                        std::cerr << "K     =" << eqs.K      << std::endl;
                        std::cerr << "scaled=" << eqs.scaled << std::endl;
                        std::cerr << "gammaC=" << eqs.gammaC << std::endl;
                        
                        //______________________________________________________
                        //
                        // Fixed species and D.O.F
                        //______________________________________________________
                        findDOF();
                        std::cerr << "#fix=" << nfix << std::endl;
                        std::cerr << "#dof=" << ndof << std::endl;
                        std::cerr << "idof=" << idof << std::endl;
                        
                        //______________________________________________________
                        //
                        // Gramian Matrix to check P rank as well
                        //______________________________________________________
                        buildJ();
                        
                        //______________________________________________________
                        //
                        // Orthogonal matrix Q to P
                        //______________________________________________________
                        buildQ();
                        std::cerr << "Q=" << Q << std::endl;
                        
                        //______________________________________________________
                        //
                        // Auxiliary matrices
                        //______________________________________________________
                        PA.make(M,Nc);
                        mkl::mul_ltrn(PA, P, A);
                        std::cerr << "PA=" << PA << std::endl;
                        rint_matrix(PA);
                        
                        dL.make(Nc,0.0);
                        dX.make(M,0.0);
                        PhiQ.make(N,N);
                        iPhiQ.make(N,N);
                        rhs.make(N,0.0);
                        dV.make(N,0.0);
                        dY.make(M,0.0);
                        C0.make(M,0.0);
                        C1.make(M,0.0);
                        
                        //______________________________________________________
                        //
                        // Starting point
                        //______________________________________________________
                        prepareC();
                        mkl::set(C0,C);
                        
                       
                        for(size_t count=1;;++count)
                        {
                            //__________________________________________________
                            //
                            // Initialize Gamma and Phi for non linear part
                            //__________________________________________________
                            eqs.updateGammaAndPhi(C);
                           
                            std::cerr << "C0   =" << C         << std::endl;
                            std::cerr << "Gamma=" << eqs.Gamma << std::endl;
                            std::cerr << "Phi  =" << eqs.Phi   << std::endl;
                            std::cerr << "RMS="   << RMS()     << std::endl;
                            
                            //__________________________________________________
                            //
                            // Compute Phi*Q' and check numerically OK
                            //__________________________________________________
                            mkl::mul_rtrn(PhiQ, eqs.Phi, Q);
                            iPhiQ.assign(PhiQ);
                            if(! crout<double>::build(iPhiQ) )
                            {
                                std::cerr << "# invalid concentration level I" << std::endl;
                                exit(1);
                            }
                            
                            //__________________________________________________
                            //
                            // solve the linear part by projection
                            //__________________________________________________
                            projectC();

                            //__________________________________________________
                            //
                            // compute the effective dX from C0 and projected C
                            //__________________________________________________
                            for(size_t j=M;j>0;--j)
                            {
                                dX[j] = C[j] - C0[j];
                            }
                            std::cerr << "dX=" << dX << std::endl;
                            
                            //__________________________________________________
                            //
                            // prepare the arguments -(Gamma+Phi*dX)
                            //__________________________________________________
                            mkl::neg(rhs,eqs.Gamma);
                            mkl::mulsub(rhs,eqs.Phi,dX);
                            mkl::set(dV,rhs);
                            
                            //__________________________________________________
                            //
                            // solve dV and compute dY
                            //__________________________________________________
                            crout<double>::solve(iPhiQ,dV);
                            crout<double>::improve(dV, PhiQ, iPhiQ, rhs);
                            //std::cerr << "dV=" << dV << std::endl;
                            mkl::mul_trn(dY,Q, dV);
                            //std::cerr << "dY=" << dY << std::endl;
                            
                            mkl::add(C,dY);
                            std::cerr << "C2=" << C << std::endl;
                            
                            //__________________________________________________
                            //
                            // numerically re-project
                            //__________________________________________________
                            projectC();
                            std::cerr << "C3=" << C << std::endl;
                            
                            //__________________________________________________
                            //
                            // Compute the effective total delta C
                            //__________________________________________________
                            for(size_t j=M;j>0;--j)
                            {
                                dX[j] = C[j] - C0[j];
                            }
                            std::cerr << "dC=" << dX << std::endl;
                            const double err = StepRMS();
                            std::cerr << "err=" << err << std::endl;
                            if(err<=0)
                            {
                                std::cerr << "#Precision was reached" << std::endl;
                                break;
                            }
                            mkl::set(C0,C);
                        }
                        
                        
                    }
                    catch(...)
                    {
                        eqs.restore_topology();
                        throw;
                    }
                    eqs.restore_topology();
                    
                    
                }
                
                inline ~BootManager() throw()
                {
                    
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootManager);
                //______________________________________________________________
                //
                // Find D.O.F from fixed indices
                //______________________________________________________________
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
                
                //______________________________________________________________
                //
                // build the J=P*P' matrix
                // and its adjoint A such that J*A=Id * D
                //______________________________________________________________
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
                
                //______________________________________________________________
                //
                // buil the exact integer orthogonal matrix to P
                //______________________________________________________________
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
                
                //______________________________________________________________
                //
                // Prepare a concentration
                //______________________________________________________________
                inline void prepareC()
                {
                    eqs.generate(C,loader.ran);
                    for(size_t j=nfix;j>0;--j)
                    {
                        C[ ifixed[j] ] = Cfixed[j];
                    }
                }
                
                //______________________________________________________________
                //
                // dX to project C on P*C-Lambda=0
                //______________________________________________________________
                inline void compute_dL() throw()
                {
                    mkl::set(dL,Lambda);
                    mkl::mulsub(dL,P,C);
                }
                
                //______________________________________________________________
                //
                // numerically solve P*C=Lambda
                //______________________________________________________________
                inline double prjrms()  throw()
                {
                    compute_dL();
                    double sq = 0;
                    for(size_t i=Nc;i>0;--i)
                    {
                        sq += Square(dL[i]);
                    }
                    return sqrt(sq/Nc);
                }
                
                inline void projectC()
                {
                    
                    // initialize
                    double old_rms = prjrms();
                    while(true)
                    {
                        mkl::mul(dX,PA,dL);
                        for(size_t j=M;j>0;--j)
                        {
                            const double Cj = C[j];
                            C1[j] = Cj;
                            C[j] = (D*Cj+dX[j])/D;
                        }
                    
                        const double new_rms = prjrms();
                        //std::cerr << "old_rms=" << old_rms << "; new_rms=" << new_rms << ";" << std::endl;
                        if(new_rms>=old_rms)
                        {
                            mkl::set(C,C1);
                            return;
                        }
                        old_rms = new_rms;
                    }
                    
                }
                
                
                //______________________________________________________________
                //
                // preconditionned Gamma RMS
                //______________________________________________________________
                inline double RMS() const throw()
                {
                    double sum = 0;
                    for(size_t i=N;i>0;--i)
                    {
                        const double d = eqs.Gamma[i] / eqs.gammaC[i];
                        sum += d*d;
                    }
                    return sqrt(sum/N);
                }
                
                //______________________________________________________________
                //
                // Step RMS
                //______________________________________________________________
                inline double StepRMS() const throw()
                {
                    double sq = 0;
                    for(size_t j=M;j>0;--j)
                    {
                        sq += Square(dX[j]);
                    }
                    
                    return sqrt(sq/M);
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
                C0[i] = mgr.C[i];
            }
            
        }
        
        
    }
    
    
}
