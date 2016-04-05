#include "yocto/lingua/pattern/logic.hpp"

namespace yocto
{
    namespace lingua
    {
        logical:: ~logical() throw()
        {
        }

        logical:: logical(const uint32_t t) throw() :
        pattern(t)
        {}

        logical:: logical(const logical &other) :
        pattern(other)
        {
        }
        
        void logical:: __out(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(operands.size);
            for(const pattern *p=operands.head;p;p=p->next)
            {
                p->save(fp);
            }
        }
    }
}

namespace yocto
{
    namespace lingua
    {
        AND::  AND() throw() : logical(UUID) {}
        AND:: ~AND() throw() {}

        AND:: AND(const AND &other) : logical(other) {}

        pattern *AND:: clone() const
        {
            return new AND(*this);
        }

        AND * AND:: create()
        {
            return new AND();
        }

        bool AND:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            assert(operands.size>0);
            for(const pattern *p = operands.head;p;p=p->next)
            {
                token sub;
                if(p->match(sub,src))
                {
                    tkn.merge_back(sub);
                }
                else
                {
                    assert(0==sub.size);
                    src.unread(tkn);
                    return false;
                }
            }
            return true;
        }


    }

}

namespace yocto
{
    namespace lingua
    {
        OR::  OR() throw() : logical(UUID) {}
        OR:: ~OR() throw() {}

        OR:: OR(const OR &other) : logical(other) {}

        pattern *OR:: clone() const
        {
            return new OR(*this);
        }

        OR * OR:: create()
        {
            return new OR();
        }

        bool OR:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            assert(operands.size>0);
            for(const pattern *p = operands.head;p;p=p->next)
            {
                if(p->match(tkn,src))
                {
                    return true;
                }
                assert(0==tkn.size);
            }
            return false;
        }

    }
    
}


namespace yocto
{
    namespace lingua
    {
        NONE::  NONE() throw() : logical(UUID) {}
        NONE:: ~NONE() throw() {}

        NONE:: NONE(const NONE &other) : logical(other) {}

        pattern *NONE:: clone() const
        {
            return new NONE(*this);
        }

        NONE * NONE:: create()
        {
            return new NONE();
        }

        bool NONE:: match(YOCTO_LINGUA_PATTERN_MATCH_ARGS) const
        {
            assert(0==tkn.size);
            assert(operands.size>0);
            for(const pattern *p = operands.head;p;p=p->next)
            {
                token sub;
                if(p->match(sub,src))
                {
                    src.unread(sub);
                    return false;
                }
            }
            t_char *ch = src.query();
            if(ch)
            {
                tkn.push_back(ch);
            }
            return true;
        }
    }
    
}

