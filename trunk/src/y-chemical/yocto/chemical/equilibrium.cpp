#include "yocto/chemical/equilibrium.hpp"
#include "yocto/code/hsort.hpp"

#include <iostream>


namespace yocto
{
    namespace chemical
    {
        ////////////////////////////////////////////////////////////////////////
        equilibrium:: equilibrium( const string &id ) :
        name(id),
        data()
        {
            
        }
        
        equilibrium:: equilibrium(const char *id ) :
        name(id),
        data()
        {
            
        }
        
        
        equilibrium:: ~equilibrium() throw() {}
        
        
        static inline
        int __compare_actors( const equilibrium::actor &lhs, const equilibrium::actor &rhs ) throw()
        {
            return lhs.coef - rhs.coef;
        }
        
        void equilibrium:: add( const species::ptr &spec, const int coef )
        {
            if(coef!=0)
            {
                const actor a(spec,coef);
                actors.push_back(a);
                hsort(actors,__compare_actors);
            }
        }
        
        size_t equilibrium:: count() const throw()
        {
            return actors.size();
        }
        
        std::ostream & operator<<( std::ostream &os, const equilibrium &eq)
        {
            const array<equilibrium::actor> &actors = eq.actors;
            const size_t na = actors.size();
            size_t nr = 0;
            for(size_t i=1; i <= na; ++i )
            {
                if(actors[i].coef>0) break;
                ++nr;
            }
            
            os << eq.name << ": ";
            
            for(size_t i=1; i <= nr; ++i )
            {
                if(i>1) os << " + ";
                os << "(" << -actors[i].coef << ")*" << actors[i].spec->name;
            }
            
            os << " <=> ";
            const size_t ns = nr+1;
            for(size_t i=ns;i<=actors.size();++i)
            {
                if(i>ns) os << " + ";
                os << "(" << actors[i].coef << ")*" << actors[i].spec->name;
            }
            
            os << " | " << eq.K(0, standard_pressure, standard_temperature);
            return os;
        }
        
        ////////////////////////////////////////////////////////////////////////
        equilibrium:: actor:: actor( const species::ptr &sp, const int stochio ) throw() :
        spec(sp),
        coef(stochio)
        {
            assert(coef!=0);
        }
        
        
        equilibrium:: actor:: actor( const actor &other ) throw() :
        spec( other.spec ),
        coef( other.coef )
        {
            
        }
        
        equilibrium:: actor:: ~actor() throw() {}
        
        
        ////////////////////////////////////////////////////////////////////////
        constant_equilibrium:: constant_equilibrium( const string &id, const double Keq ) :
        equilibrium(id)
        {
            data.make<double>(Keq);
        }
        
        
        
        constant_equilibrium:: constant_equilibrium( const char *id, const double Keq ) :
        equilibrium(id)
        {
            data.make<double>(Keq);
        }
        
        constant_equilibrium:: ~constant_equilibrium() throw()
        {
        }
        
        double constant_equilibrium::K(double t, double, double) const throw()
        {
            return data.as<double>();
        }
        
        
    }
}
