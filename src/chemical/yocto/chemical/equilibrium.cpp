#include "yocto/chemical/equilibrium.hpp"
#include "yocto/sort/heap.hpp"

#include <iostream>


namespace yocto
{
    namespace chemical
    {
        ////////////////////////////////////////////////////////////////////////
        const string & equilibrium:: key() const throw()
        {
            return name;
        }
        
#if defined(_MSC_VER)
		// using this in ctor
#pragma warning ( disable : 4355 )
#endif
        equilibrium:: equilibrium( const string &id ) :
        name(id),
        data(),
		actors(),
        Nu(0),
        NuR(0),
        NuP(0),
        C0(0),
        K( this, & equilibrium::computeK)
        {
            
        }
        
        equilibrium:: equilibrium(const char *id ) :
        name(id),
        data(),
        actors(),
        Nu(0),
        NuR(0),
        NuP(0),
        C0(0),
        K( this, & equilibrium::computeK)
        {
            
        }
        
        
        double equilibrium:: computeK( double t ) const
        {
            return getK(t);
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
                ((int&) Nu) += coef;
                if(coef>0)
                    ((unsigned&)NuP) += unsigned(coef);
                else
                    ((unsigned&)NuR) += unsigned(-coef);
            }
        }
        
        size_t equilibrium:: count() const throw()
        {
            return actors.size();
        }
        
        void equilibrium:: fill(array<ptrdiff_t> &nuR,
                                array<ptrdiff_t> &nuP) const throw()
        {
            assert(nuR.size()==nuP.size());
            
            for( size_t i=actors.size();i>0;--i)
            {
                const actor   &a    =  actors[i];
                const species &spec = *a.spec;
                const int      coef =  a.coef;
                assert(spec.indx>=1);
                assert(spec.indx<=nuR.size());
                const size_t    j   = spec.indx;
                if(coef>=0)
                    nuP[j] += coef;
                else
                    nuR[j] -= coef;
            }
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
                os << "(" << -actors[i].coef << ")*{" << actors[i].spec->name << "}";
            }
            
            os << " <=> ";
            const size_t ns = nr+1;
            for(size_t i=ns;i<=actors.size();++i)
            {
                if(i>ns) os << " + ";
                os << "(" << actors[i].coef << ")*{" << actors[i].spec->name << "}";
            }
            
            os << " | K= " << eq.K(0);
            os << " / Nu= " << eq.Nu << ", NuP= " << eq.NuP << ", NuR= " << eq.NuR;
            return os;
        }
        
        void equilibrium:: scale(double t) const
        {
            C0 = 1;
            const double K0 = K(t);
            if(Nu!=0 && K0>0)
            {
                C0=pow(K0,1.0/Nu);
            }
        }
        
        void equilibrium:: append( array<double> &C, urand32 &ran ) const throw()
        {
            for( size_t i=actors.size();i>0;--i)
            {
                const species &sp = * actors[i].spec;
                assert(sp.indx>0);
                assert(sp.indx<=C.size());
                C[sp.indx] += C0 * ran.get<double>();
            }
            
        }
        
        bool equilibrium:: is_valid() const throw()
        {
            // does it mean something
            if( actors.size() <= 0 )
                return false;
            
            // charge conservation
            int DeltaNuZ = 0;
            for( size_t i=actors.size();i>0;--i)
            {
                const actor &a = actors[i];
                const int    nu = a.coef;
                const int    z  = a.spec->z;
                DeltaNuZ += z*nu;
            }
            
            if(DeltaNuZ != 0 )
                return false;
            
            return true;
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
        
        double constant_equilibrium::getK(double t) const
        {
            return data.as<double>();
        }
        
        
    }
}
