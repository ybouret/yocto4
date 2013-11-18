#include "yocto/aqueous/initializer.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/kernel/svd.hpp"

namespace yocto
{
	namespace aqueous
	{
        
		double constraint:: fixed( double ) const throw()
		{
			return fixedValue;
		}
        
		constraint::~ constraint() throw() {}
        
        size_t constraint:: size() const throw()
        {
            return coefficients.size();
        }
        
        
        constraint::iterator constraint:: begin() const throw()
        {
            return coefficients.begin();
        }
        
        constraint::iterator constraint:: end() const throw()
        {
            return coefficients.end();
        }
        
        bool constraint:: has( const string &id ) const throw()
        {
            return coefficients.search( id ) != 0;
        }
        
        bool constraint:: has( const char *id ) const throw()
        {
            const string ID(id);
            return has(ID);
        }
        
        
#if defined(_MSC_VER)
		// this in ctor
#pragma warning ( disable : 4355 )
#endif
		constraint:: constraint( const library &L, double v ) :
		value( this, & constraint::fixed ),
        coefficients(),
        fixedValue(v),
        lib(L)
		{}
        
		constraint:: constraint( const library &L, const initproc &v ) :
		value( v ),
        coefficients(),
        fixedValue(0),
        lib(L)
		{}
        
        
        void constraint:: set_value( double v )
        {
            const initproc new_v(this, & constraint::fixed );
            value = new_v;
            (double &)fixedValue = v;
        }
        
        
        
		void constraint:: add( const string &id, double w )
		{
			if( ! lib.search(id) )
				throw exception("constraint( no '%s' )", id.c_str() );
            
			if( ! coefficients.insert(id, w) )
				throw exception("constraint( multiple '%s' )", id.c_str() );
		}
        
        void constraint:: add( const char   *id, double w )
        {
            const string ID(id);
            add( ID, w );
        }
        
        double * constraint:: weight_address_of( const string &id )
        {
            return coefficients.search( id );
        }
        
        double *constraint:: weight_address_of( const char * id )
        {
            const string ID(id);
            return weight_address_of(ID);
        }
        
        const double * constraint:: weight_address_of( const string &id ) const
        {
            return coefficients.search( id );
        }
        
        const double *constraint:: weight_address_of( const char * id ) const
        {
            const string ID(id);
            return weight_address_of(ID);
        }
        
        
		initializer:: ~initializer() throw()
		{
		}
        
		initializer:: initializer( const library &L ) throw() :
		constraints(),
        lib(L),
        is_variable( false )
		{
		}
        
        
		constraint & initializer:: create( double v )
		{
			constraint::ptr p( new constraint(lib,v) );
			constraints.push_back(p);
			return *p;
		}
        
		constraint & initializer:: create( const initproc &v )
		{
			constraint::ptr p( new constraint(lib,v) );
			constraints.push_back(p);
			(bool&)is_variable = true;
			return *p;
		}
        
        constraint & initializer:: operator[]( size_t index ) throw()
        {
            assert(index>0);
            assert(index<size());
            return *constraints[index];
        }
        
        const constraint & initializer:: operator[]( size_t index ) const throw()
        {
            assert(index>0);
            assert(index<size());
            return *constraints[index];
        }
        
        std::ostream & operator<<( std::ostream &os, const initializer &ini )
        {
            os << "################################" << std::endl;
            for( size_t i=1; i <= ini.constraints.size(); ++i )
            {
                const constraint &cn = *ini.constraints[i];
                os << "# ";
                os << cn.value(0.0) << " = ";
                constraint::iterator it = cn.begin();
                for( size_t j=1; j <= cn.size(); ++j, ++it )
                {
                    const string &id = it->key;
                    const double  w  = *it;
                    os << "(" << w << ")*[" << id << "]";
                    if( j < cn.size() ) os << "+";
                }
                os << std::endl;
            }
            os << "################################" << std::endl;
            return os;
        }
        
        
        void initializer:: electroneutrality()
		{
			constraint &c = create(0.0);
			for( library::const_iterator i = lib.begin(); i != lib.end(); ++i )
			{
				const species &sp = **i;
				c.add(sp.name, sp.z);
			}
		}
        
		size_t initializer:: size() const throw() { return constraints.size(); }
        
        
	}
    
}
