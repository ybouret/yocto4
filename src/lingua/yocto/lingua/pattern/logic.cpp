#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        logical:: ~logical() throw()
        {
        }

        logical:: logical(const uint32_t t) throw() :
        pattern(t),
        operands()
        {}

        logical:: logical(const logical &other) :
        pattern(other),
        operands(other.operands)
        {
        }

        void logical:: __sav(ios::ostream &fp) const
        {
            fp.emit<uint32_t>(operands.size);
            for(const pattern *p=operands.head;p;p=p->next)
            {
                p->save(fp);
            }
        }

        void logical:: add( pattern *p ) throw()
        {
            operands.push_back(p);
        }

        void logical:: __subviz(ios::ostream &fp) const
        {
            for(const pattern *p=operands.head;p;p=p->next)
            {
                p->__viz(fp);
                fp.viz( (const pattern *)this ); fp << "->"; fp.viz(p); fp << ";\n";
            }
        }

    }
}

namespace yocto
{
    namespace lingua
    {
        AND::  AND() throw() : logical(UUID)
        {
            YOCTO_LINGUA_PATTERN_IS(AND);
        }

        AND:: ~AND() throw() {}

        AND:: AND(const AND &other) :
        logical(other)
        {
            YOCTO_LINGUA_PATTERN_IS(AND);
        }

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


        void AND:: __viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp << "[label=\"&&\",shape=house];\n";
            __subviz(fp);
        }

        bool AND:: match_empty() const
        {
            for(const pattern *p=operands.head;p;p=p->next)
            {
                if(p->match_empty())
                {
                    continue;
                }
                else
                {
                    // one of the requested operand doesn't match empty
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
        OR::  OR() throw() : logical(UUID)
        {
            YOCTO_LINGUA_PATTERN_IS(OR);
        }

        OR:: ~OR() throw() {}

        OR:: OR(const OR &other) : logical(other)
        {
            YOCTO_LINGUA_PATTERN_IS(OR);
        }

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

        void OR:: __viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp << "[label=\"||\",shape=diamond];\n";
            __subviz(fp);
        }

        bool OR:: match_empty() const
        {
            // if one of the operands accept empty => true
            for(const pattern *p=operands.head;p;p=p->next)
            {
                if(p->match_empty())
                {
                    return true;;
                }
            }
            return false;
        }


    }

}


namespace yocto
{
    namespace lingua
    {
        NONE::  NONE() throw() : logical(UUID)
        {
            YOCTO_LINGUA_PATTERN_IS(NONE);
        }

        NONE:: ~NONE() throw() {}

        NONE:: NONE(const NONE &other) : logical(other)
        {
            YOCTO_LINGUA_PATTERN_IS(NONE);
        }

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

        void NONE:: __viz(ios::ostream &fp) const
        {
            fp.viz((const pattern *)this);
            fp << "[label=\"!!\",shape=triangle];\n";
            __subviz(fp);
        }

        bool NONE:: match_empty() const
        {
            return false;
        }

    }

}

#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        logical * logical::among(const string &str)
        {
            auto_ptr<logical> p( OR::create() );
            const size_t n = str.size();
            for( size_t i=0;i<n;++i)
            {
                p->add( single::create(str[i]) );
            }
            return p.yield();
        }

        logical *logical:: among(const char *str)
        {
            const string tmp(str);
            return among(tmp);
        }

        logical * logical::equal(const string &str)
        {
            auto_ptr<logical> p( AND::create() );
            const size_t n = str.size();
            for( size_t i=0;i<n;++i)
            {
                p->add( single::create(str[i]) );
            }
            return p.yield();
        }
        
        logical *logical:: equal(const char *str)
        {
            const string tmp(str);
            return equal(tmp);
        }
        
        
    }
    
}



