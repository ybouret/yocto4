#ifndef YOCTO_ASSOCIATIVE_TYPE_GLOSSARY_INCLUDED
#define YOCTO_ASSOCIATIVE_TYPE_GLOSSARY_INCLUDED 1

#include "yocto/associative/glossary.hpp"
#include <typeinfo>

#include <iostream>

namespace yocto
{
    namespace hidden
    {
        struct type_glossary_info
        {
            static const char name[];
            static void throw_different_values(const char *the_name,const char *the_id);
        };
    }

    template <typename T>
    class type_glossary : public glossary<T>
    {
    public:
        using typename glossary<T>::param_type;
        using typename glossary<T>::const_type;

        inline explicit type_glossary() : glossary<T> () {}
        inline explicit type_glossary(const size_t n) : glossary<T>(n) {}
        virtual        ~type_glossary() throw() {}

        virtual const char *name() const throw() { return hidden::type_glossary_info::name; }

        inline void enroll( const std::type_info &ti, param_type arg)
        {
            const char *key = ti.name();
            std::cerr << "key=" << key << " => " << arg << std::endl;
            const_type *ans = this->search(key);
            if(ans)
            {
                //______________________________________________________________
                //
                // already exists
                //______________________________________________________________
                if( *ans != arg )
                {
                    hidden::type_glossary_info::throw_different_values(name(),key);
                }
            }
            else
            {
                this->insert(key,arg);
            }
        }

        template <typename U>
        inline void enroll(param_type arg)
        {
            enroll( typeid(U), arg );
        }

    
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(type_glossary);
    };


}

#endif

