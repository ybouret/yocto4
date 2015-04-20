#ifndef YOCTO_LANG_PATTERN_INCLUDED
#define YOCTO_LANG_PATTERN_INCLUDED 1

#include "yocto/lang/source.hpp"
#include "yocto/counted-object.hpp"

namespace yocto
{
    namespace lang
    {

#define Y_LANG_PATTERN_MATCH_ARGS token &tkn, source &src, ios::istream &fp

        class pattern : public object
        {
        public:
            pattern *next;
            pattern *prev;
            const uint32_t uuid;
            virtual ~pattern() throw();

            //! clone the behavior
            virtual pattern *clone() const = 0;

            //! if matching, content is appended to tkn
            virtual bool     match( Y_LANG_PATTERN_MATCH_ARGS ) const = 0;


        protected:
            explicit pattern(uint32_t ID) throw();
            pattern(const pattern &) throw();

        private:
            YOCTO_DISABLE_ASSIGN(pattern);
        };

    }
}


#endif

