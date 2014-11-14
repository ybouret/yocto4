#include "yocto/chemical/equilibrium.hpp"

namespace yocto
{
    namespace chemical
    {
        
        equilibrium:: actor:: actor(const species::pointer  &a_sp, const int a_nu) throw() :
        sp(a_sp),
        nu(a_nu)
        {
        }
        
        equilibrium:: actor:: actor(const actor &a) throw() :
        sp(a.sp),
        nu(a.nu)
        {
        }
        
        equilibrium:: actor:: ~actor() throw() {}
        
        int equilibrium:: actor:: compare(const actor &lhs, const actor &rhs) throw()
        {
            return string::compare(lhs.sp->name,rhs.sp->name);
        }
        
    }
    
}

#include "yocto/sort/quick.hpp"
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
        
        
        void equilibrium:: add( const species::pointer &sp, const int nu)
        {
            if(nu!=0)
            {
                const size_t na = actors.size();
                for(size_t i=1;i<=na;++i)
                {
                    actor &a = actors[i];
                    if(sp->name==a.sp->name)
                    {
                        (int&)(a.nu) += nu;
                        if(0==a.nu)
                        {
                            bswap(a,actors[na]);
                            actors.pop_back();
                        }
                        return;
                    }
                }
                const actor a(sp,nu);
                actors.push_back(a);
                quicksort(actors,actor::compare);
            }
        }
        
        void equilibrium:: output( std::ostream &os ) const
        {
            // first pass: reactant, nu < 0
            os << name << " : ";
            const size_t na = actors.size();
            size_t nr = 0;
            size_t np = 0;
            for(size_t i=1;i<=na;++i)
            {
                const actor &a = actors[i];
                if(a.nu<0) ++nr;
                if(a.nu>0) ++np;
            }
            
            for(size_t i=1,j=0;i<=na;++i)
            {
                const actor &a = actors[i];
                if(a.nu<0)
                {
                    if(-a.nu!=1)
                    {
                        os <<  -a.nu << "*";
                    }
                    os << a.sp->name;
                    if(++j<nr) os << " + ";
                }
            }
            os << " <=> ";
            for(size_t i=1,j=0;i<=na;++i)
            {
                const actor &a = actors[i];
                if(a.nu>0)
                {
                    if(a.nu!=1)
                    {
                        os << a.nu << "*";
                    }
                    os << a.sp->name;
                    if(++j<np) os << " + ";
                }
            }
            
            os << " | K=" << callK(0);
            
        }
        
        
    }
    
}

namespace yocto
{
    namespace chemical
    {
        ////////////////////////////////////////////////////////////////////////
        
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

