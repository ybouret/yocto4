#include "yocto/lingua/pattern/logic.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lingua
    {
        
        ////////////////////////////////////////////////////////////////////////
        //
        // logical
        //
        ////////////////////////////////////////////////////////////////////////
        
        logical:: ~logical() throw() {}
        
        logical:: logical(uint32_t t) throw() :
        pattern(t),
        operands()
        {
            data = &operands;
        }
        
        logical:: logical( const logical &other ) :
        pattern(other.type),
        operands(other.operands)
        {
            if(operands.size<=0) throw exception("lingua::logical(NO OPERANDS)");
            data = &operands;
        }
        
        void logical:: reset() throw()
        {
            clear();
            for( pattern *p = operands.head;p;p=p->next)
            {
                p->reset();
            }
        }
        
        void logical:: write( ios::ostream &fp ) const
        {
            fp.emit<uint32_t>(operands.size);
            for(const pattern *p = operands.head;p;p=p->next)
            {
                p->save(fp);
            }
        }
        
        void logical:: append( pattern *p ) throw()
        {
            assert(p);
            operands.push_back(p);
        }
        
		 pattern *logical::remove() throw()
		 {
			 assert(operands.size>0);
			 return operands.pop_back();
		 }

        void logical:: optimize_all() throw()
        {
            for( pattern *p = operands.head;p;p=p->next)
            {
                p->optimize();
            }
        }
        
        void logical:: __viz(const void *parent, ios::ostream &fp) const
        {
            for(const pattern *p = operands.head;p;p=p->next)
            {
                p->viz(fp);
            }
            for(const pattern *p = operands.head;p;p=p->next)
            {
                fp.viz(parent); fp << " -> ";  fp.viz(p); fp << ";\n";
            }
        }
    }
}

#include "yocto/lingua/pattern/basic.hpp"

namespace yocto
{
    namespace lingua
    {
        
        pattern *logical:: EQUAL(const string &s)
        {
            const size_t n = s.size();
            if(n<=0) throw exception("lingua::EQUAL(no char)");
            auto_ptr<logical> p( AND::create() );
            for(size_t i=0; i <n; ++i )
                p->operands.push_back( single::create( s[i] ) );
            return p.yield();
        }
        
        pattern *logical:: EQUAL(const char *s)
        {
            const string tmp(s);
            return EQUAL(tmp);
        }
        
        pattern *logical:: AMONG(const string &s)
        {
            const size_t n = s.size();
            if(n<=0) throw exception("lingua::AMONG(no char)");
            auto_ptr<logical> p( OR::create() );
            for(size_t i=0; i <n; ++i )
                p->operands.push_back( single::create( s[i] ) );
            return p.yield();
        }
        
        pattern *logical:: AMONG(const char *s)
        {
            const string tmp(s);
            return AMONG(tmp);
        }
        
    }
    
    
}

