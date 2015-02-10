#include "yocto/gems/library.hpp"
#include "yocto/exception.hpp"

#include <iostream>

namespace yocto
{
    namespace gems
    {
        library:: library() :
        db(16,as_capacity),
        atoms()
        {
        }

        library:: ~library() throw()
        {
        }

        properties & library::insert(const string &name, const double mass)
        {


            properties::pointer p( new properties(name,mass) );
            if(!db.insert(p))
            {
                throw exception("Multiple properties '%s' or colliding uuid!", name.c_str());
            }
            assert(p->refcount()>0);
            return *p;
        }

        const string & properties::key()    const throw() { return name; }
        const word_t & properties::subkey() const throw() { return uuid; }

        properties & library:: operator[](const string &name)
        {
            properties::pointer *pp = db.search(name);
            if(!pp)
            {
                throw exception("no properties '%s'", name.c_str());
            }
            return **pp;
        }


        const properties & library:: operator[](const string &name) const
        {
            const properties::pointer *pp = db.search(name);
            if(!pp)
            {
                throw exception("no const properties '%s'", name.c_str());
            }
            return **pp;
        }

        void library:: __throw_multiple_uuid(word_t uuid) const
        {
            throw exception("library: multiple atom uuid %u", unsigned(uuid));
        }

        void library:: display() const
        {
            for(atom_info::table::const_iterator i=atoms.begin();i!=atoms.end();++i)
            {
                std::cerr << " " << (*i)->uuid << " " << std::endl;
            }
        }

    }

}
