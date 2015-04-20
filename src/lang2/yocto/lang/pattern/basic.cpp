#include "yocto/lang/pattern/basic.hpp"

namespace yocto
{
    namespace lang
    {

        one_char:: ~one_char() throw() {}

        one_char:: one_char(uint32_t ID) throw() : pattern(ID)
        {
        }

        one_char:: one_char( const one_char &other ) throw() : pattern(other)
        {
        }

        bool one_char:: match(Y_LANG_PATTERN_MATCH_ARGS) const
        {
            t_char *ch = src.get(fp);
            if(ch)
            {
                if( is_valid(ch->code) )
                {
                    tkn.push_back(ch);
                    return true;
                }
                else
                {
                    src.unget(ch);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

    }
}


namespace yocto
{
    namespace lang
    {
        any1:: ~any1() throw() {}

        any1:: any1() throw() : one_char( any1::UUID ) {}

        bool any1::is_valid(code_t) const throw() { return true; }

        pattern * any1:: clone() const
        {
            return new any1();
        }

        pattern *any1:: create()
        {
            return new any1();
        }
    }

}

namespace yocto
{
    namespace lang
    {
        single:: ~single() throw() {}

        single:: single(code_t C) throw() : one_char( single::UUID ), code(C) {}

        bool single::is_valid(code_t) const throw() { return true; }

        single:: single(const single &other) throw(): one_char(*this), code(other.code) {}


        pattern * single:: clone() const
        {
            return new single(*this);
        }

        pattern *single:: create(code_t C)
        {
            return new single(C);
        }
    }
    
}
