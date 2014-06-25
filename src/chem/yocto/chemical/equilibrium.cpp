#include "yocto/chemical/equilibrium.hpp"
#include "yocto/exception.hpp"
#include "yocto/code/bswap.hpp"

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

                
        void equilibrium:: fill( array<double> &Nu, array<ptrdiff_t> &NuR, array<ptrdiff_t> &NuP) const throw()
        {
            assert(Nu.size()==NuR.size());
            assert(Nu.size()==NuP.size());
            
            for(size_t i=actors.size();i>0;--i)
            {
                const actor &A = actors[i];
                const size_t j = A.sp->indx; assert(j>0);assert(j<=Nu.size());
                const int    nu =  A.nu; assert(nu!=0);
                Nu[j] = nu;
                if(nu>0)
                {
                    NuP[j] = nu;
                }
                else
                {
                    NuR[i] = -nu;
                }
            }
            
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
