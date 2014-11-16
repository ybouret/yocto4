#include "yocto/chemical/equilibrium.hpp"

namespace yocto
{
    namespace chemical
    {
        
        equilibrium:: actor:: actor(const species::pointer  &a_sp, const int a_nu) throw() :
        sp(a_sp),
        nu(a_nu),
        prev(0),
        next(0)
        {
        }
        
       
        
        equilibrium:: actor:: ~actor() throw() {}
        
        
    }
    
}

#include "yocto/sort/merge.hpp"
#include "yocto/code/ipower.hpp"

namespace yocto
{
    namespace chemical
    {
        
        equilibrium::equilibrium(const string &id ):
        name(id),
        K(this,&equilibrium::callK),
        xi_rev(0),
        xi_fwd(0)
        {
        }
        
        double equilibrium::callK(double t) const { return getK(t); }
        
        
        
        equilibrium:: ~equilibrium() throw() {}
        
        
        static inline int __compare_actors( const equilibrium::actor *lhs, const equilibrium::actor *rhs, void * ) throw()
        {
            return string::compare(lhs->sp->name,rhs->sp->name);
        }
        
        void equilibrium:: add( const species::pointer &sp, const int nu)
        {
            if(nu!=0)
            {
                // check among reactants
                for(actor *a = reac.head;a;a=a->next)
                {
                    if(a->sp->name == sp->name )
                    {
                        assert(a->nu<0);
                        (int&)(a->nu) += nu;
                        if(a->nu>0)
                        {
                            prod.push_back( reac.unlink(a) );
                            core::merging<actor>::sort(prod, __compare_actors,0);
                            return;
                        }
                        else
                        {
                            if(a->nu==0)
                            {
                                delete reac.unlink(a);
                                return;
                            }
                            else
                            {
                                assert(a->nu<0);
                                return;
                            }
                        }
                    }
                }
                
                //check among products
                for(actor *a=prod.head;a;a=a->next)
                {
                    if(a->sp->name==sp->name)
                    {
                        assert(a->nu>0);
                        (int&)(a->nu) += nu;
                        if(a->nu<0)
                        {
                            reac.push_back( prod.unlink(a) );
                            core::merging<actor>::sort(reac,__compare_actors,0);
                            return;
                        }
                        else
                        {
                            if(a->nu==0)
                            {
                                delete prod.unlink(a);
                                return;
                            }
                            else
                            {
                                assert(a->nu>0);
                                return;
                            }
                        }
                    }
                }
                
                // a new one
                actors *L = (nu<0) ? &reac : &prod;
                L->push_back(new actor(sp,nu));
                core::merging<actor>::sort(*L,__compare_actors,0);
            }
        }
        
        void equilibrium:: output( std::ostream &os ) const
        {
            // first pass: reactant, nu < 0
            os << name << " : ";
            for(const actor *a=reac.head;a;a=a->next)
            {
                assert(a->nu<0);
                if(a->nu!=-1)
                {
                    os << -a->nu << '*';
                }
                os << a->sp->name;
                if(a!=reac.tail)
                {
                    os << " + ";
                }
            }
            os << " <=> ";
            for(const actor *a=prod.head;a;a=a->next)
            {
                assert(a->nu>0);
                if(a->nu!=1)
                {
                    os << a->nu << '*';
                }
                os << a->sp->name;
                if(a!=prod.tail)
                {
                    os << " + ";
                }
            }

            os << " | K=" << callK(0);
            
        }
        
        
        void equilibrium:: initialize( array<ptrdiff_t> &Nu, array<bool> &active) const throw()
        {
            assert(Nu.size()==active.size());
            
            for(const actor *a=reac.head;a;a=a->next)
            {
                const size_t j = a->sp->indx;
                assert(j>=1);
                assert(j<=Nu.size());
                Nu[j]     = a->nu;
                active[j] = true;
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                const size_t j = a->sp->indx;
                assert(j>=1);
                assert(j<=Nu.size());
                Nu[j]     = a->nu;
                active[j] = true;
            }
        }

        
        double equilibrium:: computeGamma( double t, const array<double> &C, double &Kt ) const
        {
            return updateGamma(C, (Kt=callK(t)) );
        }

        
        double equilibrium:: updateGamma( const array<double> &C, const double Kt ) const
        {
            double p_prod = 1;
            double r_prod = Kt;
            
            for(const actor *a=reac.head;a;a=a->next)
            {
                assert(a->sp->indx>=1);
                assert(a->sp->indx<=C.size());
                assert(a->nu<0);
                r_prod *= ipower(C[a->sp->indx],(-a->nu));
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                assert(a->sp->indx>=1);
                assert(a->sp->indx<=C.size());
                assert(a->nu>0);
                p_prod *= ipower(C[a->sp->indx],a->nu);
            }
            
            
            return r_prod - p_prod;
        }

        double equilibrium:: computeGammaAndPhi( double t, const array<double> &C, double &Kt, array<double> &Phi ) const
        {
            return updateGammaAndPhi(C, (Kt=callK(t)), Phi);
        }
        
        double equilibrium:: updateGammaAndPhi(const array<double> &C, const double Kt, array<double> &Phi) const
        {
            assert(C.size()>=Phi.size());
            double p_prod = 1;
            double r_prod = Kt;
            const size_t M = Phi.size();
            for(size_t i=M;i>0;--i) Phi[i] = 0.0;
            
            for(const actor *a=reac.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(i<=Phi.size());
                assert(a->nu<0);
                const size_t q = (-a->nu);
                r_prod *= ipower(C[i],q);
                
                double phi = q*ipower(C[i],q-1);
                for(const actor *b=reac.head;b;b=b->next)
                {
                    if(a!=b)
                    {
                        const size_t j = b->sp->indx;
                        phi *= ipower(C[j],(-b->nu));
                    }
                }
                Phi[i] += Kt * phi;
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(a->nu>0);
                const size_t q = a->nu;
                p_prod *= ipower(C[i],q);
                
                double phi = q*ipower(C[i],q-1);
                for(const actor *b=prod.head;b;b=b->next)
                {
                    if(a!=b)
                    {
                        const size_t j = b->sp->indx;
                        phi *= ipower(C[j],(-b->nu));
                    }
                }
                Phi[i] -= phi;
            }
            
            
            return r_prod - p_prod;

        }
        
        
        void equilibrium:: compute_limits( const array<double> &C )  throw()
        {
            xi_rev = xi_fwd = 0;
            
            // from reactant, max fwd
            for(const actor *a=reac.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(a->nu<0);
                const double Ci = C[i];
                if(Ci>0)
                {
                    const double xi = Ci/(-a->nu);
                    if(xi_fwd<=0)
                    {
                        xi_fwd = xi;
                    }
                    else
                    {
                        if(xi<xi_fwd) xi_fwd = xi;
                    }
                }
            }
            
            // from products, max rev
            for(const actor *a=prod.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(a->nu>0);
                const double Ci = C[i];
                if(Ci>0)
                {
                    const double xi = Ci/(a->nu);
                    if(xi_rev<=0)
                    {
                        xi_rev = xi;
                    }
                    else
                    {
                        if(xi<xi_rev) xi_rev = xi;
                    }
                }
            }
            
        }


    }
    
}

namespace yocto
{
    namespace chemical
    {
        
        const_equilibrium:: const_equilibrium(const string &id, const double Kvalue) :
        equilibrium(id),
        value(Kvalue)
        {
        }
        
        const_equilibrium:: ~const_equilibrium() throw()
        {
        }
        
        double const_equilibrium:: getK(double) const { return value; }
        
        
    }
    
}

