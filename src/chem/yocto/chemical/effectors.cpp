#include "yocto/chemical/effectors.hpp"

namespace yocto
{
    namespace chemical
    {
        
        effector:: ~effector() throw()
        {
        }
        
        effector:: effector(const string &id) :
        name(id),
        pace(1.0)
        {
        }
        
        const string & effector:: key() const throw() { return name; }
        
        void effector:: update( array<double> &rho, const variables &params) const throw()
        {
            assert(rho.size()>=params.lib.size());
            for(size_t j=params.lib.size();j>0;--j)
            {
                rho[j] *= pace;
            }
        }

        std::ostream & operator<<( std::ostream &os, const effector &eff)
        {
            os << " (*) " << eff.name << ", pace=" << eff.pace;
            return os;
        }
        
    }
    
}

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        effectors:: ~effectors() throw()
        {
            
        }
        
        
        effectors:: effectors() throw() : _rho()
        {
            
        }
        
        void effectors::rate(array<double>       &rho,
                             const double         t,
                             const array<double> &Cin,
                             const array<double> &Cout,
                             const variables     &params)
        {
            const size_t M = params.lib.size(); assert(rho.size()>=M);
            _rho.make(M,0.0);
            
            for(size_t j=M;j>0;--j)
            {
                rho[j] = 0;
            }
            
            for( iterator i = begin(); i != end(); ++i )
            {
                effector &eff = **i;
                for(size_t j=M;j>0;--j) _rho[j] = 0;

                eff.rate(_rho,t,Cin,Cout,params);
                for(size_t j=M;j>0;--j)
                {
                    _rho[j] *= eff.pace;
                    rho[j]  +=  _rho[j];
                }
                //std::cerr << "rho_" << eff.name << "=" << _rho << std::endl;
            }
        }
        
        effector & effectors:: operator[](const string &name)
        {
            effector::pointer *pp = search(name);
            if(!pp)
                throw exception("no effector '%s'", name.c_str());
            return **pp;
        }
        
        effector & effectors:: operator[](const char *name)
        {
            const string NAME(name);
            return (*this)[NAME];
        }

        void effectors:: rescale_pace( const double speedup ) throw()
        {
            for( iterator i = begin(); i != end(); ++i )
            {
                (**i).pace *= speedup;
            }
        }

        std::ostream & operator<<( std::ostream &os, const effectors &eff)
        {
            os << " (************************)" << std::endl;
            for( effectors::const_iterator i = eff.begin(); i != eff.end(); ++i)
            {
                os << **i << std::endl;
            }
            os << " (************************)";
            return os;
        }



    }

}

