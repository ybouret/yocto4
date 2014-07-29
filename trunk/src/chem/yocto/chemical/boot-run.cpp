#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"

#include "yocto/ordered/sorted-vector.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/math/opt/minimize.hpp"
#include "yocto/math/opt/bracket.hpp"


namespace yocto
{
    namespace chemical
    {
        
        typedef math::algebra<double> mkl;
        static inline int __rint( double x ) throw()
        {
            return int( floor(x+0.5) );
        }
        
        static inline
        bool __check_valid( const array<double> &P ) throw()
        {
            for(size_t j=P.size();j>0;--j)
            {
                if(0!=int(P[j]))
                    return true;
            }
            return false;
        }
        
        
        static inline
        size_t __count_items( size_t &s, const array<double> &P ) throw()
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
        
        static inline
        void __factorize(matrix_t              &P,
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
                const size_t count = __count_items(s,P[i]);
                
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
            
            
        }
        
        
        //======================================================================
        //
        // integer Gram-Schmidt
        //
        //======================================================================
        namespace
        {
            static inline integer_t __inorm_sq( const array<double> &v ) throw()
            {
                integer_t ans = 0;
                for(size_t j=v.size();j>0;--j)
                {
                    const integer_t ii = __rint(v[j]);
                    ans += ii*ii;
                }
                return ans;
            }
            
            static inline integer_t __idot( const array<double> &v, const array<double> &u) throw()
            {
                integer_t ans = 0;
                assert(u.size()==v.size());
                for(size_t j=v.size();j>0;--j)
                {
                    ans += __rint(v[j]) * __rint(u[j]);
                }
                return ans;
            }
            
            static inline void __simplify( array<integer_t> &w ) throw()
            {
                integer_t g = 0;
                const size_t n = w.size();
                for(size_t i=n;i>0;--i)
                {
                    const integer_t wi = w[i];
                    const integer_t ai = wi < 0 ? -wi : wi;
                    if(!ai)
                        continue;
                    
                    for(size_t j=i;j>0;--j)
                    {
                        const integer_t wj  = w[j];
                        const integer_t aj  = wj < 0 ? -wj : wj;
                        if(!aj)
                            continue;
                        
                        const integer_t gij = gcd_of(ai,aj);
                        if(g<=0)
                        {
                            g = gij;
                        }
                        else
                        {
                            g = min_of(g,gij);
                        }
                    }
                }
                if(g>0)
                {
                    for(size_t i=n;i>0;--i)
                    {
                        w[i] /= g;
                    }
                }
            }
        }
        
        
        
        void boot:: igs( matrix_t &A )
        {
            const size_t dim = A.cols;
            const size_t num = A.rows;
            ivector_t    usq(num,0);
            ivector_t    w(dim,0);
            
            for(size_t i=1;i<=num;++i)
            {
                array<double> &v  = A[i];
                for(size_t k=1;k<i;++k)
                {
                    const array<double> &u  = A[k];
                    const integer_t      u2 = usq[k];
                    const integer_t      uv = __idot(u,v);
                    // construct integer vector
                    for(size_t j=dim;j>0;--j)
                    {
                        w[j] = u2*__rint(v[j]) - uv * __rint(u[j]);
                    }
                    __simplify(w);
                    for(size_t j=dim;j>0;--j)
                    {
                        v[j] = w[j];
                    }
                }
                usq[i] = __inorm_sq(v);
            }
            
        }
        
        //======================================================================
        //
        // special CG
        //
        //======================================================================
        
        namespace
        {

            static inline double __RMS( const array<double> &G ) throw()
            {
                const size_t M = G.size();
                double sum_sq = 0;
                for(size_t i=M;i>0;--i)
                {
                    const double g = G[i];
                    sum_sq += g*g;
                }
                return sqrt(sum_sq/M);
            }
            
            class LinMin
            {
            public:
                const vector_t &X;
                const vector_t &dX;
                vector_t       &X1;
                equilibria     &cs;
                explicit LinMin(const vector_t &user_X,
                                const vector_t &user_dX,
                                vector_t       &user_X1,
                                equilibria     &user_cs) throw() :
                X( user_X ),
                dX( user_dX ),
                X1( user_X1 ),
                cs( user_cs )
                {
                }
                
                inline ~LinMin() throw()
                {
                    
                }
                
                inline double F( double u )
                {
                    mkl::set(X1,X);
                    mkl::muladd(X1,u,dX);
                    cs.updateGamma(X1);
                    return __RMS(cs.Gamma);
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(LinMin);
            };
            
        }
        
        
        void equilibria::initialize_with(const boot       &loader,
                                         const collection &lib,
                                         const double      t,
                                         array<double>     &C0)
        {
            static const char fn[] = "chemical::boot: ";
            startup(lib);
            assert(C0.size()>=M);
            try
            {
                
                //______________________________________________________________
                //
                //
                // Find what to do
                //
                //______________________________________________________________
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
                
                // if Nc==0 -> special case
                
                //______________________________________________________________
                //
                //
                // Fill the P matrix and Lambda vector
                //
                //______________________________________________________________
                matrix_t  P(Nc,M);
                vector_t  Lambda(Nc,0.0);
                for(size_t i=1;i<=Nc;++i)
                {
                    const boot::constraint &cc = loader[i];
                    Lambda[i] = cc.sum;
                    cc.fill(P[i]);
                }
                
                std::cerr << "P0="      << P      << std::endl;
                std::cerr << "Lambda0=" << Lambda << std::endl;
                std::cerr << "Nu0    =" << Nu << std::endl;
                
                //______________________________________________________________
                //
                // optimize with singleton(s): find fixed species
                //______________________________________________________________
                vector_t              Cfixed(Nc,as_capacity);
                vector<size_t>        ifixed(Nc,as_capacity);
                __factorize(P, Lambda, Nu, Cfixed, ifixed);
                find_active_species();
                co_qsort(ifixed, Cfixed);
                std::cerr << "ifixed=" << ifixed << std::endl;
                std::cerr << "Cfixed=" << Cfixed << std::endl;
                
                std::cerr << "Nu0=" << Nu0 << std::endl;
                
                std::cerr << "Nu="  << Nu << std::endl;
                std::cerr << "P=" << P << std::endl;
                std::cerr << "Lambda=" << Lambda << std::endl;
                
                //______________________________________________________________
                //
                //
                // special case
                //
                //______________________________________________________________
                if( Nc <= 0 )
                {
                    assert(P.is_square());
                    if(M>0)
                    {
                        lu_t solver(M);
                        if( !solver.build(P) )
                        {
                            throw exception("%ssingular full constraints",fn);
                        }
                        mkl::set(C0,Lambda);
                        lu_t::solve(P,C0);
                        
                    }
                    return;
                }
                
                //______________________________________________________________
                //
                //
                // Fixed species and D.O.F
                //
                //______________________________________________________________
                const size_t Nf = ifixed.size();
                std::cerr << "#fixed=" << Nf << std::endl;
                
                
                const size_t   ndof = M-Nf;
                vector<size_t> idof(ndof,as_capacity);
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
                std::cerr << "idof=" << idof << std::endl;
                
                
                //______________________________________________________________
                //
                //
                // Check rank using the Gramian matrix
                //
                //______________________________________________________________
                matrix_t P2(Nc,Nc);
                for(size_t i=Nc;i>0;--i)
                {
                    for(size_t j=i;j>0;--j)
                    {
                        double sum = 0;
                        for(size_t k=M;k>0;--k)
                        {
                            sum += P[i][k] * P[j][k];
                        }
                        P2[i][j] = P2[j][i] = sum;
                    }
                }
                std::cerr << "P2=" << P2 << std::endl;
                matrix_t AP2(P2);
                
                //______________________________________________________________
                //
                // Compute the determinant using AP2
                //______________________________________________________________
                const int detP2 = __rint(math::__determinant_of(AP2));
                std::cerr << "detP2=" << detP2 << std::endl;
                if(!detP2)
                {
                    throw exception("%s:singular set of constraints",fn);
                }
                
                //______________________________________________________________
                //
                // Compute the adjoint matrix to avoid precision loss
                //______________________________________________________________
                math::adjoint(AP2,P2);
                for(size_t i=1;i<=Nc;++i)
                {
                    for(size_t j=1;j<=Nc;++j)
                    {
                        AP2[i][j] = __rint(AP2[i][j]);
                    }
                }
                std::cerr << "AP2=" << AP2 << std::endl;
                
                //______________________________________________________________
                //
                //
                // Compute the orthogonal space Q
                //
                //______________________________________________________________
                matrix_t Q(N,M);
                
                //______________________________________________________________
                //
                // using P (+) Nu
                //______________________________________________________________
                {
                    matrix_t F(M,M);
                    {
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
                                F[Nc+i][j] = Nu[i][j];
                            }
                        }
                        std::cerr << "F0=" << F << std::endl;
                        if( __rint(math::determinant_of(F)) == 0)
                        {
                            throw exception("constraints are colinear to reactions");
                        }
                        boot::igs(F);
                        std::cerr << "F=" << F << std::endl;
                        for(size_t i=N;i>0;--i)
                        {
                            for(size_t j=M;j>0;--j)
                            {
                                Q[i][j] = F[Nc+i][j];
                            }
                        }
                    }
                }
                std::cerr << "Q=" << Q << std::endl;
                
                //______________________________________________________________
                //
                // find the diagonal terms
                //______________________________________________________________
                ivector_t q(N,0);
                for(size_t i=N;i>0;--i)
                {
                    q[i] = __inorm_sq(Q[i]);
                }
                std::cerr << "q=" << q << std::endl;
                
                //______________________________________________________________
                //
                //
                // Compute Xstar such that P.Xstar = Lamba and Q.Xstar = 0
                //
                //______________________________________________________________
                vector_t Xstar(M,0.0);
                vector_t U(Nc,0.0);
                mkl::mul(U,AP2,Lambda);
                mkl::mul_trn(Xstar, P, U);
                for(size_t j=M;j>0;--j)
                {
                    Xstar[j]/=detP2;
                }
                std::cerr << "Xstar=" << Xstar << std::endl;
                
                //______________________________________________________________
                //
                //
                // Compute the scaled concentrations
                //
                //______________________________________________________________
                computeK(t);
                compute_scaled_concentrations();
                std::cerr << "K=" << K << std::endl;
                std::cerr << "scaled=" << scaled << std::endl;
                std::cerr << "gammaC=" << gammaC << std::endl;
                
                //______________________________________________________________
                //
                //
                // Let us start
                //
                //______________________________________________________________
                
               
                
                vector_t X(M,0.0);
                vector_t X0(M,0.0);
                vector_t X1(M,0.0);
                vector_t G(M,0.0);
                vector_t dX(M,0);
                vector_t dL(Nc,0);
                vector_t dU(Nc,0);
                vector_t dY(M,0.0);
                
                vector_t dV(N,0.0);
                vector_t V(N,0.0);
                
                
                LinMin                          Opt(X,dX,X1,*this);
                math::numeric<double>::function Fopt( &Opt, &LinMin::F);
                
                
            PREPARE_CONC:
                //-- prepare a valid sample
                for(size_t j=Nf;j>0;--j)
                {
                    X[ ifixed[j] ] = Cfixed[j];
                }
                generate(X,loader.ran);
                std::cerr << "X=" << X << std::endl;
                mkl::set(X0,X);
                
                for(size_t sub=1;;++sub)
                {
                    //__________________________________________________________
                    //
                    // prepare Gamma and Phi
                    //__________________________________________________________
                    updateGammaAndPhi(X);
                    std::cerr << "Gamma=" << Gamma << std::endl;
                    std::cerr << "Phi  =" << Phi   << std::endl;
                    
                    //__________________________________________________________
                    //
                    // set W = inv(Phi*Q')
                    //__________________________________________________________

                    mkl::mul_rtrn(W, Phi, Q);
                    if( !LU.build(W))
                    {
                        std::cerr << "singular concentrations" << std::endl;
                        goto PREPARE_CONC;
                    }
                    
                    //__________________________________________________________
                    //
                    // compute dU * detP2 = AdjointP2 * (Lambda-P*X)
                    //__________________________________________________________
                    mkl::set(dL,Lambda);
                    mkl::mulsub(dL,P,X);
                    mkl::mul(dU,AP2,dL);
                    
                    //__________________________________________________________
                    //
                    // compute dX = P'*dU
                    //__________________________________________________________
                    mkl::mul_trn(dX,P,dU);
                    for(size_t ii=M;ii>0;--ii) dX[ii]/=detP2;
                    std::cerr << "delX=" << dX << std::endl;
                    
                    //__________________________________________________________
                    //
                    // compute xi=-(Gamma+Phi*dX)
                    //__________________________________________________________
                    mkl::neg(xi,Gamma);
                    mkl::mulsub(xi, Phi, dX);
                    
                    //__________________________________________________________
                    //
                    // compute dY = Q'*inv(Phi*Q') * xi
                    //__________________________________________________________
                    lu_t::solve(W,xi);
                    mkl::mul_trn(dY,Q,xi);
                    std::cerr << "delY=" << dY << std::endl;
                    
                    //__________________________________________________________
                    //
                    // compute the full Newton's step dX
                    //__________________________________________________________
                    mkl::add(dX,dY);
                    
                    //__________________________________________________________
                    //
                    // move at the full Newton's step
                    //__________________________________________________________
                    mkl::add(X, dX);
                    std::cerr << "X=" << X << std::endl;
                    
                    //__________________________________________________________
                    //
                    // at this, point, the constraints PX=Lambda
                    // are numerically OK
                    //__________________________________________________________
                    mkl::set(X0,X);
                    
                    
                    //__________________________________________________________
                    //
                    // Check the Q step
                    //__________________________________________________________
                    updateGammaAndPhi(X);
                    std::cerr << "Gamma=" << Gamma << std::endl;
                    std::cerr << "Phi  =" << Phi   << std::endl;
                    
                    // Q only
                    mkl::mul_rtrn(W, Phi, Q);
                    if( ! LU.build(W) )
                    {
                        std::cerr << "invalid solution" << std::endl;
                        goto PREPARE_CONC;
                    }
                    mkl::neg(dV,Gamma);
                    lu_t::solve(W, dV);
                    mkl::mul_trn(dX,Q,dV);
                    std::cerr << "dXq=" << dX << std::endl;
                    
                    const double F0 = __RMS(Gamma);
                    const double F1 = Fopt(1.0);
                    std::cerr << "F0=" << F0 << std::endl;
                    std::cerr << "F1=" << F1 << std::endl;
                    
                    math::triplet<double> XX = { 0,  1,  1 };
                    math::triplet<double> FF = { F0, F1, F1};
                    math::bracket<double>::expand(Fopt, XX, FF);
                    math::minimize(Fopt, XX, FF, 0.0);
                    const double alpha=XX.b;
                    std::cerr << "alpha=" << XX.b << std::endl;
                    
                    mkl::muladd(X,alpha,dX);
                    
                    
                    
                    mkl::vec(dX,X0,X);
                    std::cerr << "dX=" << dX << std::endl;
                    bool converged = true;
                    for(size_t j=M;j>0;--j)
                    {
                        if( fabs(dX[j])>0 )
                        {
                            converged = false;
                            break;
                        }
                    }
                    
                    if(converged)
                    {
                        std::cerr << "# Newton II converged" << std::endl;
                        if( !normalize(-1, X, false) )
                        {
                            std::cerr << "-- invalid solution" << std::endl;
                            goto PREPARE_CONC;
                        }
                        break;
                    }
                    
                    mkl::set(X0,X);
                }
                
                updateGamma(X);
                std::cerr << "Gamma=" << Gamma << std::endl;
                //______________________________________________________________
                //
                // All Done ! Restore Topology after catch
                //______________________________________________________________
                for(size_t j=M;j>0;--j) C0[j] = X[j];
                std::cerr << "C0=" << C0 << std::endl;
                std::cerr << "#"       << std::endl;
                std::cerr << "Nu="     << Nu << std::endl;
                std::cerr << "P="      << P << std::endl;
                std::cerr << "Lambda=" << Lambda << std::endl;
                std::cerr << "Q="      << Q << std::endl;
                std::cerr << "Xstar="  << Xstar << std::endl;
                
            }
            catch(...)
            {
                restore_topology();
                throw;
            }
            std::cerr << "#DONE, restoring topology" << std::endl;
            restore_topology();
            if( !normalize(-1,C0,false) )
            {
                throw exception("-- unexpected full topology failure");
            }
        }
        
        
    }
}
