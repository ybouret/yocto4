#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        any1:: ~any1() throw()
        {
        }

        any1:: any1() throw() : pattern( UUID )
        {
        }

        pattern * any1:: clone() const
        {
            return new any1();
        }

        any1 * any1:: create()
        {
            return new any1();
        }

        bool any1:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            t_char *ch = src.query();
            if(ch)
            {
                tkn.push_back(ch);
                return true;
            }
            else
            {
                return false;
            }
        }

        void any1:: __out(ios::ostream &) const
        {
        }


    }
}

namespace yocto
{
    namespace lingua
    {
        single:: ~single() throw()
        {
        }

        single:: single(const char C) throw() : pattern(UUID), code(C) {}

        single:: single(const single &other) throw() : pattern(other), code(other.code) {}

        single * single::create(const char C) { return new single(C); }

        pattern * single::clone() const { return new single(*this); }

        bool single:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            t_char *ch = src.query();
            if(ch)
            {
                if(code==ch->code)
                {
                    tkn.push_back(ch);
                    return true;
                }
                else
                {
                    src.store(ch);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        void single:: __out(ios::ostream &fp) const
        {
            fp.emit<uint8_t>(code);
        }


    }


}


#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace lingua
    {
        range:: ~range() throw()
        {
        }

        range:: range(const uint8_t lo, const uint8_t up) throw() :
        pattern(UUID),
        lower( min_of(lo,up) ),
        upper( max_of(lo,up) )
        {
        }

        range:: range(const range &other) throw() :
        pattern(other),
        lower(other.lower),
        upper(other.upper)
        {
        }

        range * range::create(const uint8_t lo, const uint8_t up)
        {
            return new range(lo,up);
        }

        pattern * range:: clone() const
        {
            return new range(*this);
        }

        bool range:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            t_char *ch = src.query();
            if(ch)
            {
                const uint8_t code = ch->code;
                if(lower<=code&&code<=upper)
                {
                    tkn.push_back(ch);
                    return true;
                }
                else
                {
                    src.store(ch);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        void range:: __out(ios::ostream &fp) const
        {
            fp.emit<uint8_t>(lower);
            fp.emit<uint8_t>(upper);
        }

    }

}

