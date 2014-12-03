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
        Vmax(1.0)
        {
        }
        
        const string & effector:: key() const throw() { return name; }
        
        void effector:: update( array<double> &rho, const parameters &params) const throw()
        {
            assert(rho.size()>=params.lib.size());
            for(size_t j=params.lib.size();j>0;--j)
            {
                rho[j] *= Vmax;
            }
        }
        
        
    }
    
}


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
                             const parameters    &params)
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
                    rho[j] += eff.Vmax * _rho[j];
                }
            }
        }
        
    }
    
}

