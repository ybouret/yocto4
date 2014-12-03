#include "yocto/chemical/equilibria.hpp"
#include "yocto/chemical/boot.hpp"
#include "yocto/exception.hpp"
#include "yocto/sort/quick.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/round.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/math/opt/cgrad.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

#include <cstdlib>

namespace yocto
{
    namespace chemical
    {
        using namespace math;
        
        namespace
        {
            //------------------------------------------------------------------
            //
            // rewriting constraints
            //
            //------------------------------------------------------------------
            static inline
            size_t count_species( const array<ptrdiff_t> &row, size_t &single ) throw()
            {
                size_t ans = 0;
                for(size_t j=row.size();j>0;--j)
                {
                    if(0!=row[j])
                    {
                        single = j;
                        ++ans;
                    }
                }
                return ans;
            }
            
            static inline
            void rewrite_constraints(imatrix_t       &P,
                                     vector_t        &Lam,
                                     uvector_t       &Jfixed,
                                     vector_t        &Cfixed,
                                     const bvector_t &active
                                     )
            
            {
                
                assert(0==Jfixed.size());
                assert(0==Cfixed.size());
                const size_t Nc = P.rows;
                size_t ifixed = 0; // last fixed index
                for(size_t i=1;i<=Nc;++i)
                {
                    size_t       jfixed = 0;
                    const size_t ns = count_species(P[i],jfixed);
                    switch(ns)
                    {
                        case 0:
                            throw exception("equilibria.boot: deduced an empty constraint");
                            
                        case 1:
                        {
                            //-- re order matrix
                            ++ifixed;
                            P.swap_rows(ifixed,i);
                            cswap(Lam[ifixed],Lam[i]);
                            
                            //-- simplify row/level
                            const double cfixed = (Lam[ifixed]/=P[ifixed][jfixed]);
                            P[ifixed][jfixed] = 1;
                            
                            //-- check possible
                            if( (cfixed<0) && active[jfixed])
                                throw exception("equilibria.boot: active species #%u set to a negative value", unsigned(jfixed));
                            
                            //-- propagate information below
                            for(size_t k=ifixed+1;k<=Nc;++k)
                            {
                                const ptrdiff_t p = P[k][jfixed];
                                Lam[k] -= p * cfixed;
                                P[k][jfixed] = 0;
                            }
                            
                            //-- register who/value
                            Jfixed.push_back(jfixed);
                            Cfixed.push_back(cfixed);
                            
                            
                        } break;
                            
                        default:
                            break;
                    }
                }
                co_qsort(Jfixed,Cfixed);
                std::cerr << "Jfixed=" << Jfixed << std::endl;
                std::cerr << "Cfixed=" << Cfixed << std::endl;
            }
            
            
            //------------------------------------------------------------------
            //
            // orthogonal space to constraints
            //
            //------------------------------------------------------------------
            
            //__________________________________________________________________
            //
            // integer Gram-Schmidt
            //__________________________________________________________________
            static inline
            bool IGS( imatrix_t &A)
            {
                const size_t dim = A.cols;
                const size_t num = A.rows;
                ivector_t    usq(num,0);
                ivector_t    w(dim,0);
                
                for(size_t i=1;i<=num;++i)
                {
                    array<integer_t> &v  = A[i];
                    for(size_t k=1;k<i;++k)
                    {
                        const array<integer_t> &u  = A[k];
                        const integer_t         u2 = usq[k];
                        const integer_t         uv = tao::dot(u,v);
                        for(size_t j=dim;j>0;--j)
                        {
                            w[j] = u2* v[j] - uv * u[j];
                        }
                        (void)tao::i_simplify(w);
                        for(size_t j=dim;j>0;--j)
                        {
                            v[j] = w[j];
                        }
                    }
                    const integer_t last = usq[i] = tao::norm_sq(v);
                    if(!last)
                        return false;
                }
                return true;
            }
            
            
            
            //__________________________________________________________________
            //
            // Compute the chemical Q-space
            //__________________________________________________________________
            static inline
            void compute_Qv2(imatrix_t &Q, const imatrix_t &P, const imatrix_t &Nu)
            {
                const size_t N  = Q.rows;
                const size_t Nc = P.rows;
                const size_t M  = Q.cols;
                
                imatrix_t F(P);
                if(!IGS(F))
                    throw exception("unexpected failure in P normalization");
                
                std::cerr << "F=" << F << std::endl;
                
                for(size_t i=N;i>0;--i)
                {
                    array<integer_t> &Qi =Q[i];
                    tao::set(Qi, Nu[i]);
                    for(size_t k=Nc;k>0;--k)
                    {
                        const array<integer_t> &Pk = F[k];
                        const integer_t qp = tao::dot(Qi,Pk);
                        const integer_t p2 = tao::dot(Pk,Pk);
                        for(size_t j=M;j>0;--j)
                        {
                            Qi[j] = p2 * Qi[j] - qp * Pk[j];
                        }
                        (void)tao::i_simplify(Qi);
                    }
                    
                }
            }
            
            
            
        }
        
        
        
        void equilibria:: compute_C(const array<double> &V)
        {
            assert(V.size()==N);
            assert(Delta!=0);
            tao::mul_trn(C,Q,V);
            for(size_t j=M;j>0;--j)
            {
                C[j] = (Xstar[j]+Delta*C[j])/Delta;
            }
            for(size_t j=fixedJ.size();j>0;--j)
            {
                C[ fixedJ[j] ] =  fixedC[j];
            }
        }
        
        
        double equilibria:: computeH( const array<double> &V )
        {
            assert(V.size()==N);
            compute_C(V);
            double H = 0;
            for(size_t j=M;j>0;--j)
            {
                if(aboot[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        H += Cj*Cj;
                    }
                }
            }
            return H*0.5;
        }
        
        void equilibria:: computeG(array<double> &G, const array<double> &V)
        {
            assert(V.size()==N);
            assert(G.size()==N);
            compute_C(V);
            for(size_t j=M;j>0;--j)
            {
                dC[j] = 0;
                if(aboot[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        dC[j] = Cj;
                    }
                }
            }
            tao::mul(G,Q,dC);
        }
        
        void equilibria:: optimize()
        {
            cgrad<double>::optimize(optH,optG, xi, 0, NULL);
            compute_C(xi);
        }
        
        double equilibria:: computeF(double ratio)
        {
            tao::setprobe(xi, xis, ratio, U);
            optimize();
            updateGamma();
            return scaledGamma();
        }
        
        
        
        void equilibria:: create(array<double> &C0, const boot &loader, const double t)
        {
            
            assert(C0.size()>=M);
            //__________________________________________________________________
            //
            //
            // Sanity check
            //
            //__________________________________________________________________
            if(N>M)
                throw exception("equilibria: not enough species!");
            
            const size_t Nc = loader.size();
            if(Nc+N!=M)
                throw exception("equilibria: M=%u != N=%u + Nc=%u", unsigned(M), unsigned(N), unsigned(Nc) );
            
            //__________________________________________________________________
            //
            //
            // Special cases
            //
            //__________________________________________________________________
            if(Nc<=0)
            {
                //______________________________________________________________
                //
                // Special case
                //______________________________________________________________
                std::cerr << "Not Handled: no constraints" << std::endl;
                exit(1);
                
            }
            
            
            //__________________________________________________________________
            //
            //
            // Generic case
            //
            //__________________________________________________________________
            imatrix_t P(Nc,M);
            vector_t  Lam(Nc,0);
            
            //__________________________________________________________________
            //
            // create data from boot loader
            //__________________________________________________________________
            for(size_t i=Nc;i>0;--i)
            {
                const constraint &c = *loader[i];
                Lam[i] = c.level;
                for(size_t j=c.count();j>0;--j)
                {
                    const constraint::member &m = c[j];
                    P[i][m.sp->indx] = m.weight;
                }
            }
            
            //__________________________________________________________________
            //
            // factorize data: get constant and check structure
            //__________________________________________________________________
            fixedJ.free();
            fixedC.free();
            rewrite_constraints(P,Lam,fixedJ,fixedC,active);
            std::cerr << "Lam=" << Lam << std::endl;
            std::cerr << "P="   << P   << std::endl;
            tao::set(aboot,active);
            for(size_t j=fixedJ.size();j>0;--j)
            {
                aboot[ fixedJ[j] ] = false;
            }
            
            
            
            std::cerr << "active=" << active << std::endl;
            std::cerr << "localA=" << aboot  << std::endl;
            
            {
                imatrix_t P2(Nc,Nc);
                tao::mmul_rtrn(P2, P, P);
                //std::cerr << "P2=" << P2 << std::endl;
                Delta = determinant_of(P2);
                //std::cerr << "Delta=" << Delta << std::endl;
                if(Delta==0)
                    throw exception("unexpected singular set of constraints");
                imatrix_t J(Nc,Nc);
                adjoint(J, P2);
                //std::cerr << "J=" << J << std::endl;
                imatrix_t Y(M,Nc);
                tao::mmul_ltrn(Y, P, J);
                tao::mul(Xstar,Y,Lam);
            }
            //std::cerr << "Y=" << Y << std::endl;
            //std::cerr << "Xstar=" << Xstar << std::endl;
            
            //__________________________________________________________________
            //
            // compute orthogonal space
            //__________________________________________________________________
            compute_Qv2(Q, P, Nu);
            //std::cerr << "Q=" << Q << std::endl;
            //std::cerr << "Nu="  << Nu << std::endl;
            
            //__________________________________________________________________
            //
            //
            //
            // Algorithm
            //
            //
            //__________________________________________________________________
            computeK(t);
            updateScaling();
            matrix_t A(N,N);
            vector_t V(N,0);
            
            //__________________________________________________________________
            //
            // Starting point
            //__________________________________________________________________
            size_t count = 0;
            size_t cycle = 0;
            
        GENERATE_C: ++cycle;
            for(size_t j=N;j>0;--j)
            {
                xi[j] = ran() - 0.5;
            }
            optimize();
            //std::cerr << "xi=" << xi << std::endl;
            //std::cerr << "C="  << C << std::endl;
            updateGammaAndPhi();
            
        LOOP: ++count;
            //__________________________________________________________________
            //
            // We have a would be solution:
            // Newton for 0 =  Gamma + Phi*Q'*U
            // compute Gamma and Modified Phi
            //__________________________________________________________________
            tao::set(Cs,C);   //-- save C
            tao::set(xis,xi); //-- save xi
            const double G0 = scaledGamma();
            for(size_t j=fixedJ.size();j>0;--j)
            {
                const size_t jj = fixedJ[j];
                for(size_t i=N;i>0;--i)
                {
                    Phi[i][jj] = 0;
                }
            }
            std::cerr << "\tC=" << C << std::endl;
            
            //__________________________________________________________________
            //
            // W = Phi * Q'
            //__________________________________________________________________
            tao::mmul_rtrn(W, Phi, Q);
            A.assign(W);
            if( !crout<double>::build(W) )
            {
                std::cerr << "equilibria.booting: singular composition..." << std::endl;
                goto GENERATE_C;
            }
            
            //__________________________________________________________________
            //
            // get the best U = - inv(Phi*Q') * Gamma
            //__________________________________________________________________
            tao::neg(U,Gamma);
            tao::set(V,U);
            crout<double>::solve(W,U);
            crout<double>::improve(U, A, W, V);
            
            //__________________________________________________________________
            //
            // deduce the new xi = xis+U, optimized
            //__________________________________________________________________
            tao::add(xi,U);
            optimize();
            
            
            //__________________________________________________________________
            //
            // where are we now ?
            //__________________________________________________________________
            updateGammaAndPhi();
            const double G1 = scaledGamma();
            
            if(G1>=G0)
            {
                //std::cerr << "-- equilibria.booting: need to backtrack" << std::endl;
                // TODO: do I need to contract it before ?
                triplet<double> XX = { 0,   1,  1};
                triplet<double> FF = { G0, G1, G1};
                bracket<double>::expand(optF, XX, FF);
                minimize<double>(optF, XX, FF, 0);
                (void) computeF(XX.b);
            }
            
            //__________________________________________________________________
            //
            // Test convergence
            //__________________________________________________________________
            bool converged = true;
            for(size_t j=M;j>0;--j)
            {
                const double cc = C[j];
                const double dd =  fabs( (dC[j] = cc - Cs[j]) );
                if( dd > numeric<double>::ftol * fabs(cc) )
                {
                    converged = false;
                }
            }
            
            if(converged)
            {
                //std::cerr << "#variables have converged" << std::endl;
                goto HAS_CONVERGED;
            }
            goto LOOP;
            
            
        HAS_CONVERGED:
            //__________________________________________________________________
            //
            // compute error
            //__________________________________________________________________
            updateGammaAndPhi();
            tao::mmul_rtrn(W, Phi, Q);
            if( !crout<double>::build(W) )
            {
                std::cerr << "-- equilibria.booting: invalid final composition..." << std::endl;
                goto GENERATE_C;
            }
            tao::neg(xi,Gamma);
            crout<double>::solve(W,xi);
            tao::mul_trn(dC,W,xi);
            
            //__________________________________________________________________
            //
            // compute tolerance
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                if(aboot[j])
                {
                    double dCj = fabs(dC[j]);
                    if(dCj>0)
                    {
                        dCj = log_round_ceil(dCj);
                    }
                    dC[j] = dCj;
                }
                else
                {
                    dC[j] = 0;
                }
            }
            for(size_t j=M;j>0;--j)
            {
                if(aboot[j])
                {
                    if( C[j] <= dC[j] ) C[j] = 0;
                }
            }
            
            //__________________________________________________________________
            //
            // then send it to chemistry
            //__________________________________________________________________
            if(!balance(C))
            {
                std::cerr << "-- unable to balance..." << std::endl;
                goto GENERATE_C;
            }
            
            if(!normalize(C,-1, false))
            {
                std::cerr << "-- unable to normalize a balance,booted solution!!!" << std::endl;
                goto GENERATE_C;
            }
            
            //__________________________________________________________________
            //
            // then send it to the user
            //__________________________________________________________________
            copy_to(C0);
        }
        
        
        
        
        
    }
    
}
