#ifndef YOCTO_REGEXP_COMPILER_INCLUDED
#define YOCTO_REGEXP_COMPILER_INCLUDED 1

#include "yocto/lang/regexp.hpp"
#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {

        class RegExp
        {
        public:
            RegExp(const char *expr, size_t n, const p_dict *d);
            ~RegExp() throw();

            const char   *curr;
            const char   *last;
            const p_dict *dict;
            int           level;

            //! create a new 'AND' from curr.
            pattern *parse_expr();

        private:

            //! parse an escape sequence in an expression
            pattern *parse_expr_esc();

            //! parce braces
            pattern *parse_braces(logical &);


            //! parse classes
            pattern *parse_class();

            //! parse posix
            pattern *parse_posix();

            //! parse class escape sequence
            pattern *parse_class_esc();

            pattern *parse_esc_hexa(const char *where);


            YOCTO_DISABLE_COPY_AND_ASSIGN(RegExp);
        };
    }
}

#endif
