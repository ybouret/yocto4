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
        K(this,&equilibrium::callK)
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
        
        
        double equilibrium:: computeGamma( double t, const array<double> &C, double &Kt ) const
        {
            Kt = callK(t);
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

