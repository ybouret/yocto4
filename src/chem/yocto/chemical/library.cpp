#include "yocto/chemical/library.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        library:: library() throw() :
        nref(0),
        db(),
        max_name_length(0),
        _proton("H+")
        {
        }
        
        library:: ~library() throw()
        {
        }
        
        size_t library::size() const throw() { return db.size(); }
        
        library::iterator library::begin() throw()
        {
            return db.begin();
        }
        
        library::iterator library::end() throw()
        {
            return db.end();
        }
        
        library::const_iterator library::begin() const throw()
        {
            return db.begin();
        }
        
        library::const_iterator library::end() const throw()
        {
            return db.end();
        }
        
        species::pointer & library:: operator[](const string &id )
        {
            species::pointer *pp = db.search(id);
            if(!pp)
                throw exception("no library['%s']", id.c_str());
            return *pp;
        }
        
        const species::pointer & library:: operator[](const string &id ) const
        {
            const species::pointer *pp = db.search(id);
            if(!pp)
                throw exception("no const library['%s']", id.c_str());
            return *pp;
        }
        
        species::pointer & library:: operator[](const char   *id)
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        const species::pointer & library:: operator[](const char   *id) const
        {
            const string ID(id);
            return (*this)[ID];
        }
        
        species & library:: add(const string &name, const int z)
        {
            if( nref > 0 )
                throw exception("library is locked while adding '%s',z=%d", name.c_str(), z);
            
            species::pointer p( new species(name,z) );
            if(!db.insert(p))
                throw exception("library.insert(multiple '%s')",name.c_str());
            
            (size_t &)(p->indx) = db.size();
            if(name.size()>max_name_length) max_name_length = name.size();
            return *p;
        }
        
        species & library:: add(const char *name, const int z)
        {
            const string NAME(name);
            return add(NAME,z);
        }
        
        void library:: find_max_name_length() throw()
        {
            max_name_length = 0;
            size_t j        = 0;
            for(iterator i=begin();i!=end();++i)
            {
                species &sp = **i;
                (size_t &)(sp.indx) = ++j;
                if(sp.name.length()>max_name_length)
                {
                    max_name_length = sp.name.length();
                }
            }
        }
        
        void library:: remove(const string &name)
        {
            if( nref > 0 )
                throw exception("library is locked while removind '%s'", name.c_str());
            
            if( db.remove(name) )
            {
                find_max_name_length();
            }
            
        }
        
        void library:: remove(const char *name)
        {
            const string NAME(name);
            remove(NAME);
        }
        
        void library:: output(std::ostream &os) const
        {
            
            os << "{" << std::endl;
            for(const_iterator i=begin();i!=end();++i)
            {
                const species &sp = **i;
                os << sp.name;
                for(size_t j=sp.name.size();j<max_name_length;++j)
                {
                    os << ' ';
                }
                os << " : " << sp.z << std::endl;
            }
            os << "}";
        }
        
        bool library:: has(const string &name) const throw()
        {
            return 0 != db.search(name);
        }
        
        bool library:: has(const char *name) const
        {
            const string NAME(name);
            return 0 != db.search(NAME);
        }
        
        size_t library:: index_of(const string &name) const
        {
            const species::pointer *pp = db.search(name);
            if(!pp)
            {
                throw exception("library::index_of(no '%s')", name.c_str());
            }
            return (**pp).indx;
        }
        
        size_t library:: index_of(const char *name) const
        {
            const string NAME(name);
            return index_of(NAME);
        }
        
        void library:: increase() const throw() { ++( (size_t &)nref ); }
        void library:: decrease() const throw() { assert(nref>0); --( (size_t&)nref ); }
        
        std::ostream & library:: display(std::ostream &os, const array<double> &C) const
        {
            assert(C.size()>=size());
            os << "{" << std::endl;
            size_t k=1;
            for(const_iterator i=begin();i!=end();++i,++k)
            {
                const species &sp = **i;
                os << '\t' << '[' << sp.name <<']';
                for(size_t j=sp.name.size();j<max_name_length;++j)
                {
                    os << ' ';
                }
                os << " = " << C[k] << std::endl;
            }
            os << "}";
            
            return os;
        }
        
        
    }
    
}
