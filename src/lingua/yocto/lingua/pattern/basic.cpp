#include "yocto/lingua/pattern/basic.hpp"
#include "yocto/ios/graphviz.hpp"

namespace yocto
{
    namespace lingua
    {
        one_char:: ~one_char() throw()
        {
        }

        one_char:: one_char(const uint32_t t) throw() : pattern(t)
        {
        }

        one_char:: one_char(const one_char &other) throw() : pattern(other)
        {
        }


        bool one_char:: match_empty() const
        {
            return false;
        }
    }
}

namespace yocto
{
    namespace lingua
    {
        any1:: ~any1() throw()
        {
        }

        any1:: any1() throw() : one_char( UUID )
        {
            YOCTO_LINGUA_PATTERN_IS(any1);
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

        void any1:: __sav(ios::ostream &) const
        {
            // do nothing
        }

        void  any1:: __viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp << "[label=\"ANY1\",shape=circle];\n";
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

        single:: single(const char C) throw() : one_char(UUID), code(C)
        {
            YOCTO_LINGUA_PATTERN_IS(single);
        }

        single:: single(const single &other) throw() :
        one_char(other), code(other.code)
        {
            YOCTO_LINGUA_PATTERN_IS(single);
        }

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

        void single:: __sav(ios::ostream &fp) const
        {
            fp.emit<uint8_t>(code);
        }

        void  single:: __viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);

            fp << "[label=\"";
            ios::graphviz_encode(char(code),fp);
            fp << "\",shape=square];\n";
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
        one_char(UUID),
        lower( min_of(lo,up) ),
        upper( max_of(lo,up) )
        {
            YOCTO_LINGUA_PATTERN_IS(range);
        }

        range:: range(const range &other) throw() :
        one_char(other),
        lower(other.lower),
        upper(other.upper)
        {
            YOCTO_LINGUA_PATTERN_IS(range);
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

        void range:: __sav(ios::ostream &fp) const
        {
            fp.emit<uint8_t>(lower);
            fp.emit<uint8_t>(upper);
        }


        void range:: __viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp << "[label=\"";
            ios::graphviz_encode(char(lower),fp);
            fp << '-';
            ios::graphviz_encode(char(upper),fp);
            fp << "\",shape=note];\n";
        }
    }

}

