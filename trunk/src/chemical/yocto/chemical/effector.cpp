#include "yocto/chemical/effector.hpp"

namespace yocto
{
    namespace chemical
    {
        
        effector:: ~effector() throw()
        {
        }
        
        effector:: effector( const string &id ) :
        name(id),
        factor(1)
        {
        }
        
        const string & effector::key() const throw() { return name; }
        
               
        effectors:: effectors()
        {
            
        }
        
        effectors:: ~effectors() throw()
        {
        }
        
        
        void effectors:: collect( solution &dSdt, double t, double zeta, const solution &S ) const
        {
            assert(dSdt.components==S.components);
            solution rate(S);
            dSdt.ldz();
            
            for( const_iterator i=begin(); i != end(); ++i )
            {
                const effector &eff = **i;
                rate.ldz();
                eff.call(rate, t, zeta,S);
                for( solution::iterator j=rate.begin(),k=dSdt.begin();j!=rate.end();++j,++k)
                {
                    const component &p = (*j);
                    component       &q = (*k);
                    q.conc += eff.factor * p.conc;
                }
            }
        }
        
        std::ostream & operator<<( std::ostream &os, const effectors &db)
        {
            os << "________________" << std::endl;
            os << std::endl;
            for( effectors::const_iterator i=db.begin(); i != db.end(); ++i )
            {
                const effector &eff = **i;
                os << "effector '" << eff.name << "'" << std::endl;
            }
            os << "________________";
            return os;
        }

    }

}
