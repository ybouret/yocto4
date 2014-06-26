#include "yocto/chemical/equilibrium.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/sort/quick.hpp"

namespace yocto
{
    namespace chemical
    {
        
        //______________________________________________________________________
        //
        // actor
        //______________________________________________________________________
        equilibrium::actor:: actor( const species::pointer &spec, const int coef ) throw() :
        sp( spec ),
        nu( coef )
        {
        }
        
        equilibrium:: actor:: ~actor() throw()
        {
        }
        
        equilibrium::actor:: actor( const actor &other ) throw() :
        sp( other.sp ),
        nu( other.nu )
        {
        }
        
        int equilibrium::actor:: compare(const actor &lhs, const actor &rhs) throw()
        {
            return string::compare(lhs.sp->name, rhs.sp->name);
        }
        
        //______________________________________________________________________
        //
        // instr
        //______________________________________________________________________
        equilibrium::instr:: instr( const size_t indx, const size_t coef) throw() :
        i(indx),
        p(coef)
        {
            assert(i>0);
            assert(p>0);
        }
        
        equilibrium::instr:: ~instr() throw() {}
        
        equilibrium:: instr:: instr( const instr &other ) throw() :
        i( other.i ),
        p( other.p )
        {
        }
        
        int equilibrium::instr:: compare( const instr &lhs, const instr &rhs) throw()
        {
            return __compare<size_t>(lhs.i,rhs.i);
        }
        
        
        //______________________________________________________________________
        //
        // d_instr
        //______________________________________________________________________
        equilibrium:: d_instr:: ~d_instr() throw() {}
        
        equilibrium:: d_instr:: d_instr( const size_t i,const size_t nu,const size_t nmax ) :
        indx(i),
        coef(nu),
        code(nmax,as_capacity)
        {
            assert(indx>0);
            assert(coef>0);
        }
        
        
        //______________________________________________________________________
        //
        // equilbrium
        //______________________________________________________________________
        equilibrium:: ~equilibrium() throw()
        {
        }
        
        equilibrium:: equilibrium(const string &id ) :
        name(id),
        data(),
        K(this, & equilibrium::computeK),
        actors(4,as_capacity),
        r_code(),
        p_code(),
        DeltaNu(0)
        {
            
        }
        
        const string & equilibrium :: key() const throw() { return name; }
        
        
        bool equilibrium:: is_valid() const throw()
        {
            // charge conservation
            int Q = 0;
            for(size_t i=actors.size();i>0;--i)
            {
                const actor &A = actors[i];
                Q += A.nu * A.sp->z;
            }
            if( Q!=0 )
                return false;
            
            return true;
        }
        
        void equilibrium:: output(std::ostream &os) const
        {
            os << name << " : ";
            // first pass: reactants
            const size_t n = actors.size();
            size_t       r = 0;
            for(size_t i=1;i<=n;++i)
            {
                const actor &A = actors[i];
                if(A.nu<0)
                {
                    if(r++>0) { os << " + "; }
                    if(abs(A.nu)>1)
                    {
                        os << A.nu << '*';
                    }
                    os << A.sp->name;
                }
            }
            
            os << " <=> ";
            // second pass: products
            size_t       p = 0;
            for(size_t i=1;i<=n;++i)
            {
                const actor &A = actors[i];
                if(A.nu>0)
                {
                    if(p++>0) { os << " + "; }
                    if(abs(A.nu)>1)
                    {
                        os << A.nu << '*';
                    }
                    os << A.sp->name;
                }
            }
            
            os << " | {" << getK(0) << "}";
        }
        
        void equilibrium:: add( const species::pointer &spec, const int coef )
        {
            if(coef!=0)
            {
                // look up
                const size_t n = actors.size();
                for(size_t i=n;i>0;--i)
                {
                    actor &A = actors[i];
                    if(A.sp->name==spec->name)
                    {
                        const int sum = A.nu + coef;
                        if(sum!=0)
                        {
                            (int &)(A.nu) = sum;
                        }
                        else
                        {
                            bswap(A, actors[n]);
                            assert(actors.back().sp->name == spec->name);
                            actors.pop_back();
                        }
                        update_delta();
                        return;
                    }
                }
                
                // new actor
                const actor B(spec,coef);
                actors.push_back(B);
                update_delta();
            }
        }
        
        void equilibrium:: update_delta() throw()
        {
            int d = 0;
            for(size_t i=actors.size();i>0;--i)
            {
                d += actors[i].nu;
            }
            (int &)DeltaNu = d;
        }
        
        double equilibrium::computeK(double t) const
        {
            return getK(t);
        }
        
        
        void equilibrium:: compile( array<double> &Nu, const collection &lib)
        {
            //__________________________________________________________________
            //
            // first pass: fill Nu and count reactants/products
            //__________________________________________________________________
            size_t nr = 0, np = 0;
            for(size_t i=actors.size();i>0;--i)
            {
                const actor &A  = actors[i];
                const size_t j  = A.sp->indx; assert(j>0);assert(j<=Nu.size());
                const int    nu = A.nu; assert(nu!=0);
                Nu[j] = nu;
                if(nu>0)
                {
                    ++np; //NuP[j] = nu;
                }
                else
                {
                    ++nr; //NuR[i] = -nu;
                }
            }
            
            //__________________________________________________________________
            //
            // allocate resources
            //__________________________________________________________________
            pj_code.release();
            rj_code.release();
            p_code.release();
            r_code.release();
            
            r_code.ensure(nr);
            p_code.ensure(np);
            
            //__________________________________________________________________
            //
            // second pass: compile codes
            //__________________________________________________________________
            for(size_t i=actors.size();i>0;--i)
            {
                const actor &A  = actors[i];
                const size_t j  = A.sp->indx; assert(j>0);assert(j<=Nu.size());
                const int    nu = A.nu; assert(nu!=0);
                if(nu>0)
                {
                    const instr I(j,nu);
                    p_code.push_back(I);
                }
                else
                {
                    const instr I(j,-nu);
                    r_code.push_back(I);
                }
            }
            
            quicksort(p_code,instr::compare);
            quicksort(r_code,instr::compare);
            std::cerr << "<code name='" << name << "'>" << std::endl;
            std::cerr << "r_code=" << r_code << std::endl;
            std::cerr << "p_code=" << p_code << std::endl;
            
            //__________________________________________________________________
            //
            // third pass: jacobian codes
            //__________________________________________________________________
            precompute(rj_code, r_code);
            precompute(pj_code, p_code);
            std::cerr << "</code>" << std::endl;
            
        }
        
        void equilibrium:: precompute( vector<j_instr> &jcode, const vector<instr> &code)
        {
            std::cerr << "Creating Jacobian Code from " << code << std::endl;
            const size_t nc = code.size();
            jcode.free();
            for(size_t i=1;i<=nc;++i)
            {
                const instr &cc = code[i];
                assert(cc.p>0);
                j_instr pJ( new d_instr(cc.i,cc.p,nc) );
                jcode.push_back(pJ);
                if(cc.p>1)
                {
                    const instr tmp(cc.i,cc.p-1);
                    pJ->code.push_back(tmp);
                }
                for(size_t j=1;j<=nc;++j)
                {
                    if(j!=i)
                        pJ->code.push_back(code[j]);
                }
                quicksort(pJ->code,instr::compare);
                std::cerr << "\tdGamma/dC" << cc.i << " = " << pJ->coef << " * " << pJ->code << std::endl;
            }
        }
        
        
        double equilibrium:: computeGamma(double t, const array<double> &C, double &KK)
        {
            KK = getK(t);
            return updateGamma(C,KK);
            
        }
        
        double equilibrium:: updateGamma(const array<double> &C, const double KK) const throw()
        {
            double lhs = KK;
            for(size_t i=r_code.size();i>0;--i)
            {
                const instr &I = r_code[i];
                lhs *= ipower(C[I.i],I.p);
            }
            
            double rhs = 1;
            for(size_t i=p_code.size();i>0;--i)
            {
                const instr &I = p_code[i];
                rhs *= ipower(C[I.i],I.p);
            }
            
            return lhs - rhs;
        }
        
        double equilibrium:: computeGammaAndPhi(array<double>       &Phi,
                                                double               t,
                                                const array<double> &C,
                                                double              &KK)
        {
            const double localK = (KK = getK(t));
            return updateGammaAndPhi(Phi,C,localK);
        }
        
        double equilibrium:: updateGammaAndPhi( array<double> &Phi,const array<double> &C, const double KK ) const throw()
        {
            for(size_t i=Phi.size();i>0;--i)
                Phi[i] = 0;
            
            // reactant jacobian
            for(size_t k=rj_code.size();k>0;--k)
            {
                const d_instr      &J    = *rj_code[k];
                double              prod = KK * J.coef;
                const array<instr> &code = J.code;
                
                assert(J.indx>0);
                assert(J.indx<=Phi.size());
                
                for(size_t j=code.size();j>0;--j)
                {
                    const instr &cc = code[j];
                    prod *= ipower( C[cc.i], cc.p );
                }
                
                Phi[J.indx] += prod;
            }
            
            // products jacobian
            for(size_t k=pj_code.size();k>0;--k)
            {
                const d_instr      &J    = *pj_code[k];
                double              prod =  J.coef;
                const array<instr> &code = J.code;
                
                assert(J.indx>0);
                assert(J.indx<=Phi.size());
                
                for(size_t j=code.size();j>0;--j)
                {
                    const instr &cc = code[j];
                    prod *= ipower( C[cc.i], cc.p );
                }
                
                Phi[J.indx] -= prod;
            }

            
            
            return updateGamma(C,KK);
        }
        
        
        
        
        //______________________________________________________________________
        //
        // const equilibrium
        //______________________________________________________________________
        const_equilibrium:: const_equilibrium( const string &id, const double K) :
        equilibrium(id)
        {
            data.make<double>(K);
        }
        
        const_equilibrium:: ~const_equilibrium() throw()
        {
        }
        
        double const_equilibrium:: getK( double ) const
        {
            return data.as<double>();
        }
        
        
        
    }
}
