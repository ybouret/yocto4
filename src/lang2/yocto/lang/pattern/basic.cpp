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
        
        void any1::viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[label=\"ANY1\",shape=box];\n");
        }
    }

}

namespace yocto
{
    namespace lang
    {
        single:: ~single() throw() {}

        single:: single(code_t C) throw() : one_char( single::UUID ), code(C) {}

        bool single::is_valid(code_t C) const throw() { return code==C; }

        single:: single(const single &other) throw(): one_char(*this), code(other.code) {}


        pattern * single:: clone() const
        {
            return new single(*this);
        }

        pattern *single:: create(code_t C)
        {
            return new single(C);
        }

        const void *single:: content() const throw() { return &code; }
        
        void single::viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[shape=ellipse,label=\"");
            encode(code, fp);
            fp("\"];\n");
        }
    }
    
}


namespace yocto
{
    namespace lang
    {
        range:: ~range() throw() {}

        range:: range(code_t lo, code_t hi) throw() : one_char( single::UUID ), lower(lo), upper(hi) {}

        bool range::is_valid(code_t C) const throw() { return (C>=lower) && (C<=upper); }

        range:: range(const range &other) throw(): one_char(*this), lower(other.lower), upper(other.upper) {}


        pattern * range:: clone() const
        {
            return new range(*this);
        }

        pattern *range:: create(code_t lo, code_t hi)
        {
            if(lo>hi)
            {
                cswap(lo, hi);
            }
            return new range(lo,hi);
        }
        
        void range::viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[shape=tab,label=\"");
            encode(lower, fp);
            fp << "-";
            encode(upper, fp);
            fp << "\"];\n";
        }
    }
    
}

#if 0
namespace yocto
{
    namespace lang
    {
        multi::~multi() throw()
        {
        }

        multi:: multi() throw() : one_char( multi::UUID ), codes() {}


        multi:: multi(const multi &other) : one_char(other), codes(other.codes) {}

        bool multi:: is_valid(code_t C)const throw() { return codes.search(C); }

        multi *multi::create() { return new multi(); }

        pattern *multi:: clone() const
        {
            return new multi(*this);
        }
        
        void multi:: add(code_t C) { (void) codes.insert(C); }

        void multi:: viz(ios::ostream &fp) const
        {
            fp.viz( (const pattern *)this );
            fp("[shape=folder,label=\"");
            fp("multi");
            fp("\"];\n");
        }
        
    }

}
#endif


