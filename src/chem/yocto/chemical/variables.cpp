#include "yocto/chemical/variables.hpp"

namespace yocto
{
    namespace chemical
    {

        variable:: ~variable() throw() {}

        variable::variable(const size_t iv, const char *id, const char *ld) :
        indx(iv),
        name(id),
        load(ld)
        {
        }

        variable::variable(const size_t iv, const string &id, const string &ld) :
        indx(iv),
        name(id),
        load(ld)
        {
        }

        const string & variable:: key() const throw()
        {
            return name;
        }

    }

}

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {

        variables:: ~variables() throw()
        {
            lib.decrease();
        }

        size_t variables:: operator[](const string &name) const
        {
            const variable::ptr *ppVar = db.search(name);
            if(!ppVar)
            {
                throw exception("no variable '%s'", name.c_str());
            }
            return (*ppVar)->indx;
        }

        size_t variables:: operator[](const char *name) const
        {
            const string Name(name);
            return (*this)[Name];
        }


        variables::iterator variables::begin() const throw() { return db.begin(); }
        variables::iterator variables::end()   const throw() { return db.end();   }


#define YCHEM_VARIABLES_CTOR(N) \
lib(a_lib), count(lib.size()),extra(0),db(N,as_capacity)
        variables:: variables(const library &a_lib,
                              const char    *names[],
                              const char    *loads[],
                              const size_t   num) :
        YCHEM_VARIABLES_CTOR(num)
        {
            for(size_t i=0;i<num;++i)
            {
                assert(names);
                assert(loads);
                const string name = names[i];
                const string load = loads[i];
                create_variable(name,load);
            }
            lib.increase();
        }

        variables:: variables(const library       &a_lib,
                              const array<string> &names,
                              const array<string> &loads) :
        YCHEM_VARIABLES_CTOR(names.size())
        {
            assert(names.size()==loads.size());
            for(size_t i=1;i<=names.size();++i)
            {
                create_variable(names[i],loads[i]);
            }
            lib.increase();
        }


        void variables:: create_variable(const string &name, const string &load)
        {
            const variable::ptr pV( new variable(lib.size()+db.size()+1,name,load) );
            if(!db.insert(pV))
            {
                throw exception("mutiple variable '%s'", name.c_str());
            }
            ++(size_t&)count;
            ++(size_t&)extra;
        }
        
    }
    
}
