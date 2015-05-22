#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {
        logical:: ~logical() throw()
        {
        }

        logical:: logical(uint32_t ID) throw() : pattern(ID), operands() {}

        logical:: logical(const logical &other) : pattern(other), operands(other.operands) {}

        void logical:: append( pattern *p ) throw()
        {
            assert(p);
            operands.push_back(p);
        }
        
        void logical:: vizops( ios::ostream &fp ) const
        {
            for( const pattern *p = operands.head; p; p=p->next)
            {
                p->viz(fp);
                vizlink(fp, this, p);
            }
        }

        const void * logical:: content() const throw()
        {
            return &operands;
        }

        void logical::save( ios::ostream &fp ) const
        {
            fp.emit(uuid);
            fp.emit<uint32_t>(operands.size);
            for( const pattern *p = operands.head; p; p=p->next)
            {
                p->save(fp);
            }

        }


    }
}

#include "yocto/lang/pattern/basic.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace lang
    {
        
        logical * logical::among(const string &s)
        {
            const size_t      n = s.size();
            if(n<=0)
            {
                throw exception("logical::among(EMPTY STRING)");
            }
            auto_ptr<logical> p( OR::create() );
            for(size_t i=0;i<n;++i)
            {
                p->append( single::create(s[i]) );
            }
            return p.yield();
        }

        logical * logical::among(const char *txt)
        {
            const string s(txt);
            return logical::among(s);
        }
    }
}

namespace yocto
{
    namespace lang
    {

        logical * logical::equal(const string &s)
        {
            const size_t      n = s.size();
            if(n<=0)
            {
                throw exception("logical::equal(EMPTY STRING)");
            }
            auto_ptr<logical> p( AND::create() );
            for(size_t i=0;i<n;++i)
            {
                p->append( single::create(s[i]) );
            }
            return p.yield();
        }

        logical * logical::equal(const char *txt)
        {
            const string s(txt);
            return logical::equal(s);
        }
    }
}

