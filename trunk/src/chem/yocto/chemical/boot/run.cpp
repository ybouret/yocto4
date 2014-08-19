#include "yocto/chemical/boot.hpp"
#include "yocto/math/types.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/sort/quick.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/round.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

#include <cstdlib>

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
                matrix_t          J;
                const integer_t   D;
                matrix_t          A;
                matrix_t          Q;
                ivector_t         q;
                matrix_t          PA;
                vector_t          Cstar;
                vector_t          V;
                vector_t          dL;
                vector_t          dX;
                vector_t          C1;
                matrix_t          PhiQ;
                matrix_t          iPhiQ;
                vector_t          rhs;
                vector_t          dV;
                vector_t          V0;
                vector_t          dY;
                vector_t          C0;
                
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
                J(),
                D(0),
                A(),
                Q(),
                q(),
                PA(),
                Cstar(),
                V(),
                dL(),
                dX(),
                C1(),
                PhiQ(),
                iPhiQ(),
                rhs(),
                dV(),
                V0(),
                dY(),
                C0()
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
                        (size_t&)nfix = ifixed.size();
                        
                        
                        
#if 1
                        std::cerr << "#fix=" << nfix << std::endl;
                        std::cerr << "ifixed=" << ifixed << std::endl;
                        std::cerr << "Cfixed=" << Cfixed << std::endl;
                        
                        std::cerr << "Nu0="    << eqs.Nu0  << std::endl;
                        std::cerr << "Nu="     << eqs.Nu   << std::endl;
                        std::cerr << "P="      << P        << std::endl;
                        std::cerr << "Lambda=" << Lambda   << std::endl;
#endif
                        
                        //______________________________________________________
                        //
                        // Reduced Nu => new active species
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
                        // Gramian Matrix to check P rank as well
                        //______________________________________________________
                        buildJ();
                        
                        //______________________________________________________
                        //
                        // Orthogonal matrix Q to P
                        //______________________________________________________
                        buildQ();
                        std::cerr << "Q=" << Q << std::endl;
                        std::cerr << "q=" << q << std::endl;
                        Method2();
                        
                        
                    }
                    catch(...)
                    {
                        eqs.restore_topology();
                        throw;
                    }
                    
                    
                }
                
                inline ~BootManager() throw()
                {
                    
                }
                
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(BootManager);
                double starRMS()
                {
                    mkl::set(dL,Lambda);
                    mkl::mulsub(dL,P,Cstar);
                    double sq = 0;
                    for(size_t i=Nc;i>0;--i)
                    {
                        sq += Square(dL[i]);
                    }
                    return sqrt(sq/Nc);
                }
                
                void compute_Cstar()
                {
                    mkl::mul(Cstar,PA,Lambda);
                    for(size_t j=M;j>0;--j) Cstar[j] /= D;
                    
                    double old_rms = starRMS();
                    for(;;)
                    {
                        mkl::mul(dX,PA,dL);
                        for(size_t j=M;j>0;--j)
                        {
                            const double Cj = Cstar[j];
                            C1[j]     = Cj;
                            Cstar[j]  = (D*Cj + dX[j])/D;
                        }
                        const double new_rms = starRMS();
                        if(new_rms>=old_rms)
                        {
                            std::cerr << "starRMS=" << old_rms << std::endl;
                            mkl::set(Cstar,C1);
                            return;
                        }
                        old_rms = new_rms;
                    }
                    
                }
                
                inline void computeC() throw()
                {
                    mkl::set(C,Cstar);
                    mkl::muladd_trn(C, Q, V);
                    
                }
                
                inline void computeV() throw()
                {
                    for(size_t j=M;j>0;--j)
                    {
                        dX[j] = C[j] - Cstar[j];
                    }
                    mkl::mul(V,Q,dX);
                    for(size_t i=N;i>0;--i) V[i] /= q[i];
                }
                
                
                
                void Method2()
                {
                    //__________________________________________________________
                    //
                    // compute Cstar
                    //__________________________________________________________
                    PA.make(M,Nc);
                    mkl::mul_ltrn(PA, P, A);
                    rint_matrix(PA);
                    std::cerr << "PA=" << PA << std::endl;
                    
                    Cstar.make(M,0.0);
                    V.make(N,0.0);
                    dL.make(Nc,0.0);
                    dX.make(M,0.0);
                    C1.make(M,0.0);
                    PhiQ.make(N,N);
                    iPhiQ.make(N,N);
                    rhs.make(N,0.0);
                    dV.make(N,0.0);
                    V0.make(N,0.0);
                    dY.make(M,0.0);
                    C0.make(M,0.0);
                    compute_Cstar();
                    std::cerr << "Cstar=" << Cstar << std::endl;
                    
                    numeric<double>::function F(this,&BootManager::Func);
                    
                    //__________________________________________________________
                    //
                    // compute initial V and C
                    //__________________________________________________________
                PREPARE_C:
                    prepareC();
                    computeV();
                    computeC();
                    mkl::set(C0,C);
                    eqs.updateGammaAndPhi(C);
                    double F0 = GammaRMS();
                    
                    for(unsigned count=1;
                        ;++count)
                    {
                        //std::cerr << "########### iter=" <<count << std::endl;
                        //std::cerr << "C="    << C << std::endl;
                        //std::cerr << "Gamma=" << eqs.Gamma << std::endl;
                        //std::cerr << "GamRMS=" << GammaRMS() << std::endl;
                        //______________________________________________________
                        //
                        // Compute the full Newton's step
                        //______________________________________________________
                        if(!computeNewtondV())
                        {
                            std::cerr << "#singular concentrations, level-1" << std::endl;
                            goto PREPARE_C;
                        }
                        
                        mkl::set(V0,V);
                        double       alpha = 1.0;
                        const double F1    = F(alpha);
                        //std::cerr << "F0=" << F0 << std::endl;
                        //std::cerr << "F1=" << F1 << std::endl;
                        //______________________________________________________
                        //
                        // Don't go too fast
                        //______________________________________________________
                        if(F1>=F0)
                        {
                            std::cerr << "#Need to optimise" << std::endl;
                            triplet<double> XX = { 0, 1, 1};
                            triplet<double> FF = { F0, F1, F1};
                            (void)bracket<double>::inside(F, XX, FF);
                            minimize<double>(F, XX, FF, 0);
                            alpha = XX.b;
                            std::cerr << "alpha=" << alpha << std::endl;
                        }
                        
                        //______________________________________________________
                        //
                        // compute the new value
                        //______________________________________________________
                        for(size_t i=N;i>0;--i) { V[i] = V0[i] + alpha * dV[i]; }
                        computeC();
                        eqs.updateGammaAndPhi(C);
                        for(size_t j=M;j>0;--j)
                        {
                            dX[j] = C[j] - C0[j];
                        }
                        //std::cerr << "dX=" << dX << std::endl;
                        
                        //______________________________________________________
                        //
                        // until numerical underflow
                        //______________________________________________________
                        const double newRMS = StepRMS();
                        if(newRMS<=0)
                        {
                            break;
                        }
                        
                        F0 = GammaRMS();
                        mkl::set(C0,C);
                        
                    }
                    
                    std::cerr << "#Newton II converged: check..." << std::endl;
                    std::cerr << "C1="     << C << std::endl;
                    std::cerr << "Gamma=" << eqs.Gamma << std::endl;
                    
                    //__________________________________________________________
                    //
                    // compute the approximated errors
                    //__________________________________________________________
                    if( !computeNewtondV() )
                    {
                        std::cerr << "#unexpected singular concentrations, level-2" << std::endl;
                        goto PREPARE_C;
                    }
                    mkl::mul_trn(dX, Q, dV);
                    for(size_t j=M;j>0;--j)
                    {
                        const double d = fabs(dX[j]);
                        if(d>0)
                        {
                            dX[j] = log_round_ceil(d);
                        }
                        else
                        {
                            dX[j] = 0;
                        }
                    }
                    std::cerr << "dC=" << dX << std::endl;
                    
                    //__________________________________________________________
                    //
                    // cut-off
                    //__________________________________________________________
                    for(size_t j=M;j>0;--j)
                    {
                        if((eqs.active[j]>0) &&
                           fabs(C[j]) <= dX[j]
                           )
                        {
                            C[j] = 0.0;
                        }
                    }
                    std::cerr << "C2=" << C << std::endl;
                    
                    
                    //__________________________________________________________
                    //
                    // check convergence
                    //__________________________________________________________
                    if( !eqs.normalize(-1,C,false) )
                    {
                        std::cerr << "#invalid final concentrations" << std::endl;
                        goto PREPARE_C;
                    }
                    std::cerr << "C3=" << C << std::endl;
                    
                    //__________________________________________________________
                    //
                    // full convergence
                    //__________________________________________________________
                    eqs.restore_topology();
                    if( !eqs.normalize(-1,C,false) )
                    {
                        throw exception("%s(unexpected singular concentrations)", fn);
                    }
                    
                    //__________________________________________________________
                    //
                    // linear compliance
                    //__________________________________________________________
                    const double linearRMS = computeNormalizedRMS();
                    //std::cerr << "linearRMS=" << linearRMS << std::endl;
                    if( linearRMS > numeric<double>::ftol )
                    {
                        throw exception("%s(unable to match linear constraints: RMS=%g)", fn, linearRMS);
                    }
                }
                
                inline double computeNormalizedRMS()
                {
                    double sq = 0;
                    for(size_t i=Nc;i>0;--i)
                    {
                        const array<double> &Pi = P[i];
                        double sumP2 = 0;
                        double sumPC = 0;
                        for(size_t j=M;j>0;--j)
                        {
                            const double Pij = Pi[j];
                            sumP2 += Pij*Pij;
                            sumPC += Pij * C[j];
                        }
                        sumPC -= Lambda[i];
                        const double d2 = sumPC * sumPC;
                        sq += d2/sumP2;
                    }
                    return sqrt(sq/Nc);
                }
                
                //______________________________________________________________
                //
                // Compute the Newton's step with improved precision
                //______________________________________________________________
                inline bool computeNewtondV()
                {
                    mkl::mul_rtrn(PhiQ, eqs.Phi, Q);
                    iPhiQ.assign(PhiQ);
                    if( !crout<double>::build(iPhiQ) )
                    {
                        std::cerr << "#unexpected singular concentrations, level-2" << std::endl;
                        return false;
                    }
                    mkl::neg(rhs,eqs.Gamma);
                    mkl::set(dV,rhs);
                    crout<double>::solve(iPhiQ,dV);
                    crout<double>::improve(dV, PhiQ, iPhiQ, rhs);
                    return true;
                }
                
                double Func(const double alpha)
                {
                    for(size_t i=N;i>0;--i)
                    {
                        V[i] = V0[i] + alpha * dV[i];
                    }
                    computeC();
                    eqs.updateGamma(C);
                    return GammaRMS();
                }
                
                
                
                //______________________________________________________________
                //
                // build the J=P*P' matrix
                // and its adjoint A such that J*A=Id * D
                //______________________________________________________________
                inline void buildJ()
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
                    q.make(N,0);
                    for(size_t i=N;i>0;--i)
                    {
                        integer_t sum = 0;
                        for(size_t j=M;j>0;--j)
                        {
                            const integer_t Qi = integer_t(Q[i][j]);
                            sum += Qi * Qi;
                        }
                        q[i] = sum;
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
                inline double GammaRMS() const throw()
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
                
                
                inline void CutActive()
                {
                    for(size_t j=M;j>0;--j)
                    {
                        if(eqs.active[j]>0 && C[j]<=0)
                        {
                            C[j] = 0;
                        }
                    }
                }
                
                inline bool NeedToValidate() const throw()
                {
                    for(size_t j=M;j>0;--j)
                    {
                        if(eqs.active[j]>0 && C[j]<0)
                        {
                            return true;
                        }
                        
                    }
                    return false;
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
