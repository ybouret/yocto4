#include "yocto/lang/pattern/logic.hpp"

namespace yocto
{
    namespace lang
    {

        logical:: ~logical() throw()
        {
        }
        
        logical:: logical(const uint32_t id) throw() :
        pattern(id),
        operands()
        {
            
        }
        
        logical:: logical( const logical &other ) :
        pattern(other.type),
        operands(other.operands )
        {
            
        }
        
        void logical:: reset() throw()
        {
            this->clear();
            for(pattern *op=operands.head;op;op=op->next)
            {
                op->clear();
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

        void logical:: append( pattern *p ) throw()
        {
            assert(p);
            operands.push_back(p);
        }

        pattern * logical:: remove() throw()
        {
            assert(operands.size>0);
            return operands.pop_back();
        }
    }
    
    
}

#include "yocto/lang/pattern/basic.hpp"
#include "yocto/exception.hpp"
#include "yocto/ptr/auto.hpp"

namespace yocto
{
    namespace lang
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


namespace yocto
{
    namespace lang
    {
        AND:: ~AND() throw() {}
        
        AND:: AND() throw() : logical(tag) { self = (AND *)this; }
        
        AND:: AND( const AND &other ) : logical(other) { self = (AND *)this; }
        
        pattern * AND:: clone() const
        {
            return new AND(*this);
        }
        
        logical * AND:: create()
        {
            return new AND();
        }
        
        bool AND:: match(source &src, ios::istream &fp)
        {
            assert(0==size);
            
            
            for(pattern *op=operands.head;op;op=op->next)
            {
                if(!op->match(src,fp))
                {
                    src.unget(*this);
                    return false;
                }
                merge_back(*op);
            }
            
            
            return true;
        }
        
        void AND:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"&&\"];\n";
            __viz(this,fp);
        }

        
    }

}

namespace yocto
{
    namespace lang
    {
        OR:: ~OR() throw() {}
        
        OR:: OR() throw() : logical(tag) { self = (OR *)this; }
        
        OR:: OR( const OR &other ) : logical(other) { self = (OR *)this; }
        
        pattern * OR:: clone() const
        {
            return new OR(*this);
        }
        
        logical * OR:: create()
        {
            return new OR();
        }
        
        bool OR:: match(source &src, ios::istream &fp)
        {
            assert(0==size);
            
            
            for(pattern *op=operands.head;op;op=op->next)
            {
                if(op->match(src,fp))
                {
                    merge_back(*op);
                    return true;
                }
            }
            
            return false;
        }
     
        void OR:: viz( ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"||\"];\n";
            __viz(this,fp);
        }

    }
    
}

namespace yocto
{
    namespace lang
    {
        NOT:: ~NOT() throw() {}
        
        NOT:: NOT() throw() : logical(tag) { self = (NOT *)this; }
        
        NOT:: NOT( const NOT &other ) : logical(other) { self = (NOT *)this;  }
        
        pattern * NOT:: clone() const
        {
            return new NOT(*this);
        }
        
        logical * NOT:: create()
        {
            return new NOT();
        }
        
        bool NOT:: match(source &src, ios::istream &fp)
        {
            assert(0==size);
            for(pattern *op=operands.head;op;op=op->next)
            {
                if(op->match(src,fp))
                {
                    src.unget(*op);
                    return false;
                }
            }
            
            return true;
        }

        void NOT:: viz(ios::ostream &fp) const
        {
            fp.viz(this); fp << " [ label=\"!=\"];\n";
            __viz(this,fp);
        }

    }
    
}



