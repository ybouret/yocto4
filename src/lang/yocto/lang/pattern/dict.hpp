#ifndef YOCTO_LANG_DICT_INCLUDED
#define YOCTO_LANG_DICT_INCLUDED 1

#include "yocto/lang/pattern.hpp"
#include "yocto/ptr/shared.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace lang
    {
        
        typedef shared_ptr<pattern>     pattern_ptr; //!< for dynamical handling
        typedef map<string,pattern_ptr> p_dict_type; //!< base type
        
        class p_dict : public p_dict_type
        {
        public:
            explicit p_dict() throw();
            virtual ~p_dict() throw();
            void add( const string &name, pattern *p);
            void add( const char   *name, pattern *p);
            void add( const string &name, const string &regex);
            void add( const char   *name, const char   *regex);
            
            pattern * create(const string &name) const;
            pattern * create(const char   *name) const;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(p_dict);
        };
    }
}


#endif
