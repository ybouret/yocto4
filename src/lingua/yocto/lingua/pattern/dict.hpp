#ifndef YOCTO_LINGUA_PATTERN_DICT_INCLUDED
#define YOCTO_LINGUA_PATTERN_DICT_INCLUDED 1

#include "yocto/lingua/pattern.hpp"
#include "yocto/associative/map.hpp"
#include "yocto/ptr/shared.hpp"

namespace yocto
{
    namespace lingua
    {
        typedef shared_ptr<pattern>         pattern_ptr;
        typedef map<string,pattern_ptr>     pattern_db;

        //! simple dictionary
        class p_dict : public pattern_db
        {
        public:
            explicit p_dict() throw();
            virtual ~p_dict() throw();

            void define(const string &id, pattern *p);
            void define(const char   *id, pattern *p);

            void define(const string &id, const string &expr);
            void define(const char   *id, const char   *expr);


            pattern * operator[](const string &id) const;
            pattern * operator[](const char   *id) const;

            void check_id(const string &id) const;

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(p_dict);
        };

    }
}

#endif
