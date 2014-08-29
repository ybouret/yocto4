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
        
        
    }
    
    
}

namespace yocto
{
    namespace lang
    {
        AND:: ~AND() throw() {}
        
        AND:: AND() throw() : logical(tag) {}
        
        AND:: AND( const AND &other ) : logical(other) {}
        
        pattern * AND:: clone() const
        {
            return new AND(*this);
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
        
        
    }

}

namespace yocto
{
    namespace lang
    {
        OR:: ~OR() throw() {}
        
        OR:: OR() throw() : logical(tag) {}
        
        OR:: OR( const OR &other ) : logical(other) {}
        
        pattern * OR:: clone() const
        {
            return new OR(*this);
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
        
    }
    
}

namespace yocto
{
    namespace lang
    {
        NOT:: ~NOT() throw() {}
        
        NOT:: NOT() throw() : logical(tag) {}
        
        NOT:: NOT( const NOT &other ) : logical(other) {}
        
        pattern * NOT:: clone() const
        {
            return new NOT(*this);
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
        
    }
    
}



