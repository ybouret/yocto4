#ifndef YOCTO_LANG_PATTERN_DICT_INCLUDED
#define YOCTO_LANG_PATTERN_DICT_INCLUDED 1


#include "yocto/lang/pattern.hpp"
#include "yocto/ptr/arc.hpp"
#include "yocto/associative/map.hpp"

namespace yocto
{
    namespace lang
    {

        typedef arc_ptr<pattern>        pattern_ptr;
        typedef map<string,pattern_ptr> pattern_map;

        class p_dict : public pattern_map
        {
        public:
            explicit p_dict() throw();
            virtual ~p_dict() throw();

            void define( const string &id, pattern *p );
            void define( const char   *id, pattern *p );

            pattern * create() const;
            bool      has(const string &) const throw();
            bool      has(const char   *) const;

            pattern  * operator[](const string &) const;
            pattern  * operator[](const char   *) const;


        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(p_dict);

        };
    }
}

#endif
