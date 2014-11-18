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
#include "yocto/exception.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        
        equilibrium::equilibrium(const string &id ):
        name(id),
        K(this,&equilibrium::callK),
        forward(false,false,-1),
        reverse(false,false,-1),
        blocked(false),
        indx(0),
        reac(),
        prod(),
        nu_p(0)
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
            int sum_nu_p = 0;
            for(const actor *a=prod.head;a;a=a->next)
            {
                sum_nu_p += a->nu;
            }
            (int&)nu_p = sum_nu_p;
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
            double r_prod = Kt;
            double p_prod = 1;
            const size_t M = Phi.size();
            for(size_t i=M;i>0;--i)
            {
                Phi[i] = 0.0;
            }
            
            for(const actor *a=reac.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(i<=Phi.size());
                assert(a->nu<0);
                const size_t q = (-a->nu);
                const double Ci = C[i];
                r_prod *= ipower(Ci,q);
                
                double phi = q*ipower(Ci,q-1);
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
                const size_t q  = a->nu;
                const double Ci = C[i];
                p_prod *= ipower(Ci,q);
                
                double phi = q*ipower(Ci,q-1);
                for(const actor *b=prod.head;b;b=b->next)
                {
                    if(a!=b)
                    {
                        const size_t j = b->sp->indx;
                        phi *= ipower(C[j],(b->nu));
                    }
                }
                Phi[i] -= phi;
            }
            
            const double g = r_prod - p_prod;
            return g;
        }
        
        
        static inline
        void __update( equilibrium::xi_ctrl &ctrl, const double Ci, const int nu)
        {
            assert(nu>0);
            if(Ci<=0)
            {
                ctrl.blocked = true;
                ctrl.limited = true;
                ctrl.maximum = 0;
            }
            else
            {
                if(!ctrl.blocked)
                {
                    const double xi = Ci/nu;
                    if(ctrl.limited)
                    {
                        assert(ctrl.maximum>=0);
                        if(xi<ctrl.maximum)
                        {
                            ctrl.maximum = xi;
                        }
                    }
                    else
                    {
                        ctrl.maximum = xi;
                        ctrl.limited = true;
                    }
                }
            }
        }
        
        void equilibrium:: compute_limits( const array<double> &C )  throw()
        {
            forward.blocked = reverse.blocked = false;
            forward.limited = reverse.limited = false;
            forward.maximum = reverse.maximum = -1;
            
            // from reactant, max fwd
            for(const actor *a=reac.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(a->nu<0);
                __update(forward, C[i], -a->nu);
            }
            
            
            // from products, max rev
            for(const actor *a=prod.head;a;a=a->next)
            {
                const size_t i = a->sp->indx;
                assert(i>=1);
                assert(i<=C.size());
                assert(a->nu>0);
                __update(reverse,C[i],a->nu);
            }
            
            blocked = forward.blocked && reverse.blocked;
        }
        
        void equilibrium:: validate() const
        {
            //-- check for zero charge
            int z = 0;
            for(const actor *a=reac.head;a;a=a->next)
            {
                z += a->nu * a->sp->z;
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                z += a->nu * a->sp->z;
            }
            
            if(z)
            {
                throw exception("%s is creating a net charge of %d",name.c_str(),z);
            }
        }
        
        
        static inline
        void __show_limits( const equilibrium::xi_ctrl &ctrl, std::ostream &os )
        {
            if(ctrl.blocked)
            {
                os << "BLOCKED";
            }
            else
            {
                if(ctrl.limited)
                {
                    os << ctrl.maximum;
                }
                else
                {
                    os << "unlimited";
                }
            }
        }
        
        void equilibrium:: show_limits( std::ostream &os ) const
        {
            os << "limits(" << name << ")" << std::endl;
            os << "\tforward: "; __show_limits(forward, os); os << std::endl;
            os << "\treverse: "; __show_limits(reverse, os); os << std::endl;
        }
        
        
        double equilibrium:: apply_limits( const double xi ) const throw()
        {
            if(blocked)
            {
                return 0;
            }
            else
            {
                if(xi>=0)
                {
                    //______________________________________________________________
                    //
                    // Reaction wants to go forward
                    //______________________________________________________________
                    if(forward.blocked)
                    {
                        return 0;
                    }
                    else
                    {
                        if(forward.limited)
                        {
                            
                            return min_of(forward.maximum,xi);
                        }
                        else
                        {
                            return xi;
                        }
                    }
                    
                }
                else
                {
                    //______________________________________________________________
                    //
                    // Reaction wants to go reverse
                    //______________________________________________________________
                    if(reverse.blocked)
                    {
                        return 0;
                    }
                    else
                    {
                        if(reverse.limited)
                        {
                            return -min_of(reverse.maximum,-xi);
                        }
                        else
                        {
                            return xi;
                        }
                    }
                }
            }            
        }
        
        
        bool equilibrium:: involves(const size_t indx) const throw()
        {
            for(const actor *a=reac.head;a;a=a->next)
            {
                if(a->sp->indx==indx)
                {
                    return true;
                }
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                if(a->sp->indx==indx)
                {
                    return true;
                }
            }
            
            return false;
        }
        
        static inline void __involve( vector<size_t> &a, const size_t idx )
        {
            for(size_t i=a.size();i>0;--i)
            {
                if(idx==a[i])
                    return;
            }
            a.push_back(idx);
        }
        
        void equilibrium:: collect( vector<size_t> &involved ) const
        {
            for(const actor *a=reac.head;a;a=a->next)
            {
                __involve(involved,a->sp->indx);
            }
            
            for(const actor *a=prod.head;a;a=a->next)
            {
                __involve(involved,a->sp->indx);
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

