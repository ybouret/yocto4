#include "yocto/chemical/collection.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace chemical
    {
        
        collection:: ~collection() throw() {}
        
        
#define Y_CHEM_ZTABLE(name,z) do { \
const string id(name); \
if( !ztable.insert(id,z) ) throw exception(fmt, name); \
} while(false)
        
        void collection:: build_ztable()
        {
            ztable.reserve(16);
            static const char fmt[] = "ztable: multiple %s";
            Y_CHEM_ZTABLE("H+",      1);
            Y_CHEM_ZTABLE("HO-",    -1);
            Y_CHEM_ZTABLE("Na+",     1);
            Y_CHEM_ZTABLE("Cl-",    -1);
            Y_CHEM_ZTABLE("K+",      1);
            Y_CHEM_ZTABLE("CO2",     0);
            Y_CHEM_ZTABLE("HCO3-",  -1);
            Y_CHEM_ZTABLE("CO3--",  -2);
            Y_CHEM_ZTABLE("NH3",     0);
            Y_CHEM_ZTABLE("NH4+",    1);
        }
        
        collection:: collection() :
        species::db(),
        ztable()
        {
            build_ztable();
        }
        
        collection:: collection(size_t n) :
        species::db(n,as_capacity),
        ztable()
        {
            build_ztable();
        }
        
        
        species & collection::add( const string &name, int z)
        {
            species::ptr p( new species(name,z) );
            if( !insert(p) )
                throw exception("chemical.collection.add(mutliple '%s')", name.c_str());
            p->indx = this->size();
            return *p;
        }
        
        
        species & collection::add( const char *name, int z)
        {
            const string NAME(name);
            return add(NAME,z);
        }
        
        bool collection:: has( const string &name ) const throw()
        {
            return 0 != search(name);
        }
        
        bool collection:: has( const char *name ) const
        {
            const string NAME(name);
            return has(NAME);
        }
        
        species::ptr & collection:: operator[]( const string &id )
        {
            species::ptr *pp = search(id);
            if( !pp )
                throw exception("chemical.collection[no '%s']", id.c_str());
            return *pp;
        }
        
        species::ptr & collection:: operator[]( const char *id )
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        const species::ptr & collection:: operator[]( const string &id ) const
        {
            const species::ptr * pp = search(id);
            if( !pp )
                throw exception("chemical.collection[no '%s'].const", id.c_str());
            return *pp;
        }
        
        const species::ptr & collection:: operator[]( const char *id ) const
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        void collection:: suppress(const string &name) throw()
        {
            if( remove(name) )
                update_indices();
        }
        
        void collection:: update_indices() throw()
        {
            size_t i = 1;
            for( iterator sp = begin(); sp != end(); ++sp, ++i )
            {
                (**sp).indx = i;
            }
        }
        
        bool collection:: has_valid_indices() const throw()
        {
            size_t i = 1;
            for( const_iterator sp = begin(); sp != end(); ++sp, ++i )
            {
                if( (**sp).indx != i )
                    return false;
            }
            return true;
        }
        
        species & collection:: add(const string &name)
        {
            const int *z = ztable.search(name);
            if( !z )
                throw exception("collection: no charge for '%s'", name.c_str());
            return add(name, *z);
        }
        
        species & collection:: add(const char *name)
        {
            const string NAME(name);
            return add(NAME);
        }
        
        void collection:: dump_ztable( std::ostream &os ) const
        {
            for( ztable_type::const_iterator i=ztable.begin();i!=ztable.end();++i)
            {
                const string &k = i->key;
                const int     z = *i;
                os << k << " / " << z << std::endl;
            }
        }
        
        size_t collection:: max_name_length() const throw()
        {
            size_t res = 0;
            for( collection::const_iterator i = begin(); i != end(); ++i )
                res = max_of(res, (**i).name.size());
            return res;
        }

        
        std::ostream & operator<<( std::ostream &os, const collection &lib)
        {
            const size_t maxlen = lib.max_name_length();
            os << "{" << std::endl;
            for( collection::const_iterator i = lib.begin(); i != lib.end(); ++i )
            {
                const species &sp = **i;
                os << "\t\"" << sp.name;
                for(size_t i=maxlen-sp.name.size();i>0;--i) os << ' ';
                os << "\" : " << sp.z << std::endl;
            }
            os << "}";
            return os;
        }
        
    }
    
}
